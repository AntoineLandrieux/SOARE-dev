#ifndef __SOARE_RUNTIME_H__
#define __SOARE_RUNTIME_H__ 0x1

/* #prama once */

/**
 *  _____  _____  ___  ______ _____
 * /  ___||  _  |/ _ \ | ___ \  ___|
 * \ `--. | | | / /_\ \| |_/ / |__
 *  `--. \| | | |  _  ||    /|  __|
 * /\__/ /\ \_/ / | | || |\ \| |___
 * \____/  \___/\_| |_/\_| \_\____/
 *
 * Antoine LANDRIEUX (WTFPL) <runtime.h>
 * <https://github.com/AntoineLandrieux/SOARE/>
 *
 * [!] Contribute and help me translate the comments!
 *
 */

// Mémoire utilisé par l'interpreteur
extern MEM *MEMORY;

/**
 * @brief Execute le code à partir d'un arbre et retourne une valeur en fonction du type
 * @author Antoine LANDRIEUX
 * 
 * @param _Type 
 * @param _Tree 
 * @return char* 
 */
char *Runtime(char *_Type, AST *_Tree);

/**
 * @brief Execute le code à partir d'une chaîne de caractères
 * @author Antoine LANDRIEUX
 * 
 * @param _RawCode 
 */
int Execute(char *_File, char *_RawCode);

#endif /* __SOARE_RUNTIME_H__ */