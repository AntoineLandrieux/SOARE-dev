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
#include <SOARE/utils/keywords.h>

double MathFloat(AST *_Tree)
{
    double dx;
    double dy;

    MEM *get = NULL;

    switch (_Tree->type)
    {
    case NODE_MEMGET:

        get = MemGet(MEMORY, _Tree->value);
        if (get == NULL)
        {
            LeaveException(UndefinedReference, _Tree->value, _Tree->file);
            return 0;
        }
        return atof(get->value);
    
    case NODE_NUMBER:

        return atoi(_Tree->value);

    case NODE_OPERATOR:

        dx = MathInt(_Tree->child);
        dy = MathInt(_Tree->child->sibling);

        if (strchr("/%", *(_Tree->value)) != NULL && !dy)
        {
            LeaveException(DivideByZero, _Tree->value, _Tree->file);
            return 0;
        }

        if (!strcasecmp(_Tree->value, "equ"))
            return dx == dy;

        if (!strcasecmp(_Tree->value, "neq"))
            return dx != dy;

        if (!strcasecmp(_Tree->value, "and"))
            return dx && dy;

        if (!strcasecmp(_Tree->value, "or"))
            return dx || dy;

        switch (*(_Tree->value))
        {
        case '*':
            return dx * dy;

        case '^':
            return pow(dx, dy);

        case '/':
            return dx / dy;

        case '%':
            return (double)((int)dx % (int)dy);

        case '+':
            return (dx + dy);

        case '-':
            return (dx - dy);

        case '<':
            return dx < dy;

        case '>':
            return dx > dy;

        default:
            break;
        }
        break;

    default:
        break;
    }

    LeaveException(MathError, _Tree->value, _Tree->file);
    return 0;
}

int MathInt(AST *_Tree)
{
    return (int)MathFloat(_Tree);
}

char *MathString(AST *_Tree)
{

    char *tmp;
    char *sx;
    char *sy;

    MEM *get = NULL;

    switch (_Tree->type)
    {
    case NODE_MEMGET:
        get = MemGet(MEMORY, _Tree->value);
        if (get == NULL)
            return LeaveException(UndefinedReference, _Tree->value, _Tree->file);
        return strdup(get->value);

    case NODE_STRING:

        tmp = strdup(_Tree->value);
        if (tmp == NULL)
            return LeaveException(InterpreterError, "OUT OF MEMORY", EmptyDocument());
        return tmp;

    case NODE_OPERATOR:

        sx = MathString(_Tree->child);
        sy = MathString(_Tree->child->sibling);

        if (*(_Tree->value) == '+')
        {
            tmp = malloc(strlen(sx) + strlen(sy) + 1);

            if (tmp == NULL)
                return LeaveException(InterpreterError, "OUT OF MEMORY", EmptyDocument());

            strcat(strcpy(tmp, sx), sy);
            return tmp;
        }

        tmp = malloc(2);

        if (tmp == NULL)
            return LeaveException(InterpreterError, "OUT OF MEMORY", EmptyDocument());

        strcpy(tmp, " ");

        if (!strcasecmp(_Tree->value, "equ"))
            strcpy(tmp, !strcmp(sx, sy) ? "1" : "0");

        else if (!strcasecmp(_Tree->value, "neq"))
            strcpy(tmp, strcmp(sx, sy) ? "1" : "0");

        switch (*(_Tree->value))
        {
        case '<':
            strcpy(tmp, strlen(sx) < strlen(sy) ? "1" : "0");

        case '>':
            strcpy(tmp, strlen(sx) < strlen(sy) ? "1" : "0");
        }

        if (strcmp(tmp, " "))
            return tmp;
        else
            free(tmp);

        break;

    default:
        break;
    }

    return LeaveException(MathError, _Tree->value, _Tree->file);
}

static size_t cdigits(double _Number)
{
    char tmp[50];
    snprintf(tmp, sizeof(tmp), "%.f", _Number);
    return strlen(tmp);
}

char *Math(char *_Type, AST *_Tree)
{
    if (!strcmp(_Type, TYPE_INT))
    {
        i32 i = MathInt(_Tree);
        char *string = malloc(((int)(i / 10)) + 2);

        if (string == NULL)
            return LeaveException(InterpreterError, "OUT OF MEMORY", EmptyDocument());

        itoa(i, string, 10);
        return string;
    }

    else if (!strcmp(_Type, TYPE_FLOAT))
    {
        double d = MathFloat(_Tree);
        size_t digits = cdigits(d);
        char *string = malloc(digits + 1);

        if (string == NULL)
            return LeaveException(InterpreterError, "OUT OF MEMORY", EmptyDocument());

        gcvt(d, digits, string);
        return string;
    }

    return MathString(_Tree);
}
