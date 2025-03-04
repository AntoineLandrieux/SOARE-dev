#include <DRIVER/keyboard.h>
#include <DRIVER/video.h>

#include "kernel.h"

/*
 * kernel.c

 * Antoine LANDRIEUX
 * BORIUM <https://github.com/AntoineLandrieux/BORIUM>
 *
 * MIT License
*/

unsigned char _Running = 0;

static void KEYBOARD_SELECTOR()
{
    PUTS("\n KEYBOARD\n\n ");
    CPUTS("\t1) QWERTY\t2) AZERTY\t\n", 0xF0);
    CPUTS("\n Tap the keyboard number on your keyboard to activate it\n It doesn't work with numpad!\n", 0xC);

    while (1)
    {
        switch (GETC())
        {
        case '1':
            return KEYBOARD_INIT(QUERTY);
        
        case '2':
            return KEYBOARD_INIT(AZERTY);
        
        default:
            break;
        }
    }
}

static void setup()
{
    PUTS("\n\tBORIUM [Antoine LANDRIEUX MIT license]\n");
    KEYBOARD_SELECTOR();
    while (1) {;}
}

void start()
{
    SCREEN_CLEAR();
    _Running = 1;

    setup();
}
