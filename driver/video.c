#include <DRIVER/video.h>

/**
 *
 *  _____  _____ _____ _____ _   _ __  __
 * | ___ \|  _  | ___ \_   _| | | |  \/  |
 * | |_/ /| | | | |_/ / | | | | | | .  . |
 * | ___ \| | | |    /  | | | | | | |\/| |
 * | |_/ /\ \_/ / |\ \ _| |_| |_| | |  | |
 * \____/  \___/\_| \_|\___/ \___/\_|  |_/
 *
 * Antoine LANDRIEUX (MIT License) <video.c>
 * <https://github.com/AntoineLandrieux/BORIUM/>
 *
 */

// Video memory address pointer
static unsigned short VGA_POINTER = 0;

/**
 * @brief Writes single character to stream output at current position (colored)
 * 
 * @param character 
 * @param color 
 */
void CPUTC(const char character, const unsigned char color)
{
    char *VIDEO = (char *)VGA_ADDRESS;

    if (VGA_POINTER >= (SCREEN * 2))
        SCREEN_SCROLL();

    switch (character)
    {
    case '\a':
        SCREEN_CLEAR();
        break;

    case '\b':
        VGA_POINTER -= 2;
        VIDEO[VGA_POINTER] = ' ';
        break;

    case '\n':
        VGA_POINTER += (SCREEN_WIDTH * 2);
    case '\r':
        VGA_POINTER -= (VGA_POINTER % (SCREEN_WIDTH * 2));
    case '\0':
        break;

    default:
        VIDEO[(volatile unsigned short)VGA_POINTER++] = character;
        VIDEO[(volatile unsigned short)VGA_POINTER++] = color;
        break;
    }
}

/**
 * @brief Writes strings to stream output at current position (colored)
 * 
 * @param string 
 * @param color 
 */
void CPUTS(const char *string, const unsigned char color)
{
    for (; *string; string++)
        CPUTC(*string, color);
}

/**
 * @brief Writes single character to stream output at current position
 * 
 * @param character 
 */
void PUTC(const char character)
{
    CPUTC(character, 0xF);
}

/**
 * @brief Writes strings to stream output at current position
 * 
 * @param string 
 */
void PUTS(const char *string)
{
    if (!string)
        return PUTS("(null)");
    CPUTS(string, 0xF);
}

/**
 * @brief Scroll the screen
 * 
 */
void SCREEN_SCROLL()
{
    char *VIDEO = (char *)VGA_ADDRESS;
    VGA_POINTER = ((SCREEN * 2) - (SCREEN_WIDTH * 2));

    for (unsigned short i = 0; i < VGA_POINTER; i++)
    {
        VIDEO[i] = VIDEO[i + (SCREEN_WIDTH * 2)];
        VIDEO[i + (SCREEN_WIDTH * 2)] = 0;
    }
}

/**
 * @brief Clear screen
 * 
 */
void SCREEN_CLEAR()
{
    char *VIDEO = (char *)VGA_ADDRESS;
    VGA_POINTER = 0;

    for (unsigned short i = 0; i < (SCREEN * 2); i++)
        VIDEO[i] = 0;
}
