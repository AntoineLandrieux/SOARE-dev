#include <DRIVER/video.h>
#include <DRIVER/memory.h>

/**
 *  _____  _____  ___  ______ _____
 * /  ___||  _  |/ _ \ | ___ \  ___|
 * \ `--. | | | / /_\ \| |_/ / |__
 *  `--. \| | | |  _  ||    /|  __|
 * /\__/ /\ \_/ / | | || |\ \| |___
 * \____/  \___/\_| |_/\_| \_\____/
 *
 * Antoine LANDRIEUX (MIT License) <Error.c>
 * <https://github.com/AntoineLandrieux/SOARE/>
 *
 */

#include <SOARE/SOARE.h>

/* Enable/disable error display */
static u8 enable = 1;

/* Error level */
static i8 ErrorLvl = 0;

/* Exceptions */
static char *Exceptions[] = {

    "InterpreterError",
    "FileError",
    "CharacterError",
    "SyntaxError",
    "UnexpectedNear",
    "UndefinedReference",
    "MathError",
    "IndexOutOfRange",
    "DivideByZero",
    "RaiseException"

};

/**
 * @brief Error display
 * @author Antoine LANDRIEUX
 *
 */
u8 AsIgnoredException()
{
    return !enable;
}

/**
 * @brief Enable/disable error display
 * @author Antoine LANDRIEUX
 *
 * @param ignore
 */
void IgnoreException(u8 ignore)
{
    enable = !ignore;
}

/**
 * @brief Clears errors
 * @author Antoine LANDRIEUX
 *
 */
void ClearException(void)
{
    ErrorLvl = 0;
}

/**
 * @brief Returns the error level
 * @author Antoine LANDRIEUX
 *
 * @return char
 */
char ErrorLevel(void)
{
    return ErrorLvl;
}

/**
 * @brief Create a new error, and display it
 * @author Antoine LANDRIEUX
 *
 * @param error
 * @param string
 * @param file
 * @return void* (always returns NULL)
 */
void *LeaveException(SoareExceptions error, char *string)
{
    // If the errors are disabled, nothing is displayed
    if (enable)
    {
        CPUTS("Except: ", 0xC);
        CPUTS(Exceptions[error], 0xC);
        CPUTS("\n        \"", 0xC);
        CPUTS(string, 0xC);
        CPUTS("\"\n         ^~~~\n", 0xC);
    }
    // set error at level 1
    ErrorLvl = 1;
    return NULL;
}
