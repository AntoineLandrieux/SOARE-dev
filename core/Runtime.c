#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 *  _____  _____  ___  ______ _____
 * /  ___||  _  |/ _ \ | ___ \  ___|
 * \ `--. | | | / /_\ \| |_/ / |__
 *  `--. \| | | |  _  ||    /|  __|
 * /\__/ /\ \_/ / | | || |\ \| |___
 * \____/  \___/\_| |_/\_| \_\____/
 *
 * Antoine LANDRIEUX (MIT License) <Runtime.c>
 * <https://github.com/AntoineLandrieux/SOARE/>
 *
 * [!] Contribute and help me translate the comments!
 *
 */

#include <SOARE/SOARE.h>
#include <SOARE/utils/int.h>
#include <SOARE/utils/keywords.h>

MEM MEMORY = NULL;
static MEM FUNCTION = NULL;

/**
 * @brief From file file
 * @author Antoine LANDRIEUX
 * 
 * @param _Filename 
 * @return void* 
 */
static void *RunFromFile(char *_Filename)
{
    FILE *file = fopen(_Filename, "rb");

    if (!file)
        return LeaveException(FileError, _Filename, EmptyDocument());

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char *content = (char *)malloc(size);

    if (content == NULL)
    {
        fclose(file);
        return LeaveException(InterpreterError, _Filename, EmptyDocument());
    }

    fread(content, sizeof(char), size, file);
    content[size] = 0;
    Execute(_Filename, content);
    free(content);
    fclose(file);
    return NULL;
}

/**
 * @brief Execute a function
 * @author Antoine LANDRIEUX
 *
 * @deprecated
 *
 * @param _Tree
 * @return char*
 */
char *RunFunction(AST _Tree)
{
    AST func = BranchFind(_Tree->parent->child, _Tree->value, NODE_FUNCTION);

    if (func == NULL)
        return LeaveException(UndefinedReference, _Tree->value, _Tree->file);

    AST ptr = func->child;
    AST src = _Tree->child;
    FUNCTION = Mem();

    while (ptr != NULL)
    {
        if (ptr->type == NODE_BODY)
            return Runtime(ptr);

        if (src == NULL)
        {
            FUNCTION = MemFree(FUNCTION);
            return LeaveException(UndefinedReference, ptr->value, _Tree->file);
        }

        MemPush(FUNCTION, ptr->value, Math(src));
        src = src->sibling;
        ptr = ptr->sibling;
    }

    return NULL;
}

/**
 * @brief Executes code from a tree
 * @author Antoine LANDRIEUX
 *
 * @param _Tree
 * @return char*
 */
char *Runtime(AST _Tree)
{
    if (_Tree == NULL)
        return NULL;

    AST root = _Tree;

    if (MEMORY == NULL)
        MEMORY = Mem();

    MEM statement = MemLast(MEMORY);
    MemJoin(MEMORY, FUNCTION);
    FUNCTION = NULL;

    for (AST curr = root->child; curr && !ErrorLevel(); curr = curr->sibling)
    {
        char *returned = NULL;
        int enumerate = 0;
        AST tmp = NULL;
        MEM get = NULL;

        switch (curr->type)
        {
        case NODE_IMPORT:
            RunFromFile(curr->value);
            break;

        case NODE_CONTINUE:
            statement->next = MemFree(statement->next);
            return NULL;

        case NODE_CALL:
            free(RunFunction(curr));
            break;

        case NODE_MEMSET:
            get = MemGet(MEMORY, curr->value);
            returned = Math(curr->child);
            if (MemSet(get, returned) == NULL)
                get = MemPush(statement, curr->value, returned);
            break;

        case NODE_ENUMERATE:
            for (tmp = curr->child; tmp != NULL; tmp = tmp->sibling)
            {
                returned = malloc(((int)(enumerate / 10)) + 2);
                snprintf(returned, (((int)(enumerate / 10))) + 2, "%d", enumerate);
                MemPush(
                    statement,
                    tmp->value,
                    returned);
                enumerate += 1;
            }
            break;

        case NODE_CONDITION:

            returned = Math(curr->child);
            tmp = curr->child;

            while (1)
            {
                if (returned == NULL)
                    break;

                if (strcmp(returned, "0"))
                {
                    free(returned);
                    returned = Runtime(tmp->sibling);
                    if (returned != NULL)
                    {
                        statement->next = MemFree(statement->next);
                        return returned;
                    }
                    break;
                }

                tmp = tmp->sibling->sibling;
                if (tmp == NULL)
                    break;

                free(returned);
                returned = Math(tmp);
            }

            free(returned);
            break;

        case NODE_REPETITION:

            returned = Math(curr->child);

            while (returned != NULL)
            {
                if (!strcmp(returned, "0"))
                    break;
                free(returned);
                returned = Runtime(curr->child->sibling);
                if (returned != NULL)
                {
                    statement->next = MemFree(statement->next);
                    return returned;
                }
                returned = Math(curr->child);
            }

            free(returned);
            break;

        case NODE_TRY:

            IgnoreException(0x1);
            returned = Runtime(curr->child);
            IgnoreException(0x0);
            if (ErrorLevel())
            {
                free(returned);
                ClearException();
                returned = Runtime(curr->child->sibling);
            }
            if (returned != NULL)
            {
                statement->next = MemFree(statement->next);
                return returned;
            }
            break;

        case NODE_OUTPUT:
            returned = Math(curr->child);
            if (returned != NULL)
                printf("%s\n", returned);
            break;

        case NODE_RETURN:
            returned = Math(curr->child);
            statement->next = MemFree(statement->next);
            return returned;

        case NODE_RAISE:
            statement->next = MemFree(statement->next);
            return LeaveException(curr->value, KEYWORD_RAISE, curr->file);

        default:
            break;
        }
    }

    statement->next = MemFree(statement->next);
    return NULL;
}

/**
 * @brief Execute le code à partir d'une chaîne de caractères
 * @author Antoine LANDRIEUX
 *
 * @param _RawCode
 */
int Execute(char *_File, char *_RawCode)
{
    Tokens *tokens = Tokenizer(_File, _RawCode);
    AST ast = Parse(tokens);

#ifdef __SOARE_DEBUG
    TokensLog(tokens);
    TreeLog(ast);
#endif

    free(Runtime(ast));

#ifdef __SOARE_DEBUG
    MemLog(MEMORY);
#endif

    MEMORY = MemFree(MEMORY);
    TokensFree(tokens);
    TreeFree(ast);

    return (int)ErrorLevel();
}
