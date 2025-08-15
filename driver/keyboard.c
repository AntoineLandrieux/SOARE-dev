#include <DRIVER/keyboard.h>
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
 * Antoine LANDRIEUX (MIT License) <keyboard.c>
 * <https://github.com/AntoineLandrieux/BORIUM/>
 *
 */

static KEYBOARD_INPUT keyboard = QUERTY;

static const char KEYBOARDS[][58] = {
    /* QWERTY */
    {
        //
        0, '`', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '=', 0,
        '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '{', '}', '\n',
        0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':', '"', 0,
        0, 0, 'z', 'x', 'c', 'v', 'b', 'n', 'm', '<', '>', '?', 0, 0, 0, ' '
        //
    },
    /* AZERTY */
    {
        //
        0, 0, '&', '<', '"', '\'', '(', '-', '>', '_', 0, '@', 0, '=', 0,
        '\t', 'a', 'z', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '^', '$', '\n',
        0, 'q', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', 0, 0,
        0, '*', 'w', 'x', 'c', 'v', 'b', 'n', ',', ';', ':', '!', 0, 0, 0, ' '
        //
    }
    //
};

static const char SHIFTED_KEYBOARDS[][58] = {
    /* QWERTY */
    {
        //
        0, '~', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '+', 0,
        '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n',
        0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', 0,
        0, 0, 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 0, 0, 0, ' '
        //
    },
    /* AZERTY */
    {
        //
        0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 0, '+', 0,
        '\t', 'A', 'Z', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 0, '*', '\n',
        0, 'Q', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M', '%', 0,
        0, 0, 'W', 'X', 'C', 'V', 'B', 'N', '?', '.', '/', 0, 0, 0, 0, ' '
        //
    }
    //
};

void KEYBOARD_INIT(KEYBOARD_INPUT _Keyboard)
{
    keyboard = _Keyboard;
}

void sleep(unsigned long long time)
{
    while (1)
    {
        __asm__ __volatile__("nop");
        time--;
        if (time <= 0)
            break;
    }
}

unsigned char inb(unsigned short port)
{
    unsigned char data;
    __asm__ __volatile__("inb %1, %0" : "=a"(data) : "Nd"(port));
    return data;
}

char ascii_char(unsigned char keycode, unsigned char shift_pressed)
{
    if (keycode >= sizeof(KEYBOARDS[keyboard]))
        return 0;

    if (shift_pressed)
        return SHIFTED_KEYBOARDS[keyboard][keycode];
    else
        return KEYBOARDS[keyboard][keycode];
}

char GETC()
{
    char c = 0;
    unsigned char shift_pressed = 0;

    while (!c)
    {
        unsigned char keycode = inb(KEYBOARD_PORT);

        if (keycode == 0x2A || keycode == 0x36)
            shift_pressed = 1;
        else if (keycode == 0xAA || keycode == 0xB6)
            shift_pressed = 0;
        else
            c = ascii_char(keycode, shift_pressed);
    }

    return c;
}

void GETS(char *dest, unsigned int size)
{
    if (size <= 0)
        return;

    unsigned char shift_pressed = 0;
    unsigned char keycode = 0;
    unsigned int n = 0;
    char c = 0;

    while (1)
    {
        sleep(0x5FFFFFF);
        keycode = inb(KEYBOARD_PORT);

        if (keycode == KEY_BACKSPACE && n > 0)
        {
            n--;
            dest[n] = 0;
            PUTC('\b');
        }

        if (keycode == 0x2A || keycode == 0x36)
            shift_pressed = 1;
        else if (keycode == 0xAA || keycode == 0xB6)
            shift_pressed = 0;
        else
            c = ascii_char(keycode, shift_pressed);

        if (!c || n > size)
            continue;

        if (c == '\n')
            break;

        dest[n] = c;
        PUTC(c);
        n++;
    }

    dest[n] = 0;
    PUTC(c);
}
