#include <DRIVER/video.h>

#include <STD/stdlib.h>

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
static unsigned char enable = 1;

/* Error level */
static char errorlevel = 0;

/* Exceptions */
static char *Exceptions[] = {

    "InterpreterError",
    "FileError",
    "CharacterError",
    "SyntaxError",
    "ValueError",
    "UnexpectedNear",
    "UndefinedReference",
    "ObjectIsNotCallable",
    "VariableDefinedAsFunction",
    "MathError",
    "InvalidEscapeSequence",
    "IndexOutOfRange",
    "DivideByZero",
    "RaiseException"

};

/**
 * @brief Error display
 *
 * @return unsigned char
 */
unsigned char AsIgnoredException(void)
{
    return !enable;
}

/**
 * @brief Enable/disable error display
 *
 * @param ignore
 */
void IgnoreException(unsigned char ignore)
{
    enable = !ignore;
}

/**
 * @brief Clears errors
 *
 */
void ClearException(void)
{
    errorlevel = 0;
}

/**
 * @brief Returns the error level
 *
 * @return char
 */
char ErrorLevel(void)
{
    return errorlevel;
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
    errorlevel = 1;

    // Store the error in the `__ERROR__` variable
    MemSet(MemGet(MEMORY, "__ERROR__"), Exceptions[error]);

    return NULL;
}
