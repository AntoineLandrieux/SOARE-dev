#include <DRIVER/video.h>

#include <STD/stdlib.h>

/**
 *  _____  _____  ___  ______ _____
 * /  ___||  _  |/ _ \ | ___ \  ___|
 * \ `--. | | | / /_\ \| |_/ / |__
 *  `--. \| | | |  _  ||    /|  __|
 * /\__/ /\ \_/ / | | || |\ \| |___
 * \____/  \___/\_| |_/\_| \_\____/
 *
 * Antoine LANDRIEUX (MIT License) <Math.c>
 * <https://github.com/AntoineLandrieux/SOARE/>
 *
 */

#include <SOARE/SOARE.h>

/* A clean way to write (*tokens) = (*tokens)->next */
#define __tokens_next() (*tokens) = (*tokens)->next

/**
 * @brief Returns 1 if the string is Not a Number
 *
 * @param string
 * @return unsigned char
 */
static unsigned char isNaN(char *string)
{
    // Is negative ?
    if (*string == '-')
        string++;

    /**
     *
     * Example:
     *
     * -4   : Number (returns 0)
     * 3.2  : Number (returns 0)
     * -1.  : Number (returns 0)
     * .    : Number (returns 0)
     * 0.e  : Not a Number (returns 1)
     * a.1  : Not a Number (returns 1)
     * f    : Not a Number (returns 1)
     *
     */

    for (unsigned char dot = 1; *string; string++)
        if (*string == '.' && dot)
            dot = 0;
        else if (*string < '0' || *string > '9')
            return 1;

    return 0;
}

/**
 * @brief Duplicate a string
 *
 * @param value
 * @return char*
 */
char *vardup(char *string)
{
    char *result = malloc(strlen(string) + 1);
    if (!result)
        return __SOARE_OUT_OF_MEMORY();
    strcat(result, string);
    return result;
}

/**
 * @brief Convert int to string
 *
 * @param number
 * @return char*
 */
static char *__int(int number)
{
    char string[150] = {0};
    itoa(string, sizeof(string), number);
    return vardup(string);
}

/**
 * @brief Convert boolean to string
 *
 * @param boolean
 * @return char*
 */
static char *__boolean(char boolean)
{
    // Convert char:1 to string
    char string[2] = {0, 0};

    // Now convert
    string[0] = '0' + (boolean && 1);

    // Duplicate string
    return vardup(string);
}

/**
 * @brief Looks up the mathematical priority of an operator
 *
 * @param symbol
 * @return unsigned char
 */
static unsigned char MathPriority(char symbol)
{
    if (strchr("/*%^", symbol))
        return 1;
    else if (strchr("+-", symbol))
        return 2;
    else if (strchr("<=!>", symbol))
        return 3;
    return 4;
}

/**
 * @brief Return the value as a node
 *
 * @param tokens
 * @return AST
 */
AST ParseValue(Tokens **tokens)
{
    Node *value = Branch((*tokens)->value, NODE_ROOT);
    Tokens *old = *tokens;

    __tokens_next();

    switch (old->type)
    {
    case TKN_STRING:
    case TKN_NUMBER:

        value->type = NODE_VALUE;
        break;

    case TKN_NAME:

        value->type = NODE_MEMGET;
        if ((*tokens)->type != TKN_PARENL)
            break;

        value->type = NODE_CALL;
        __tokens_next();
        AST expr = NULL;

        while ((*tokens)->type != TKN_PARENR)
        {
            if (!(expr = ParseExpr(tokens, 0xF)))
                return NULL;

            BranchJoin(value, expr);

            if ((*tokens)->type != TKN_SEMICOLON)
                break;

            __tokens_next();
        }

        if ((*tokens)->type != TKN_PARENR)
            return NULL;

        __tokens_next();
        break;

    default:

        return NULL;
    }

    return value;
}

/**
 * @brief Build a math tree
 *
 * @param tokens
 * @param priority
 * @return AST
 */
AST ParseExpr(Tokens **tokens, unsigned char priority)
{

    /**
     *
     * Example:
     *
     * Tokens: ["4"]->["+"]->["3"]->["*"]->["2.5"]
     *
     * Result:
     *
     *    (+)
     *    /
     *  (4)-(*)
     *      /
     *    (3)-(2.5)
     *
     */

    Node *x = ParseValue(tokens);
    Node *y = NULL;
    Node *symbol = NULL;

    if (!x)
        return NULL;

    while ((*tokens)->type == TKN_OPERATOR && !ErrorLevel())
    {
        unsigned char op = MathPriority(*(*tokens)->value);

        if (op >= priority)
            break;

        symbol = Branch((*tokens)->value, NODE_OPERATOR);
        __tokens_next();
        y = ParseExpr(tokens, op);

        if (!symbol || !y)
            return NULL;

        BranchJoin(symbol, x);
        BranchJoin(symbol, y);
        x = symbol;
    }

    return x;
}

/**
 * @brief Evaluate the mathematical expression of a tree
 *
 * @param tree
 * @return char*
 */
char *Math(AST tree)
{
    int x, y;
    char *sx, *sy;
    char *result = NULL;

    MEM get = NULL;

    switch (tree->type)
    {
    case NODE_MEMGET:

        get = MemGet(MEMORY, tree->value);

        if (!get)
            return LeaveException(UndefinedReference, tree->value);

        if (get->body)
            return LeaveException(VariableDefinedAsFunction, tree->value);

        return vardup(get->value);

    case NODE_CALL:

        return RunFunction(tree);

    case NODE_VALUE:

        return vardup(tree->value);

    case NODE_OPERATOR:

        sx = Math(tree->child);
        sy = Math(tree->child->sibling);

        if (!sx || !sy)
            return NULL;

        if (*(tree->value) == ',')
        {
            if (!(result = malloc(strlen(sx) + strlen(sy) + 1)))
                return __SOARE_OUT_OF_MEMORY();

            strcat(result, sx);
            strcat(result, sy);
            return result;
        }

        if (isNaN(sx) || isNaN(sy))
        {
            switch (*(tree->value))
            {
            case '=':
                result = __boolean(!strcmp(sx, sy));
                return result;

            case '~':
            case '!':
                result = __boolean(strcmp(sx, sy));
                return result;

            default:
                return LeaveException(MathError, tree->value);
            }
        }

        x = atoi(sx);
        y = atoi(sy);

        if ((*(tree->value) == '/' || *(tree->value) == '%') && !y)
            return LeaveException(DivideByZero, tree->value);

        switch (*(tree->value))
        {
        case '~':
        case '!':
            return __boolean(x != y);

        case '=':
            return __boolean(x == y);

        case '&':
            return __boolean(x && y);

        case '|':
            return __boolean(x || y);

        case '^':
            return __int((int)x ^ (int)y);

        case '%':
            return __int((int)x % (int)y);

        case '*':
            return __int(x * y);

        case '/':
            return __int(x / y);

        case '+':
            return __int(x + y);

        case '-':
            return __int(x - y);

        case '<':
            return __boolean(x < y || (x == y && tree->value[1] == '='));

        case '>':
            return __boolean(x > y || (x == y && tree->value[1] == '='));

        default:
            return LeaveException(MathError, tree->value);
        }

    default:
        return LeaveException(MathError, tree->value);
    }

    return NULL;
}
