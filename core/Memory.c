#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

/**
 *  _____  _____  ___  ______ _____
 * /  ___||  _  |/ _ \ | ___ \  ___|
 * \ `--. | | | / /_\ \| |_/ / |__
 *  `--. \| | | |  _  ||    /|  __|
 * /\__/ /\ \_/ / | | || |\ \| |___
 * \____/  \___/\_| |_/\_| \_\____/
 *
 * Antoine LANDRIEUX (MIT License) <Memory.c>
 * <https://github.com/AntoineLandrieux/SOARE/>
 *
 * [!] Contribute and help me translate the comments!
 *
 */

#include <SOARE/SOARE.h>
#include <SOARE/utils/int.h>
#include <SOARE/utils/keywords.h>

/**
 * @brief Créer une nouvelle mémoire vide
 * @author Antoine LANDRIEUX
 *
 * @return MEM*
 */
MEM *Mem()
{
    MEM *MEMORY = (MEM *)malloc(sizeof(MEM));

    if (MEMORY == NULL)
        return LeaveException(InterpreterError, "OUT OF MEMORY", EmptyDocument());

    MEMORY->name = NULL;
    MEMORY->type = NULL;
    MEMORY->next = NULL;
    MEMORY->value = NULL;

    return MEMORY;
}

/**
 * @brief Donne la dernière variable de la mémoire
 * @author Antoine LANDRIEUX
 *
 * @param _Memory
 * @return MEM*
 */
MEM *MemLast(MEM *_Memory)
{
    if (_Memory == NULL)
        return NULL;
    MEM *curr = _Memory;
    for (; curr->next != NULL; curr = curr->next)
        ;
    return curr;
}

/**
 * @brief Ajoute une variable dans une mémoire existante
 * @author Antoine LANDRIEUX
 *
 * @param _Memory
 * @param _Name
 * @param _Type
 * @param ...
 */
MEM *MemPush(MEM *_Memory, char *_Name, char *_Type, char *_Value)
{
    if (_Memory == NULL)
        return NULL;

    MEM *mem = MemLast(_Memory);
    mem->next = (MEM *)malloc(sizeof(MEM));
    mem = mem->next;

    if (mem == NULL)
        return LeaveException(InterpreterError, "OUT OF MEMORY", EmptyDocument());

    mem->next = NULL;
    mem->name = _Name;
    mem->type = _Type;
    mem->value = _Value;

    return mem;
}

/**
 * @brief Modifie une variable dans une mémoire existante
 * @author Antoine LANDRIEUX
 *
 * @param _Memory
 * @param _Type
 */
MEM *MemSet(MEM *_Memory, char *_Value)
{
    if (_Memory == NULL)
        return NULL;
    free(_Memory->value);
    _Memory->value = _Value;
    return _Memory;
}

/**
 * @brief Trouve une varible dans la mémoire
 * @author Antoine LANDRIEUX
 *
 * @param _Memory
 * @param _Name
 * @return MEM*
 */
MEM *MemGet(MEM *_Memory, char *_Name)
{
    if (_Memory == NULL)
        return NULL;
    MEM *get = MemGet(_Memory->next, _Name);
    if (get == NULL && _Memory->value != NULL)
        if (!strcmp(_Memory->name, _Name))
            return _Memory;
    return get;
}

/**
 * @brief Affiche l'essemble des variables de la mémoire
 * @author Antoine LANDRIEUX
 *
 * @param _Memory
 */
void MemLog(MEM *_Memory)
{
    if (_Memory == NULL)
        return;
    printf(
        "[MEMORY] [%p, %s:%s\t%s]\n",
        (void *)_Memory,
        _Memory->name,
        _Memory->type,
        _Memory->value);
    MemLog(_Memory->next);
}

/**
 * @brief Lie 2 mémoires
 * @author Antoine LANDRIEUX
 *
 * @param _To
 * @param _From
 */
void MemJoin(MEM *_To, MEM *_From)
{
    if (_To == NULL || _From == NULL)
        return;
    MemLast(_To)->next = _From;
}

/**
 * @brief Libére la mémoire allouée
 * @author Antoine LANDRIEUX
 *
 * @param _Memory
 * @return void* (retourne toujours NULL)
 */
void *MemFree(MEM *_Memory)
{
    if (_Memory == NULL)
        return NULL;

    MemFree(_Memory->next);
    free(_Memory->value);
    free(_Memory);
    return NULL;
}
