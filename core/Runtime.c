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
 */

#include <SOARE/SOARE.h>
#include <SOARE/utils/int.h>
#include <SOARE/utils/keywords.h>

MEM *MEMORY = NULL;

static u8 broken = 0;

char *Runtime(char *_Type, AST *_Tree)
{
    if (_Tree == NULL)
        return NULL;

    AST *root = _Tree;

    if (MEMORY == NULL)
        MEMORY = Mem();

    MEM *MEM_PTR = MemLast(MEMORY);

#define __FREE_MEM                              \
    if (MEM_PTR != MEMORY)                      \
        MEM_PTR->next = MemFree(MEM_PTR->next); \
    else                                        \
        MEMORY = MemFree(MEMORY);

    double condition = 0;

    for (AST *curr = root->child; curr && !ErrorLevel(); curr = curr->sibling)
    {
        char *returned = NULL;
        AST *tmp = NULL;

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

        case NODE_CONDITION:

            condition = MathFloat(curr->child);
            tmp = curr->child;

            while (1)
            {
                if (condition)
                {
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

                condition = MathFloat(tmp);
            }

            break;

        case NODE_REPETITION:

            condition = MathFloat(curr->child);
            broken = 0;

            while (condition)
            {
                returned = Runtime(_Type, curr->child->sibling);

                if (returned != NULL || broken)
                {
                    __FREE_MEM;
                    return returned;
                }

                condition = MathFloat(curr->child);
            }

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

            returned = MathString(curr->child);

            if (returned != NULL)
                printf("%s\n", returned);

            break;

        case NODE_RETURN:
            __FREE_MEM;
            return Math(_Type, curr->child);

        case NODE_RAISE:
            __FREE_MEM;
            return LeaveException(curr->value, KEYWORD_RAISE, curr->file);

        default:
            break;
        }

        free(returned);
    }

#ifdef __SOARE_DEBUG
    MemLog(MEMORY);
#endif

    __FREE_MEM;
    return NULL;
}

/**
 * @brief
 *
 * @param _RawCode
 */
int Execute(char *_RawCode)
{
    ClearException();

    Tokens *tokens = Tokenizer(__SOARE_FILE__, _RawCode);
    AST *ast = Parse(tokens);

#ifdef __SOARE_DEBUG
    TokensLog(tokens);
    TreeLog(ast);
#endif

    free(Runtime(TYPE_INT, ast));

    TokensFree(tokens);
    TreeFree(ast);

    return (int)ErrorLevel();
}
