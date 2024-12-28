#ifndef __SOARE_RUNTIME_H__
#define __SOARE_RUNTIME_H__ 0x1

/**
 *  _____  _____  ___  ______ _____
 * /  ___||  _  |/ _ \ | ___ \  ___|
 * \ `--. | | | / /_\ \| |_/ / |__ 
 *  `--. \| | | |  _  ||    /|  __|
 * /\__/ /\ \_/ / | | || |\ \| |___
 * \____/  \___/\_| |_/\_| \_\____/
 */

/**
 * @brief 
 * 
 * @param _Tree 
 * @return char* 
 */
char *Runtime(AST *_Tree);

/**
 * @brief 
 * 
 * @param _RawCode 
 */
int Execute(char *_RawCode);

#endif /* __SOARE_RUNTIME_H__ */