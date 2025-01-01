#ifndef __SOARE_MEMORY_H__
#define __SOARE_MEMORY_H__ 0x1

/**
 *  _____  _____  ___  ______ _____
 * /  ___||  _  |/ _ \ | ___ \  ___|
 * \ `--. | | | / /_\ \| |_/ / |__
 *  `--. \| | | |  _  ||    /|  __|
 * /\__/ /\ \_/ / | | || |\ \| |___
 * \____/  \___/\_| |_/\_| \_\____/
 */

#include "parser.h"

typedef struct MEM
{

    char *name;
    char *type;

    char *value;

    struct MEM *next;

} MEM;

/**
 * @brief
 *
 * @return MEM*
 */
MEM *Mem();

/**
 * @brief
 *
 * @param _Memory
 * @return MEM*
 */
MEM *MemLast(MEM *_Memory);

/**
 * @brief
 *
 * @param _Memory
 * @param _Name
 * @param _Type
 * @param _Value
 */
void MemPush(MEM *_Memory, char *_Name, char *_Type, char *_Value);

/**
 * @brief
 *
 * @param _Memory
 * @return void*
 */
void *MemFree(MEM *_Memory);

/**
 * @brief
 *
 * @param _Memory
 * @param _Name
 * @return MEM*
 */
MEM *MemGet(MEM *_Memory, char *_Name);

/**
 * @brief
 *
 * @param _Memory
 */
void MemLog(MEM *_Memory);

#endif /* __SOARE_MEMORY_H__ */