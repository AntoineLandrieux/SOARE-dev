#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__ 0x1

/*
 * keyboard.h

 * Antoine LANDRIEUX
 * BORIUM <https://github.com/AntoineLandrieux/BORIUM>
 *
 * MIT License
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
 * 
 * @param _Keyboard 
 */
void KEYBOARD_INIT(KEYBOARD_INPUT _Keyboard);

/**
 * @brief inb function
 * @author Antoine LANDRIEUX
 * 
 * @param _Port 
 * @return unsigned char 
 */
unsigned char inb(unsigned short _Port);

/**
 * @brief Wait a moment
 * @author Antoine LANDRIEUX
 * 
 * @param _Timer 
 */
void sleep(unsigned int _Timer);

/**
 * @brief Convert inb to ascii char (QUERTY mode)
 * @author Antoine LANDRIEUX
 * 
 * @param _KeyCode 
 * @return char 
 */
char ascii_char(unsigned char _KeyCode);

/**
 * @brief Get char
 * 
 * @return char 
 */
char GETC();

/**
 * @brief Get string
 * @author Antoine LANDRIEUX
 * 
 * @param _Dest
 * @param _Size
 */
void GETS(char *_Dest, unsigned int _Size);

#endif /* __KEYBOARD_H__ */