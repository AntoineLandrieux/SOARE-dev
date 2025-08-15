#ifndef __STDARG_H__
#define __STDARG_H__ 0x1

/**
 *
 *  _____  _____ _____ _____ _   _ __  __
 * | ___ \|  _  | ___ \_   _| | | |  \/  |
 * | |_/ /| | | | |_/ / | | | | | | .  . |
 * | ___ \| | | |    /  | | | | | | |\/| |
 * | |_/ /\ \_/ / |\ \ _| |_| |_| | |  | |
 * \____/  \___/\_| \_|\___/ \___/\_|  |_/
 *
 * Antoine LANDRIEUX (MIT License) <stdarg.h>
 * <https://github.com/AntoineLandrieux/BORIUM/>
 *
 */

// Variable argument list
typedef char* va_list;

// Variable argument list initialization
#define va_start(va, arg) (va = (va_list)&arg + sizeof(arg))

// Get the next argument from the variable argument list
#define va_arg(va, type) (*(type *)((va += sizeof(type)) - sizeof(type)))

// Clean up the variable argument list
#define va_end(va) (va = NULL)

#endif /* __STDARG_H__ */