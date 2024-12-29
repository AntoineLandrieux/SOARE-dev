#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

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

static u8 broken = 0;

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

static u8 isNaN(char *_Number)
{
    char *endptr;
    strtod(_Number, &endptr);
    return *endptr != '\0';
}

static char *RunMaths(AST *_Tree)
{

    char *tmp = malloc(int_max(strlen(_Tree->value) + 1, 12));

    if (tmp == NULL)
        return strdup("0");

    char *sx;
    char *sy;
    double dx = 0;
    double dy = 0;

    size_t size = 0;
    strcpy(tmp, "0");

    switch (_Tree->type)
    {
    case NODE_ARRAY:

        strcpy(tmp, "(Array)");
        break;

    case NODE_STRING:

        strcpy(tmp, _Tree->value);
        break;

    case NODE_NUMBER:

        sprintf(tmp, "%.f", atof(_Tree->value));
        break;

    case NODE_OPERATOR:

        sx = RunMaths(_Tree->child);
        sy = RunMaths(_Tree->child->sibling);

        if (*(_Tree->value) == '+' && (isNaN(sx) || isNaN(sy)))
        {
            size = strlen(sx) + strlen(sy) + 1;
            tmp = realloc(tmp, size);
            strcat(strcpy(tmp, sx), sy);
            break;
        }

        else if (!strcasecmp(_Tree->value, "equ") || !strcasecmp(_Tree->value, "neq"))
        {
            strcpy(tmp, (strcasecmp(_Tree->value, "neq") ? !strcmp(sx, sy) : strcmp(sx, sy)) ? "1" : "0");
            break;
        }

        dx = atof(sx);
        dy = atof(sy);

        if (!strcasecmp(_Tree->value, "and") || !strcasecmp(_Tree->value, "or"))
        {
            strcpy(tmp, (strcasecmp(_Tree->value, "or") ? dx && dy : dx || dy) ? "1" : "0");
            break;
        }

        if (strchr("/%", *(_Tree->value)) != NULL && !dy)
        {
            free(tmp);
            return LeaveException(DivideByZero, _Tree->value, _Tree->file);
        }

        switch (*(_Tree->value))
        {
        case '*':
            sprintf(tmp, "%.f", dx * dy);
            break;

        case '^':
            sprintf(tmp, "%.f", pow(dx, dy));
            break;

        case '/':
            sprintf(tmp, "%.f", dx / dy);
            break;

        case '%':
            itoa((int)dx % (int)dy, tmp, 10);
            break;

        case '+':
            sprintf(tmp, "%.f", dx + dy);
            break;

        case '-':
            sprintf(tmp, "%.f", dx - dy);
            break;

        case '<':
            sprintf(tmp, "%d", dx < dy);
            break;

        case '>':
            sprintf(tmp, "%d", dx > dy);
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
            return NULL;

        case NODE_CONDITION:

            returned = RunMaths(curr->child);
            tmp = curr->child;

            while (1)
            {
                if (strcmp(returned == NULL ? "0" : returned, "0"))
                {
                    free(returned);
                    returned = Runtime(tmp->sibling);

                    if (returned != NULL)
                        return returned;
                    
                    if (broken)
                        break;

                    break;
                }
                else
                {
                    tmp = tmp->sibling->sibling;

                    if (tmp == NULL)
                        break;
                        
                    returned = RunMaths(tmp);
                }
            }
            

            break;

        case NODE_REPETITION:

            returned = RunMaths(curr->child);
            broken = 0;

            while (strcmp(returned == NULL ? "0" : returned, "0"))
            {
                free(returned);
                returned = Runtime(curr->child->sibling);

                if (returned != NULL || broken)
                    return returned;

                returned = RunMaths(curr->child);
            }

            broken = 0;
            break;

        case NODE_TRY:

            IgnoreException(0x1);
            returned = Runtime(curr->child);
            IgnoreException(0x0);

            if (ErrorLevel())
            {
                ClearException();
                returned = Runtime(curr->child->sibling);
            }

            if (returned != NULL)
                return returned;

            break;

        case NODE_OUTPUT:

            returned = RunMaths(curr->child);

            if (returned != NULL)
                printf("%s\n", returned);

            break;

        case NODE_RETURN:
            return RunMaths(curr->child);

        case NODE_RAISE:
            return LeaveException(curr->value, "raise", curr->file);

        default:
            break;
        }

        free(returned);
    }

    return NULL;
}

/**
 * @brief
 *
 * @param _RawCode
 */
int Execute(char *_RawCode)
{
    init_mem();
    ClearException(0x0);

    Tokens *tokens = Tokenizer(NULL, _RawCode);
    AST *ast = Parse(tokens);

#ifdef __SOARE_DEBUG
    TokensLog(tokens);
    TreeLog(ast);
#endif

    free(Runtime(ast));
    TokensFree(tokens);
    TreeFree(ast);
    free_mem();

    return (int)ErrorLevel();
}