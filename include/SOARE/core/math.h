#ifndef __SOARE_MATH_H__
#define __SOARE_MATH_H__ 0x1

/* #prama once */

/**
 *  _____  _____  ___  ______ _____
 * /  ___||  _  |/ _ \ | ___ \  ___|
 * \ `--. | | | / /_\ \| |_/ / |__
 *  `--. \| | | |  _  ||    /|  __|
 * /\__/ /\ \_/ / | | || |\ \| |___
 * \____/  \___/\_| |_/\_| \_\____/
 *
 * Antoine LANDRIEUX (MIT License) <math.h>
 * <https://github.com/AntoineLandrieux/SOARE/>
 *
 * [!] Contribute and help me translate the comments!
 *
 */

#include "parser.h"

/**
 * @brief Évalue l'expression mathématique d'un arbre pour en sortir une valeur en fonction du type
 * @author Antoine LANDRIEUX
 *
 * @param _Type
 * @param _Tree
 * @return char*
 */
char *Math(char *_Type, AST *_Tree);

#endif /* __SOARE_MATH_H__ */