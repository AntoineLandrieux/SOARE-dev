#ifndef __MEMORY_H__
#define __MEMORY_H__ 0x1

/**
 *
 *  _____  _____ _____ _____ _   _ __  __
 * | ___ \|  _  | ___ \_   _| | | |  \/  |
 * | |_/ /| | | | |_/ / | | | | | | .  . |
 * | ___ \| | | |    /  | | | | | | |\/| |
 * | |_/ /\ \_/ / |\ \ _| |_| |_| | |  | |
 * \____/  \___/\_| \_|\___/ \___/\_|  |_/
 *
 * Antoine LANDRIEUX (MIT License) <memory.h>
 * <https://github.com/AntoineLandrieux/BORIUM/>
 *
 */

#define __MEMORY_POOL_SIZE__ 0x2000
#define NULL (void*)(0x0)

/**
 * @brief Memory allocation
 * 
 * @param size 
 * @return void* 
 */
void *malloc(unsigned long long size);

unsigned char strcmp(char *str1, char *str2);

unsigned char strchr(char *string, char character);

long long strlen(const char *string);

long long int atoll(const char *string);

char *lltoa(char *buff, int size, long long value);

#endif /* __MEMORY_H__ */