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

static unsigned short VGA_POINTER = 0;

void CPUTC(const char character, const unsigned char color)
{
    char *VIDEO = (char *)VGA_ADDRESS;

    if (VGA_POINTER >= (SCREEN * 2))
        SCREEN_SCROLL();

    switch (character)
    {
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

void CPUTS(const char *string, const unsigned char color)
{
    for (; *string; (volatile char *)string++)
        CPUTC(*string, color);
}

void PUTC(const char character)
{
    CPUTC(character, 0xF);
}

void PUTS(const char *string)
{
    CPUTS(string, 0xF);
}

void CURSOR_MOVE_LEFT(unsigned int step)
{
    VGA_POINTER -= (step * 2);
}

void CURSOR_MOVE_RIGHT(unsigned int step)
{
    VGA_POINTER += (step * 2);
}

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

void SCREEN_CLEAR()
{
    char *VIDEO = (char *)VGA_ADDRESS;
    VGA_POINTER = 0;

    for (unsigned short i = 0; i < (SCREEN * 2); i++)
        VIDEO[i] = 0;
}
