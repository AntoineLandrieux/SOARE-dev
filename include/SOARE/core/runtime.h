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
 * Antoine LANDRIEUX (MIT License) <runtime.h>
 * <https://github.com/AntoineLandrieux/SOARE/>
 *
 */

// Memory used by the interpreter
extern MEM MEMORY;

/**
 * @brief Execute a function
 * @author Antoine LANDRIEUX
 *
 * @deprecated
 *
 * @param _Tree
 * @return char*
 */
char *RunFunction(AST _Tree);

/**
 * @brief Execute le code à partir d'un arbre et retourne une valeur en fonction du type
 * @author Antoine LANDRIEUX
 *
 * @param _Tree
 * @return char*
 */
char *Runtime(AST _Tree);

/**
 * @brief Execute le code à partir d'une chaîne de caractères
 * @author Antoine LANDRIEUX
 *
 * @param _File
 * @param _RawCode
 * @return int
 */
int Execute(char *_File, char *_RawCode);

#endif /* __SOARE_RUNTIME_H__ */