#ifndef __VIDEO_H__
#define __VIDEO_H__ 0x1

/**
 *
 *  _____  _____ _____ _____ _   _ __  __
 * | ___ \|  _  | ___ \_   _| | | |  \/  |
 * | |_/ /| | | | |_/ / | | | | | | .  . |
 * | ___ \| | | |    /  | | | | | | |\/| |
 * | |_/ /\ \_/ / |\ \ _| |_| |_| | |  | |
 * \____/  \___/\_| \_|\___/ \___/\_|  |_/
 *
 * Antoine LANDRIEUX (MIT License) <video.h>
 * <https://github.com/AntoineLandrieux/BORIUM/>
 *
 */

#define VGA_ADDRESS 0xB8000

#define SCREEN_HEIGHT 25
#define SCREEN_WIDTH 80

#define SCREEN (SCREEN_HEIGHT * SCREEN_WIDTH)

/**
 * @brief Writes single character to stream output at current position
 * @author Antoine LANDRIEUX
 *
 * @param character
 * @param color
 */
void CPUTC(const char character, const unsigned char color);

/**
 * @brief Writes strings to stream output at current position
 * @author Antoine LANDRIEUX
 *
 * @param string
 * @param color
 */
void CPUTS(const char *string, const unsigned char color);

/**
 * @brief Writes single character to stream output at current position
 * @author Antoine LANDRIEUX
 *
 * @param character
 */
void PUTC(const char character);

/**
 * @brief Writes strings to stream output at current position
 * @author Antoine LANDRIEUX
 *
 * @param string
 */
void PUTS(const char *string);

/**
 * @brief Screen scroll
 * @author Antoine LANDRIEUX
 *
 */
void SCREEN_SCROLL();

/**
 * @brief Clear screen
 * @author Antoine LANDRIEUX
 *
 */
void SCREEN_CLEAR();

#endif /* __VIDEO_H__ */