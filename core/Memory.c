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

MEM *Mem()
{
    MEM *MEMORY = (MEM *)malloc(sizeof(MEM));

    if (MEMORY == NULL)
        return LeaveException(InterpreterError, "OUT OF MEMORY", EmptyDocument());

    MEMORY->name = NULL;
    MEMORY->type = NULL;

    MEMORY->next = NULL;

    return MEMORY;
}

MEM *MemLast(MEM *_Memory)
{
    if (_Memory == NULL)
        return NULL;
    MEM *curr = _Memory;
    for (; curr->next != NULL; curr = curr->next)
        ;
    return curr;
}

void MemPush(MEM *_Memory, char *_Name, char *_Type, char *_Value)
{
    if (_Memory == NULL)
        return;

    MEM *curr = MemLast(_Memory);
    curr->next = (MEM *)malloc(sizeof(MEM));
    curr = curr->next;

    if (curr == NULL)
    {
        LeaveException(InterpreterError, "OUT OF MEMORY", EmptyDocument());
        return;
    }

    curr->name = _Name;
    curr->type = _Type;
    curr->value = _Value;
    curr->next = NULL;
}

void *MemFree(MEM *_Memory)
{
    if (_Memory == NULL)
        return NULL;

    MemFree(_Memory->next);
    free(_Memory);
    return NULL;
}

MEM *MemGet(MEM *_Memory, char *_Name)
{
    for (MEM *curr = _Memory; curr != NULL; curr = curr->next)
        if (curr->name == NULL)
            ;
        else if (!strcmp(curr->name, _Name))
            return curr;
    return NULL;
}

void MemLog(MEM *_Memory)
{
    if (_Memory == NULL)
        return;
    printf(
        "[MEMORY] [%p, %s:%s (AST *)%p]\n",
        (void *)_Memory,
        _Memory->name,
        _Memory->type,
        (void *)_Memory->value);
    MemLog(_Memory->next);
}
