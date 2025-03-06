#include <DRIVER/memory.h>
#include <DRIVER/video.h>

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

/**
 * @brief Return 1 if the string is a number
 * @author Antoine LANDRIEUX
 *
 * @param string
 * @return u8
 */
static u8 isNaN(char *string)
{
    if (!string)
        return 1;
    if (*string == '+' || *string == '-')
        (volatile char *)string++;
    for (u8 dot = 1; *string; (volatile char *)string++)
        if (*string == '.' && dot)
            dot = 0;
        else if (!(*string >= '0' && *string <= '9'))
            return 1;
    return 0;
}

/**
 * @brief Convert float to string
 * @author Antoine LANDRIEUX
 *
 * @param number
 * @return char*
 */
static char *__long(long long number)
{
    char string[250] = {0};
    lltoa(string, sizeof(string), number);
    char *result = malloc(strlen(string));
    if (!result)
        return __SOARE_OUT_OF_MEMORY();
    stradd(result, string, strlen(string));
    return result;
}

/**
 * @brief Looks up the mathematical priority of an operator
 * @author Antoine LANDRIEUX
 *
 * @param symbol
 * @return u8
 */
static u8 MathPriority(char symbol)
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
 * @brief Return the value as an array
 * @author Antoine LANDRIEUX
 *
 * @param tokens
 * @return AST
 */
static AST ParseArray(Tokens **tokens)
{
    if ((*tokens)->type != TKN_ARRAYL)
        return NULL;
    TokenNext(tokens);
    AST value = ParseExpr(tokens, 0xF);
    if ((*tokens)->type != TKN_ARRAYR)
        return NULL;
    TokenNext(tokens);
    return BranchJoin(Branch("ARRAY", NODE_ARRAY), value);
}

/**
 * @brief Return the value as a node
 * @author Antoine LANDRIEUX
 *
 * @param tokens
 * @return AST
 */
AST ParseValue(Tokens **tokens)
{
    Node *value = Branch((*tokens)->value, NODE_ROOT);
    Tokens *old = *tokens;
    TokenNext(tokens);

    switch (old->type)
    {
    case TKN_NUMBER:
        value->type = NODE_NUMBER;
        break;

    case TKN_STRING:
        value->type = NODE_STRING;
        break;

    case TKN_NAME:
        value->type = NODE_MEMGET;
        if ((*tokens)->type != TKN_PARENL)
            break;

        value->type = NODE_CALL;
        TokenNext(tokens);
        AST expr = NULL;

        while ((*tokens)->type != TKN_PARENR)
        {
            if (!(expr = ParseExpr(tokens, 0xF)))
                return NULL;
            BranchJoin(value, expr);
            if ((*tokens)->type != TKN_SEMICOLON)
                break;
            TokenNext(tokens);
        }

        if ((*tokens)->type != TKN_PARENR)
            return NULL;
        TokenNext(tokens);
        break;

    default:
        return NULL;
    }

    BranchJoin(value, ParseArray(tokens));
    return value;
}

/**
 * @brief Build a math tree
 * @author Antoine LANDRIEUX
 *
 * @param tokens
 * @param priority
 * @return AST
 */
