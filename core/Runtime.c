#include <DRIVER/memory.h>
#include <DRIVER/video.h>
#include <DRIVER/keyboard.h>

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
 */

#include <SOARE/SOARE.h>

MEM MEMORY = NULL;
static MEM FUNCTION = NULL;

static void InterpreterVar()
{
    MEMORY = Mem();

    MemPush(MEMORY, "__SOARE__", "SOARE Antoine LANDRIEUX (MIT LICENSE)");
    MemPush(MEMORY, "__BUILD__", __DATE__);

    MemPush(MEMORY, "__WRITE_END__", "\n");

    MemPush(MEMORY, "BC", "\b");
    MemPush(MEMORY, "CR", "\r");
    MemPush(MEMORY, "LN", "\n");
    MemPush(MEMORY, "TAB", "\t");
    MemPush(MEMORY, "CLS", "\033c\033[3J");
}

/**
 * @brief Execute a function
 * @author Antoine LANDRIEUX
 *
 * @deprecated
 *
 * @param tree
 * @return char*
 */
char *RunFunction(AST tree)
{
    AST func = BranchFind(tree->parent->child, tree->value, NODE_FUNCTION);

    if (!func)
        return LeaveException(UndefinedReference, tree->value);

    AST ptr = func->child;
    AST src = tree->child;
    FUNCTION = Mem();

    while (ptr)
    {
        if (ptr->type == NODE_BODY)
            return Runtime(ptr);

        if (!src)
        {
            FUNCTION = NULL;
            return LeaveException(UndefinedReference, ptr->value);
        }

        MemPush(FUNCTION, ptr->value, Eval(src));
        src = src->sibling;
        ptr = ptr->sibling;
    }

    return NULL;
}

/**
 * @brief Executes code from a tree
 * @author Antoine LANDRIEUX
 *
 * @param tree
 * @return char*
 */
char *Runtime(AST tree)
{
    if (!tree)
        return NULL;

    AST root = tree;
    MEM statement = MemLast(MEMORY);
    MemJoin(statement, FUNCTION);
    FUNCTION = NULL;

    for (AST curr = root->child; curr && !ErrorLevel(); curr = curr->sibling)
    {
        char *returned = NULL;
        char buff[100] = {0};
        i64 num = 0;
        AST tmp = NULL;
        MEM get = NULL;

        switch (curr->type)
        {
        case NODE_INPUT:

            if ((get = MemGet(MEMORY, curr->value)))
            {
                buff[0] = GETC();
                MemSet(get, buff);
                break;
            }
            LeaveException(UndefinedReference, curr->value);
            break;

        case NODE_CALL:

            RunFunction(curr);
            break;

        case NODE_MEMNEW:

            MemPush(statement, curr->value, Eval(curr->child));
            break;

        case NODE_MEMSET:

            get = MemGet(MEMORY, curr->value);

            if (!get)
            {
                LeaveException(UndefinedReference, curr->value);
                break;
            }

            num = GetArrayIndex(curr->child, get->value);
            returned = Eval(num < 0 ? curr->child : curr->child->sibling);

            if (!returned)
                break;

            if (num >= 0)
            {
                get->value[num] = returned[0];
                break;
            }

            MemSet(get, returned);
            break;

        case NODE_ENUMERATE:

            for (tmp = curr->child; tmp; tmp = tmp->sibling)
            {
                lltoa(buff, sizeof(buff), num);
                MemPush(statement, tmp->value, buff);
                num += 1;
            }
            break;

        case NODE_CONDITION:

            returned = Eval(curr->child);
            tmp = curr->child;

            while (returned)
            {
                if (strcmp(returned, "0"))
                {
                    if ((returned = Runtime(tmp->sibling)))
                    {
                        statement->next = NULL;
                        return returned;
                    }
                    break;
                }

                if (!(tmp = tmp->sibling->sibling))
                    break;

                returned = Eval(tmp);
            }

            break;

        case NODE_REPETITION:

            while ((returned = Eval(curr->child)))
            {
                if (!strcmp(returned, "0") || ErrorLevel())
                    break;
                if ((returned = Runtime(curr->child->sibling)))
                {
                    statement->next = NULL;
                    return returned;
                }
            }
            break;

        case NODE_TRY:

            num = (i64)AsIgnoredException();
            IgnoreException(0x1);
            returned = Runtime(curr->child);
            IgnoreException((u8)num);

            if (ErrorLevel())
            {
                ClearException();
                returned = Runtime(curr->child->sibling);
            }

            if (!returned)
                break;

            statement->next = NULL;
            return returned;

        case NODE_OUTPUT:

            if ((returned = Eval(curr->child)))
            {
                PUTS(returned);
                PUTS(MemGet(MEMORY, "__WRITE_END__")->value);
            }
            break;

        case NODE_RETURN:

            returned = Eval(curr->child);
            statement->next = NULL;
            return returned;

        case NODE_RAISE:

            statement->next = NULL;
            return LeaveException(RaiseException, curr->value);

        default:
            break;
        }
    }

    statement->next = NULL;
    return NULL;
}

/**
 * @brief Execute the code from a string
 * @author Antoine LANDRIEUX
 *
 * @param rawcode
 */
int Execute(char *rawcode)
{
    ClearException();
    if (MEMORY == NULL)
        InterpreterVar();
    Runtime(Parse(Tokenizer(rawcode)));
    return (int)ErrorLevel();
}
