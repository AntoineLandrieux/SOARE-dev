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

typedef struct MEM
{

    char *name;
    char *type;

    AST *value;

    struct MEM *next;

} MEM;

static MEM *MEMORY = NULL;
static MEM *MEM_PTR = NULL;

static void init_mem()
{
    if (MEMORY != NULL)
        return;

    MEMORY = (MEM *)malloc(sizeof(struct MEM));
    MEM_PTR = MEMORY;

    if (MEMORY == NULL)
        LeaveException(InterpreterError, "OUT OF MEMORY", EmptyDocument());
    else
        MEMORY->next = NULL;
}

static void push_mem(char *_Name, char *_Type, AST *_Value)
{
    if (MEM_PTR == NULL)
        return;

    MEM_PTR->name = _Name;
    MEM_PTR->type = _Type;
    MEM_PTR->value = _Value;
    MEM_PTR->next = (MEM *)malloc(sizeof(struct MEM));

    if (MEM_PTR->next != NULL)
        MEM_PTR = MEM_PTR->next;
}

static void free_mem()
{
    if (MEMORY == NULL)
        return;

    while (1)
    {
        MEM *old = MEMORY;
        MEMORY = MEMORY->next;

        free(old);

        if (MEMORY == NULL)
            break;
    }

    MEMORY = NULL;
    MEM_PTR = NULL;
}

static MEM *get_mem(char *_Name)
{
    for (MEM *curr = MEMORY; curr != NULL; curr = curr->next)
        if (!strcmp(curr->name, _Name))
            return curr;
    return NULL;
}

static char *RunMaths(AST *_Tree)
{

    char *tmp = malloc(int_min(strlen(_Tree->value) + 1, 11));

    char *sx;
    char *sy;
    i32 ix = 0;
    i32 iy = 0;

    size_t size = 0;
    strcpy(tmp, "0");

    switch (_Tree->type)
    {
    case NODE_STRING:

        strcpy(tmp, _Tree->value);
        break;

    case NODE_NUMBER:

        itoa(atoi(_Tree->value), tmp, 10);
        break;

    case NODE_OPERATOR:

        sx = RunMaths(_Tree->child);
        sy = RunMaths(_Tree->child->sibling);
        
        size = strlen(sx) + strlen(sy) + 1;

        if (*(_Tree->value) == ',')
        {
            tmp = realloc(tmp, size);
            strcat(strcpy(tmp, sx), sy);
            break;
        }

        ix = atoi(sx);
        iy = atoi(sy);

        if (strchr("/%", *(_Tree->value)) != NULL && !iy)
        {
            LeaveException(DivideByZero, _Tree->value, _Tree->file);
            break;
        }

        switch (*(_Tree->value))
        {
        case '*':
            itoa(ix * iy, tmp, 10);
            break;

        case '/':
            itoa(ix / iy, tmp, 10);
            break;

        case '%':
            itoa(ix % iy, tmp, 10);
            break;

        case '+':
            itoa(ix + iy, tmp, 10);
            break;

        case '-':
            itoa(ix - iy, tmp, 10);
            break;

        default:
            break;
        }

    default:
        break;
    }

    return tmp;
}

char *Runtime(AST *_Tree)
{
    if (_Tree == NULL)
        return NULL;

    AST *root = _Tree;
    char *returned = NULL;

    for (AST *curr = root->child; curr; curr = curr->sibling)
    {
        switch (curr->type)
        {
        case NODE_IMPORT:
            // TODO: implement import
            break;

        case NODE_TRY:

            ClearException();
            IgnoreException(0x1);
            returned = Runtime(curr->child);
            IgnoreException(0x0);

            if (ErrorLevel())
                returned = Runtime(curr->child->sibling);

            if (returned != NULL)
                return returned;

            break;

        case NODE_OUTPUT:
            returned = RunMaths(curr->child);
            printf("%s\n", returned);
            free(returned);
            break;

        case NODE_RAISE:
            return LeaveException(curr->value, "raise", curr->file);

        default:
            break;
        }
    }

    return NULL;
}

/**
 * @brief
 *
 * @param _RawCode
 */
void Execute(char *_RawCode)
{
    init_mem();

    Tokens *tokens = Tokenizer(NULL, _RawCode);
    AST *parsed = Parse(tokens);

    Runtime(parsed);
    // Looking for __main__!()

    TokensFree(tokens);
    TreeFree(parsed);
}