AST ParseExpr(Tokens **tokens, u8 priority)
{
    Node *x = ParseValue(tokens);
    Node *y = NULL;
    Node *symbol = NULL;

    if (!x)
        return NULL;

    while ((*tokens)->type == TKN_OPERATOR && !ErrorLevel())
    {
        u8 op = MathPriority(*(*tokens)->value);

        if (op >= priority)
            break;

        symbol = Branch((*tokens)->value, NODE_OPERATOR);
        TokenNext(tokens);
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
 * @brief Get the Array Index object
 * @author Antoine LANDRIEUX
 *
 * @param array
 * @param value
 * @return long long
 */
long long GetArrayIndex(AST array, char *value)
{
    while (array)
        if (array->type != NODE_ARRAY)
            array = array->sibling;
        else
            break;

    if (!array || !value)
        return -1;

    array = array->child;
    char *index = Eval(array);

    if (isNaN(index))
    {
        LeaveException(MathError, array->value);
        return -1;
    }

    long long indexlld = atoll(index);

    if (strlen(value) <= indexlld || indexlld < 0)
    {
        LeaveException(IndexOutOfRange, array->value);
        return -1;
    }

    return indexlld;
}

/**
 * @brief Array parser
 * @author Antoine LANDRIEUX
 *
 * @param value
 * @param array
 * @return char*
 */
static char *Array(char *value, AST array)
{
    long long index = GetArrayIndex(array, value);

    if (index < 0)
        return value;

    char *result = malloc(2);

    if (!result)
        return __SOARE_OUT_OF_MEMORY();

    0 [result] = value[index];
    1 [result] = 0;

    return result;
}

/**
 * @brief Evaluates the mathematical expression of a tree
 * @author Antoine LANDRIEUX
 *
 * @param tree
 * @return char*
 */
char *Math(AST tree)
{
    long long dx, dy;
    char *sx, *sy;
    char *result = NULL;

    MEM get = NULL;

    switch (tree->type)
    {
    case NODE_MEMGET:

        get = MemGet(MEMORY, tree->value);
        if (!get)
            return LeaveException(UndefinedReference, tree->value);
        return get->value;

    case NODE_CALL:

        return RunFunction(tree);

    case NODE_STRING:
    case NODE_NUMBER:

        return tree->value;

    case NODE_OPERATOR:

        sx = Eval(tree->child);
        sy = Eval(tree->child->sibling);

        if (!sx || !sy)
            return NULL;

        if (isNaN(sx) || isNaN(sy))
        {
            switch (*(tree->value))
            {
            case '&':
                result = malloc(2);
                if (!result)
                    return __SOARE_OUT_OF_MEMORY();
                lltoa(result, 2, *sx && *sy);
                break;
                result = malloc(2);
                if (!result)
                    return __SOARE_OUT_OF_MEMORY();
            case '=':
                lltoa(result, 2, !strcmp(sx, sy));
                break;
                result = malloc(2);
                if (!result)
                    return __SOARE_OUT_OF_MEMORY();
            case '!':
                lltoa(result, 2, strcmp(sx, sy));
                break;
                result = malloc(2);
                if (!result)
                    return __SOARE_OUT_OF_MEMORY();
            case '|':
                result = malloc(2);
                if (!result)
                    return __SOARE_OUT_OF_MEMORY();
                lltoa(result, 2, sx || sy);
                break;
            case '+':
                dx = strlen(sx);
                dy = strlen(sy);
                result = malloc(dx + dy + 2);
                if (!result || !stradd(result, sx, dx + 1) || !stradd(result, sy, dx + dy + 2))
                    return __SOARE_OUT_OF_MEMORY();
                break;
            default:
                return LeaveException(MathError, tree->value);
            }
            return result;
        }

        dx = atoll(sx);
        dy = atoll(sy);

        if (strchr("/%", *(tree->value)) && !dy)
            return LeaveException(DivideByZero, tree->value);

        switch (*(tree->value))
        {
        case '&':
            return __long(dx && dy);
        case '=':
            return __long(dx == dy);
        case '!':
            return __long(dx != dy);
        case '|':
            return __long(dx || dy);
        case '^':
            return __long(dx ^ dy);
        case '%':
            return __long((int)dx % (int)dy);
        case '*':
            return __long(dx * dy);
        case '/':
            return __long((int)dx / (int)dy);
        case '+':
            return __long(dx + dy);
        case '-':
            return __long(dx - dy);
        case '<':
            return __long(dx < dy || (dx == dy && tree->value[1] == '='));
        case '>':
            return __long(dx > dy || (dx == dy && tree->value[1] == '='));
        default:
            return LeaveException(MathError, tree->value);
        }

    default:
        return LeaveException(MathError, tree->value);
    }

    return NULL;
}

/**
 * @brief Evaluates the mathematical expression of a tree
 * @author Antoine LANDRIEUX
 *
 * @param tree
 * @return char*
 */
char *Eval(AST tree)
{
    char *string = NULL;
    if (tree)
        string = Array(Math(tree), tree->child);
    if (ErrorLevel())
        return NULL;
    return string;
}
