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
 *
 * Antoine LANDRIEUX (MIT License) <Math.c>
 * <https://github.com/AntoineLandrieux/SOARE/>
 *
 */

#include <SOARE/SOARE.h>
#include <SOARE/utils/int.h>
#include <SOARE/utils/keywords.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Return 1 if the string is a number
 * @author Antoine LANDRIEUX
 *
 * @param _String
 * @return u8
 */
static u8 isNaN(char *_String)
{
    for (u8 dot = 1; *_String; (volatile char *)_String++)
        if (*_String == '.' && dot)
            dot = 0;
        else if (!(*_String >= '0' && *_String <= '9'))
            return 1;
    return 0;
}

/**
 * @brief Copy a string
 * @author Antoine LANDRIEUX
 *
 * @param _Value
 * @return char*
 */
static char *vardup(char *_Value)
{
    char *result = strdup(_Value);
    if (result == NULL)
        return LeaveException(InterpreterError, "OUT OF MEMORY", EmptyDocument());
    return result;
}

/**
 * @brief Convert float to int
 * @author Antoine LANDRIEUX
 *
 * @param _Float
 * @return char*
 */
static char *__float(double _Float)
{
    char string[50];
    sprintf(string, "%g", _Float);
    char *result = (char *)malloc(strlen(string));
    if (result == NULL)
        return LeaveException(InterpreterError, "OUT OF MEMORY", EmptyDocument());
    strcpy(result, string);
    return result;
}

/**
 * @brief Evaluates the mathematical expression of a tree
 * @author Antoine LANDRIEUX
 *
 * @param _Tree
 * @return char*
 */
char *Math(AST _Tree)
{
    double dx, dy;
    char *sx, *sy;
    char *result = NULL;

    MEM get = NULL;

    switch (_Tree->type)
    {
    case NODE_MEMGET:

        get = MemGet(MEMORY, _Tree->value);
        if (get == NULL)
            return LeaveException(UndefinedReference, _Tree->value, _Tree->file);
        return vardup(get->value);

    case NODE_CALL:

        return RunFunction(_Tree);

    case NODE_STRING:
    case NODE_NUMBER:

        return vardup(_Tree->value);

    case NODE_OPERATOR:

        sx = Math(_Tree->child);
        sy = Math(_Tree->child->sibling);

        if (sx == NULL || sy == NULL)
            return NULL;

        result = malloc(2);
        if (result == NULL)
            LeaveException(InterpreterError, "OUT OF MEMORY", EmptyDocument());

        if (isNaN(sx) || isNaN(sy))
        {
            if (!strcasecmp(_Tree->value, "equ"))
                snprintf(result, 2, "%d", !strcmp(sx, sy));

            else if (!strcasecmp(_Tree->value, "neq"))
                snprintf(result, 2, "%d", strcmp(sx, sy));

            else if (*_Tree->value == '+')
            {
                result = realloc(result, strlen(sx) + strlen(sy) + 1);
                if (result == NULL)
                    return LeaveException(InterpreterError, "OUT OF MEMORY", EmptyDocument());
                strcat(strcpy(result, sx), sy);
                return result;
            }

            else
            {
                free(result);
                return LeaveException(MathError, _Tree->value, _Tree->file);
            }
        }

        dx = atof(sx);
        dy = atof(sy);

        if (strchr("/%", *(_Tree->value)) != NULL && !dy)
            return LeaveException(DivideByZero, _Tree->value, _Tree->file);

        if (!strcasecmp(_Tree->value, "equ"))
            snprintf(result, 2, "%d", dx == dy);

        else if (!strcasecmp(_Tree->value, "neq"))
            snprintf(result, 2, "%d", dx != dy);

        else if (!strcasecmp(_Tree->value, "and"))
            snprintf(result, 2, "%d", dx && dy);

        else if (!strcasecmp(_Tree->value, "or"))
            snprintf(result, 2, "%d", dx || dy);

        else
        {
            free(result);

            switch (*(_Tree->value))
            {
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
                return LeaveException(MathError, _Tree->value, _Tree->file);
            }
        }
        return result;

    default:
        LeaveException(MathError, _Tree->value, _Tree->file);
        return NULL;
    }

    return NULL;
}
