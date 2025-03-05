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
        0, '~', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '+', 0,
        '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '{', '}', '\n',
        0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':', '"', 0,
        0, 0, 'z', 'x', 'c', 'v', 'b', 'n', 'm', '<', '>', '?', 0, 0, 0, ' '
        //
    },
    /* AZERTY */
    {
        //
        0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 0, '+', 0,
        '\t', 'a', 'z', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '=', '$', '\n',
        0, 'q', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', '"', 0,
        0, '*', 'w', 'x', 'c', 'v', 'b', 'n', ',', ';', ':', '!', 0, 0, 0, ' '
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

char ascii_char(unsigned char keycode)
{
    if (keycode < sizeof(KEYBOARDS[keyboard]))
        return KEYBOARDS[keyboard][keycode];
    return 0;
}

char GETC()
{
    char c = 0;
    while (!c)
        c = ascii_char(inb(KEYBOARD_PORT));
    return c;
}

void GETS(char *dest, unsigned int size)
{
    if (size <= 0)
        return;

    unsigned int n = 0;
    char c = 0;

    while (1)
    {
        sleep(0xBFFFFFF);
        c = inb(KEYBOARD_PORT);

        if (c == KEY_BACKSPACE && n > 0)
        {
            n--;
            dest[n] = ' ';
            CURSOR_MOVE_LEFT(1);
            PUTC(' ');
            CURSOR_MOVE_LEFT(1);
        }

        c = ascii_char(c);

        if (!c || n > size)
            continue;

        if (c == '\n')
            break;

        dest[n] = c;
        PUTC(c);
        n++;
    }

    PUTC(c);
}
