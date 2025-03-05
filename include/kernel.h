#ifndef __KERNEL_H__
#define __KERNEL_H__ 0x1

/**
 *
 *  _____  _____ _____ _____ _   _ __  __
 * | ___ \|  _  | ___ \_   _| | | |  \/  |
 * | |_/ /| | | | |_/ / | | | | | | .  . |
 * | ___ \| | | |    /  | | | | | | |\/| |
 * | |_/ /\ \_/ / |\ \ _| |_| |_| | |  | |
 * \____/  \___/\_| \_|\___/ \___/\_|  |_/
 *
 * Antoine LANDRIEUX (MIT License) <kernel.h>
 * <https://github.com/AntoineLandrieux/BORIUM/>
 *
 */

#define BORIUM_KERNEL_MAJOR 1
#define BORIUM_KERNEL_MINOR 0
#define BORIUM_KERNEL_PATCH 0

extern unsigned char running;

/**
 * @brief Start kernel
 * @author Antoine LANDRIEUX
 * 
 */
void start();

#endif /* __KERNEL_H__ */