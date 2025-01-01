#ifndef __SOARE_MATH_H__
#define __SOARE_MATH_H__ 0x1

/**
 *  _____  _____  ___  ______ _____
 * /  ___||  _  |/ _ \ | ___ \  ___|
 * \ `--. | | | / /_\ \| |_/ / |__ 
 *  `--. \| | | |  _  ||    /|  __|
 * /\__/ /\ \_/ / | | || |\ \| |___
 * \____/  \___/\_| |_/\_| \_\____/
 */

#include "parser.h"

/**
 * @brief 
 * 
 * @param _Tree 
 * @return double 
 */
double MathFloat(AST *_Tree);

/**
 * @brief 
 * 
 * @param _Tree 
 * @return int 
 */
int MathInt(AST *_Tree);

/**
 * @brief 
 * 
 * @param _Tree 
 * @return char* 
 */
char *MathString(AST *_Tree);

/**
 * @brief 
 * 
 * @param _Type 
 * @param _Tree 
 * @return char* 
 */
char *Math(char *_Type, AST *_Tree);

#endif /* __SOARE_MATH_H__ */