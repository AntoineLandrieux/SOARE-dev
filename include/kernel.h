#ifndef __KERNEL_H__
#define __KERNEL_H__ 0x1

/*
 * kernel.h

 * Antoine LANDRIEUX
 * BORIUM <https://github.com/AntoineLandrieux/BORIUM>
 *
 * MIT License
*/

#define BORIUM_KERNEL_MAJOR 1
#define BORIUM_KERNEL_MINOR 0
#define BORIUM_KERNEL_PATCH 0

extern unsigned char _Running;

/**
 * @brief Start kernel
 * @author Antoine LANDRIEUX
 * 
 */
void start();

#endif /* __KERNEL_H__ */