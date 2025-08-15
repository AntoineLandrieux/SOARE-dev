#ifndef __STDLIB_H__
#define __STDLIB_H__ 0x1

/**
 *
 *  _____  _____ _____ _____ _   _ __  __
 * | ___ \|  _  | ___ \_   _| | | |  \/  |
 * | |_/ /| | | | |_/ / | | | | | | .  . |
 * | ___ \| | | |    /  | | | | | | |\/| |
 * | |_/ /\ \_/ / |\ \ _| |_| |_| | |  | |
 * \____/  \___/\_| \_|\___/ \___/\_|  |_/
 *
 * Antoine LANDRIEUX (MIT License) <stdlib.h>
 * <https://github.com/AntoineLandrieux/BORIUM/>
 *
 */

#define __MEMORY_POOL_SIZE__ 0x2000

#define NULL (void *)(0x0)

/**
 * @brief Memory allocation
 *
 * @param size
 * @return void*
 */
void *malloc(unsigned long long size);

/**
 * @brief Free allocated memory
 * 
 */
void free();

/**
 * @brief Compare string
 * 
 * @param str1 
 * @param str2 
 * @return unsigned char 
 */
unsigned char strcmp(char *str1, char *str2);

/**
 * @brief Search character from string 
 * 
 * @param string 
 * @param character 
 * @return unsigned char 
 */
unsigned char strchr(char *string, char character);

/**
 * @brief String length
 * 
 * @param string 
 * @return long long 
 */
long long strlen(const char *string);

/**
 * @brief String to int
 * 
 * @param string 
 * @return int 
 */
int atoi(const char *string);

/**
 * @brief Int to string
 *
 * @param buff
 * @param size
 * @param value
 * @return char*
 */
char *itoa(char *buff, int size, int value);

/**
 * @brief Append string
 * 
 * @param dest 
 * @param string 
 */
void strcat(char *dest, char *string);

/**
 * @brief Copy string
 * 
 * @param dest 
 * @param string 
 */
void strcpy(char *dest, char *string);

#endif /* __STDLIB_H__ */