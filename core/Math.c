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
 * Antoine LANDRIEUX (WTFPL) <Math.c>
 * <https://github.com/AntoineLandrieux/SOARE/>
 *
 * [!] Contribute and help me translate the comments!
 *
 */

#include <SOARE/SOARE.h>
#include <SOARE/utils/int.h>
#include <SOARE/utils/keywords.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/**
 * @brief Retourne 1 si la chaîne de caractères est un nombre
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
 * @brief Copie une chaîne de caractères
 * @author Antoine LANDRIEUX
 * 
 * @param _Value 
 * @return char* 
 */
static char *vardup(char *_Value)
{
    if (_Value == NULL)
        return NULL;
    char *result = strdup(_Value);
    if (result == NULL)
        return LeaveException(InterpreterError, "OUT OF MEMORY", EmptyDocument());
    return result;
}

/**
 * @brief Copie une chaîne de caractères nombre
 * @author Antoine LANDRIEUX
 * 
 * @param _Value 
 * @return char* 
 */
static char *varintdup(char *_Value)
{
    if (_Value == NULL)
        return NULL;
    size_t size = strlen(_Value) + 1;
    char *result = malloc(size);
    if (result == NULL)
        LeaveException(InterpreterError, "OUT OF MEMORY", EmptyDocument());
    snprintf(result, size, "%.f", atof(_Value));
    return result;
}

/**
 * @brief Évalue l'expression mathématique d'un arbre pour en sortir une valeur en fonction du type
 * @author Antoine LANDRIEUX
 *
 * @param _Type
 * @param _Tree
 * @return char*
 */
char *Math(char *_Type, AST *_Tree)
{
    double dx, dy;
    char *sx, *sy;
    char *result = NULL;

    MEM *get = NULL;

    switch (_Tree->type)
    {
    case NODE_MEMGET:

        get = MemGet(MEMORY, _Tree->value);

        if (get == NULL)
            return LeaveException(UndefinedReference, _Tree->value, _Tree->file);

        if (!strcmp(get->type, TYPE_NUMBER))
            return varintdup(get->value);
        else if (!strcmp(get->type, TYPE_STRING))
            return vardup(get->value);

        break;

    case NODE_NUMBER:

        return vardup(_Tree->value);

    case NODE_STRING:

        if (!strcmp(_Type, TYPE_NUMBER))
            return varintdup(_Tree->value);
        return vardup(_Tree->value);

    case NODE_OPERATOR:

        sx = Math(_Type, _Tree->child);
        sy = Math(_Type, _Tree->child->sibling);

        if (sx == NULL || sy == NULL)
            return NULL;

        result = malloc(50);
        if (result == NULL)
            LeaveException(InterpreterError, "OUT OF MEMORY", EmptyDocument());

        if (isNaN(sx) || isNaN(sy))
        {
            if (!strcasecmp(_Tree->value, "equ"))
                snprintf(result, 50, "%d", !strcmp(sx, sy));

            else if (!strcasecmp(_Tree->value, "neq"))
                snprintf(result, 50, "%d", strcmp(sx, sy));

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
                return LeaveException(TypeError, _Tree->value, _Tree->file);
            }
        }

        dx = atof(Math(_Type, _Tree->child));
        dy = atof(Math(_Type, _Tree->child->sibling));

        if (strchr("/%", *(_Tree->value)) != NULL && !dy)
            return LeaveException(DivideByZero, _Tree->value, _Tree->file);

        if (!strcasecmp(_Tree->value, "equ"))
            snprintf(result, 50, "%d", dx == dy);

        else if (!strcasecmp(_Tree->value, "neq"))
            snprintf(result, 50, "%d", dx != dy);

        else if (!strcasecmp(_Tree->value, "and"))
            snprintf(result, 50, "%d", dx && dy);

        else if (!strcasecmp(_Tree->value, "or"))
            snprintf(result, 50, "%d", dx || dy);

        else
        {
            switch (*(_Tree->value))
            {
            case '*':
                snprintf(result, 50, "%.f", dx * dy);
                break;
            case '^':
                snprintf(result, 50, "%d", (int)dx ^ (int)dy);
                break;
            case '/':
                snprintf(result, 50, "%.f", dx / dy);
                break;
            case '%':
                snprintf(result, 50, "%d", (int)dx % (int)dy);
                break;
            case '+':
                snprintf(result, 50, "%.f", dx + dy);
                break;
            case '-':
                snprintf(result, 50, "%.f", dx - dy);
                break;
            case '<':
                snprintf(result, 50, "%d", dx < dy);
                break;
            case '>':
                snprintf(result, 50, "%d", dx > dy);
                break;
            default:
                free(result);
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
