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
 * Antoine LANDRIEUX (WTFPL) <memory.h>
 * <https://github.com/AntoineLandrieux/SOARE/>
 *
 * [!] Contribute and help me translate the comments!
 *
 */

/**
 * @brief Structure de la mémoire
 * @author Antoine LANDRIEUX
 */
typedef struct MEM
{

    // Nom de la variable
    char *name;
    // Type de la variable
    char *type;
    // Valeur de la variable
    char *value;

    // La variable suivante
    struct MEM *next;

} MEM;

/**
 * @brief Créer une nouvelle mémoire vide
 * @author Antoine LANDRIEUX
 *
 * @return MEM*
 */
MEM *Mem();

/**
 * @brief Donne la dernière variable de la mémoire
 * @author Antoine LANDRIEUX
 *
 * @param _Memory
 * @return MEM*
 */
MEM *MemLast(MEM *_Memory);

/**
 * @brief Ajoute une variable dans une mémoire existante
 * @author Antoine LANDRIEUX
 *
 * @param _Memory
 * @param _Name
 * @param _Type
 * @param ...
 */
MEM *MemPush(MEM *_Memory, char *_Name, char *_Type, char *_Value);

/**
 * @brief Modifie une variable dans une mémoire existante
 * @author Antoine LANDRIEUX
 *
 * @param _Memory
 * @param _Name
 * @param _Type
 * @param ...
 */
MEM *MemSet(MEM *_Memory, char *_Value);

/**
 * @brief Libére la mémoire allouée
 * @author Antoine LANDRIEUX
 *
 * @param _Memory
 * @return void* (retourne toujours NULL)
 */
void *MemFree(MEM *_Memory);

/**
 * @brief Trouve une varible dans la mémoire
 * @author Antoine LANDRIEUX
 *
 * @param _Memory
 * @param _Name
 * @return MEM*
 */
MEM *MemGet(MEM *_Memory, char *_Name);

/**
 * @brief Affiche l'essemble des variables de la mémoire
 * @author Antoine LANDRIEUX
 *
 * @param _Memory
 */
void MemLog(MEM *_Memory);

#endif /* __SOARE_MEMORY_H__ */