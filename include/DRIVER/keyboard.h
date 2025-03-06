#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__ 0x1

/**
 *
 *  _____  _____ _____ _____ _   _ __  __
 * | ___ \|  _  | ___ \_   _| | | |  \/  |
 * | |_/ /| | | | |_/ / | | | | | | .  . |
 * | ___ \| | | |    /  | | | | | | |\/| |
 * | |_/ /\ \_/ / |\ \ _| |_| |_| | |  | |
 * \____/  \___/\_| \_|\___/ \___/\_|  |_/
 *
 * Antoine LANDRIEUX (MIT License) <keyboard.h>
 * <https://github.com/AntoineLandrieux/BORIUM/>
 *
 */

#define KEYBOARD_PORT 0x60
#define KEY_BACKSPACE 0x0E

typedef enum keyboard_input
{

    QUERTY,
    AZERTY

} KEYBOARD_INPUT;

/**
 * @brief Initialize keyboard
 * @author Antoine LANDRIEUX
 *
 * @param keyboard
 */
void KEYBOARD_INIT(KEYBOARD_INPUT keyboard);

/**
 * @brief inb function
 * @author Antoine LANDRIEUX
 *
 * @param port
 * @return unsigned char
 */
unsigned char inb(unsigned short port);

/**
 * @brief Wait a moment
 * @author Antoine LANDRIEUX
 *
 * @param time
 */
void sleep(unsigned long long time);

/**
 * @brief Convert inb to ascii char (QUERTY mode)
 * @author Antoine LANDRIEUX
 *
 * @param keycode
 * @return char
 */
char ascii_char(unsigned char keycode, unsigned char shift_pressed);

/**
 * @brief Get char
 * @author Antoine LANDRIEUX
 *
 * @return char
 */
char GETC();

/**
 * @brief Get string
 * @author Antoine LANDRIEUX
 *
 * @param dest
 * @param size
 */
void GETS(char *dest, unsigned int size);

#endif /* __KEYBOARD_H__ */