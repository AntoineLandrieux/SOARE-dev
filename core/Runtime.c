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
 * Antoine LANDRIEUX (WTFPL) <Runtime.c>
 * <https://github.com/AntoineLandrieux/SOARE/>
 *
 * [!] Contribute and help me translate the comments!
 *
 */

#include <SOARE/SOARE.h>
#include <SOARE/utils/int.h>
#include <SOARE/utils/keywords.h>

MEM *MEMORY = NULL;

static u8 broken = 0;

/**
 * @brief Execute le code à partir d'un arbre et retourne une valeur en fonction du type
 * @author Antoine LANDRIEUX
 *
 * @param _Type
 * @param _Tree
 * @return char*
 */
char *Runtime(char *_Type, AST *_Tree)
{
    if (_Type == NULL || _Tree == NULL)
        return NULL;

    AST *root = _Tree;

    if (MEMORY == NULL)
        MEMORY = Mem();

    MEM *MEM_PTR = MemLast(MEMORY);

#define __FREE_MEM         \
    if (MEM_PTR != MEMORY) \
        MEM_PTR->next = MemFree(MEM_PTR->next);

    for (AST *curr = root->child; curr && !ErrorLevel(); curr = curr->sibling)
    {
        char *returned = NULL;
        int enumerate = 0;
        AST *tmp = NULL;
        MEM *get = NULL;

        switch (curr->type)
        {
        case NODE_IMPORT:
            // TODO: implement import
            break;

        case NODE_BREAK:
            broken = 1;
        case NODE_CONTINUE:
            __FREE_MEM;
            return NULL;

        case NODE_MEMCREATE:
            MemPush(
                MEM_PTR,
                curr->value,
                curr->child->value,
                Math(curr->child->value, curr->child->sibling));
            break;

        case NODE_MEMSET:
            get = MemGet(MEMORY, curr->value);
            if (get == NULL)
            {
                __FREE_MEM;
                return LeaveException(UndefinedReference, curr->value, curr->file);
            }
            MemSet(get, Math(get->type, curr->child));

        case NODE_ENUMERATE:
            for (tmp = curr->child; tmp != NULL; tmp = tmp->sibling)
            {
                returned = malloc(((int)(enumerate / 10)) + 2);
                snprintf(returned, (((int)(enumerate / 10))) + 2, "%d", enumerate);
                MemPush(
                    MEM_PTR,
                    tmp->value,
                    TYPE_NUMBER,
                    returned);
                enumerate += 1;
            }
            break;

        case NODE_CONDITION:

            returned = Math(TYPE_NUMBER, curr->child);
            tmp = curr->child;

            while (1)
            {
                if (returned == NULL)
                    break;

                if (strcmp(returned, "0"))
                {
                    free(returned);
                    returned = Runtime(_Type, tmp->sibling);
                    if (returned != NULL)
                    {
                        __FREE_MEM;
                        return returned;
                    }
                    break;
                }

                tmp = tmp->sibling->sibling;
                if (tmp == NULL)
                    break;

                free(returned);
                returned = Math(TYPE_NUMBER, tmp);
            }

            free(returned);

            if (broken)
                return NULL;

            break;

        case NODE_REPETITION:

            returned = Math(TYPE_NUMBER, curr->child);
            broken = 0;

            if (returned == NULL)
                break;

            while (strcmp(returned, "0") && !broken)
            {
                free(returned);
                returned = Runtime(_Type, curr->child->sibling);
                if (returned != NULL || broken)
                {
                    __FREE_MEM;
                    return returned;
                }
                returned = Math(TYPE_NUMBER, curr->child);
            }

            free(returned);
            break;

        case NODE_TRY:

            IgnoreException(0x1);
            returned = Runtime(_Type, curr->child);
            IgnoreException(0x0);

            if (ErrorLevel())
            {
                ClearException();
                returned = Runtime(_Type, curr->child->sibling);
            }

            if (returned != NULL)
            {
                __FREE_MEM;
                return returned;
            }

            break;

        case NODE_OUTPUT:
            returned = Math(TYPE_STRING, curr->child);
            printf("%s\n", returned == NULL ? "" : returned);
            break;

        case NODE_RETURN:
            returned = Math(_Type, curr->child);
            __FREE_MEM;
            return returned;

        case NODE_RAISE:
            __FREE_MEM;
            return LeaveException(curr->value, KEYWORD_RAISE, curr->file);

        default:
            break;
        }
    }

    __FREE_MEM;
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
    ClearException();

    Tokens *tokens = Tokenizer(_File, _RawCode);
    AST *ast = Parse(tokens);

#ifdef __SOARE_DEBUG
    TokensLog(tokens);
    TreeLog(ast);
#endif

    free(Runtime(TYPE_NUMBER, ast));

#ifdef __SOARE_DEBUG
    MemLog(MEMORY);
#endif

    MEMORY = MemFree(MEMORY);
    TokensFree(tokens);
    TreeFree(ast);

    return (int)ErrorLevel();
}
