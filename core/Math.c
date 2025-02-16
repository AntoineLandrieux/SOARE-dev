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
 * Antoine LANDRIEUX (MIT License) <Math.c>
 * <https://github.com/AntoineLandrieux/SOARE/>
 *
 */

#include <SOARE/SOARE.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Return 1 if the string is a number
 * @author Antoine LANDRIEUX
 *
 * @param string
 * @return u8
 */
static u8 isNaN(char *string)
{
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
 * @brief Copy a string
 * @author Antoine LANDRIEUX
 *
 * @param value
 * @return char*
 */
static char *vardup(char *value)
{
    char *result = strdup(value);
    if (!result)
        return __SOARE_OUT_OF_MEMORY();
    return result;
}

/**
 * @brief Convert float to string
 * @author Antoine LANDRIEUX
 *
 * @param number
 * @return char*
 */
static char *__float(double number)
{
    char string[50];
    sprintf(string, "%g", number);
    char *result = (char *)malloc(strlen(string));
    if (!result)
        return __SOARE_OUT_OF_MEMORY();
    strcpy(result, string);
    return result;
}

/**
 * @brief Looks up the mathematical priority of an operator
 * @author Antoine LANDRIEUX
 *
 * @param symbol
 * @return u8
 */
u8 MathPriority(char symbol)
{
    if (strchr("/*%^", symbol))
        return 0;
    else if (strchr("+-", symbol))
        return 1;
    else if (strchr("<=!>", symbol))
        return 2;
    return 3;
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
    Node *value = Branch((*tokens)->value, NODE_ROOT, (*tokens)->file);
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
        if ((*tokens)->type != TKN_FUNCTION)
            break;
        value->type = NODE_CALL;
        TokenNext(tokens);

        if ((*tokens)->type != TKN_PARENL)
        {
            free(value);
            return NULL;
        }

        TokenNext(tokens);

        while ((*tokens)->type != TKN_PARENR)
        {
            AST expr = ParseExpr(tokens, 0xFF);
            if (!expr)
            {
                TreeFree(value);
                return NULL;
            }
            BranchJoin(value, expr);
            if ((*tokens)->type != TKN_SEMICOLON)
                break;
            TokenNext(tokens);
        }

        if ((*tokens)->type != TKN_PARENR)
        {
            TreeFree(value);
            return NULL;
        }
        TokenNext(tokens);
        break;

    default:
        TreeFree(value);
        return NULL;
    }

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
    Node *left = ParseValue(tokens);
    Node *right = NULL;
    Node *symbol = NULL;

    if (!left)
        return NULL;

    while ((*tokens)->type == TKN_OPERATOR && !ErrorLevel())
    {
        u8 operator= MathPriority(*(*tokens)->value);

        if (operator>= priority)
            break;

        symbol = Branch((*tokens)->value, NODE_OPERATOR, (*tokens)->file);
        TokenNext(tokens);
        right = ParseExpr(tokens, priority);

        if (!symbol || !right)
        {
            TreeFree(left);
            TreeFree(right);
            TreeFree(symbol);
            return NULL;
        }

        BranchJoin(symbol, left);
        BranchJoin(symbol, right);

        left = symbol;
    }

    return left;
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
    double dx, dy;
    char *sx, *sy;
    char *result = NULL;

    MEM get = NULL;

    switch (tree->type)
    {
    case NODE_MEMGET:

        get = MemGet(MEMORY, tree->value);
        if (!get)
            return LeaveException(UndefinedReference, tree->value, tree->file);
        return vardup(get->value);

    case NODE_CALL:

        return RunFunction(tree);

    case NODE_STRING:
    case NODE_NUMBER:

        return vardup(tree->value);

    case NODE_OPERATOR:

        sx = Math(tree->child);
        sy = Math(tree->child->sibling);

        if (!sx || !sy)
            return NULL;

        if (isNaN(sx) || isNaN(sy))
        {
            result = malloc(2);
            if (!result)
                return __SOARE_OUT_OF_MEMORY();

            switch (*(tree->value))
            {
            // AND
            case 'A':
            case 'a':
                snprintf(result, 2, "%d", !strcmp(sx, "0") && !strcmp(sy, "0"));
                break;
            // EQU
            case 'E':
            case 'e':
                snprintf(result, 2, "%d", !strcmp(sx, sy));
                break;
            // NEQ
            case 'N':
            case 'n':
                snprintf(result, 2, "%d", strcmp(sx, sy));
                break;
            // OR
            case 'O':
            case 'o':
                snprintf(result, 2, "%d", !strcmp(sx, "0") || !strcmp(sy, "0"));
                break;
            case '+':
                result = realloc(result, strlen(sx) + strlen(sy) + 1);
                if (!result)
                    return __SOARE_OUT_OF_MEMORY();
                strcat(strcpy(result, sx), sy);
                break;
            default:
                free(result);
                return LeaveException(MathError, tree->value, tree->file);
            }
            return result;
        }

        dx = atof(sx);
        dy = atof(sy);

        if (strchr("/%", *(tree->value)) && !dy)
            return LeaveException(DivideByZero, tree->value, tree->file);

        switch (*(tree->value))
        {
        // AND
        case 'A':
        case 'a':
            return __float(dx && dy);
        // EQU
        case 'E':
        case 'e':
            return __float(dx == dy);
        // NEQ
        case 'N':
        case 'n':
            return __float(dx != dy);
        // OR
        case 'O':
        case 'o':
            return __float(dx || dy);
        case '^':
            return __float((double)((int)dx ^ (int)dy));
        case '%':
            return __float((double)((int)dx % (int)dy));
        case '*':
            return __float(dx * dy);
        case '/':
            return __float(dx / dy);
        case '+':
            return __float(dx + dy);
        case '-':
            return __float(dx - dy);
        case '<':
            return __float(dx < dy);
        case '>':
            return __float(dx > dy);
        default:
            return LeaveException(MathError, tree->value, tree->file);
        }

    default:
        return LeaveException(MathError, tree->value, tree->file);
    }

    return NULL;
}
