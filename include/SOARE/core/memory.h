#ifndef __SOARE_MEMORY_H__
#define __SOARE_MEMORY_H__ 0x1

/* #prama once */

/**
 *  _____  _____  ___  ______ _____
 * /  ___||  _  |/ _ \ | ___ \  ___|
 * \ `--. | | | / /_\ \| |_/ / |__
 *  `--. \| | | |  _  ||    /|  __|
 * /\__/ /\ \_/ / | | || |\ \| |___
 * \____/  \___/\_| |_/\_| \_\____/
 *
 * Antoine LANDRIEUX (MIT License) <memory.h>
 * <https://github.com/AntoineLandrieux/SOARE/>
 *
 */

/**
 * @brief Structure of memory
 * @author Antoine LANDRIEUX
 */
typedef struct mem
{

    // Name
    char *name;
    // Value
    char *value;

    // Next
    struct mem *next;

} *MEM;

/**
 * @brief Create a new empty memory
 * @author Antoine LANDRIEUX
 *
 * @return MEM
 */
MEM Mem();

/**
 * @brief Give the last variable in the memory
 * @author Antoine LANDRIEUX
 *
 * @param _Memory
 * @return MEM
 */
MEM MemLast(MEM _Memory);

/**
 * @brief Add a variable to an existing memory
 * @author Antoine LANDRIEUX
 *
 * @param _Memory
 * @param _Name
 * @return MEM
 */
MEM MemPush(MEM _Memory, char *_Name, char *_Value);

/**
 * @brief Find a variable in the memory
 * @author Antoine LANDRIEUX
 *
 * @param _Memory
 * @param _Name
 * @return MEM
 */
MEM MemGet(MEM _Memory, char *_Name);

/**
 * @brief Update a variable
 * @author Antoine LANDRIEUX
 *
 * @param _Memory
 * @param _Name
 * @return MEM
 */
MEM MemSet(MEM _Memory, char *_Value);

/**
 * @brief Display all variables
 * @author Antoine LANDRIEUX
 *
 * @param _Memory
 */
void MemLog(MEM _Memory);

/**
 * @brief Join 2 memories
 * @author Antoine LANDRIEUX
 *
 * @param _To
 * @param _From
 */
void MemJoin(MEM _To, MEM _From);

/**
 * @brief Free the allocated memory
 * @author Antoine LANDRIEUX
 *
 * @param _Memory
 * @return void* (always returns NULL)
 */
void *MemFree(MEM _Memory);

#endif /* __SOARE_MEMORY_H__ */