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
 */

#include "parser.h"

/**
 * @brief Evaluates the mathematical expression of a tree
 * @author Antoine LANDRIEUX
 *
 * @param _Tree
 * @return char*
 */
char *Math(AST _Tree);

#endif /* __SOARE_MATH_H__ */