#include <DRIVER/keyboard.h>
#include <DRIVER/video.h>

#include <STD/stdlib.h>

#include <SOARE/SOARE.h>

/**
 *
 *  _____  _____ _____ _____ _   _ __  __
 * | ___ \|  _  | ___ \_   _| | | |  \/  |
 * | |_/ /| | | | |_/ / | | | | | | .  . |
 * | ___ \| | | |    /  | | | | | | |\/| |
 * | |_/ /\ \_/ / |\ \ _| |_| |_| | |  | |
 * \____/  \___/\_| \_|\___/ \___/\_|  |_/
 *
 * Antoine LANDRIEUX (MIT License) <kernel.c>
 * <https://github.com/AntoineLandrieux/BORIUM/>
 *
 */

#include "kernel.h"

// Is the kernel running?
unsigned char running = 0;

/**
 * @brief Select the keyboard layout
 * 
 */
static void KEYBOARD_SELECTOR()
{
    PUTS("\n KEYBOARD\n\n ");
    CPUTS("    1) QWERTY    2) AZERTY    \n", 0xF0);
    CPUTS("\n Tap the keyboard number on your keyboard to activate it\n It doesn't work with numpad!\n", 0x0C);

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

/**
 * @brief Shell for the interpreter
 * 
 */
static void shell()
{
    char input[1024] = {0};

    CPUTS("\nSOARE INTERPRETER\n", 0x09);
    CPUTS("Enter '?run' to run code or '?exit' to quit.\n", 0x07);
    CPUTS("\n[OK] 1024 BYTE INPUT\n\n", 0x0A);

    while (running)
    {
        CPUTS(">>> ", 0x0D);

        char user[__SOARE_MAX_INPUT__] = {0};
        GETS(user, sizeof(user) - 1);

        if (!strcmp(user, "?run"))
        {
            Execute(input);
            free();
        }

        else if (!strcmp(user, "?commit"))
        {

            Execute(input);
            free();
            for (unsigned int i = 0; i < sizeof(input); i++)
                input[i] = 0;
        }

        else if (!strcmp(user, "?clear"))
        {
            PUTC('\a');
        }

        else if (!strcmp(user, "?exit"))
        {
            running = 0;
        }

        else if (strlen(user) + strlen(input) > sizeof(input))
        {
            __SOARE_OUT_OF_MEMORY();

            CPUTS("Clear input ? (Y/N)\n", 0x07);

            switch (GETC())
            {
            case 'Y':
            case 'y':

                free();

                for (unsigned int i = 0; i < sizeof(input); i++)
                    input[i] = 0;

                CPUTS("BUFFER ERASED\n", 0x0A);
                break;

            default:

                CPUTS("CANCELED\n", 0x0C);
                break;
            }
        }

        else
        {
            strcat(input, user);
        }
    }
}

/**
 * @brief Setup the kernel
 * 
 */
static void setup()
{
    PUTS("\n BORIUM [Antoine LANDRIEUX MIT license]\n");
    KEYBOARD_SELECTOR();
}

/**
 * @brief Start the kernel
 * 
 * @return * void 
 */
void start()
{
    running = 1;
    SCREEN_CLEAR();
    setup();
    shell();
}
