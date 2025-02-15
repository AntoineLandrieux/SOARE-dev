#include <stdio.h>
#include <stdlib.h>

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
#include <SOARE/utils/int.h>

/* Enable/disable error display */
static u8 Exception = 1;

/* Error level */
static i8 ErrorLvl = 0;

/**
 * @brief Enable/disable error display
 * @author Antoine LANDRIEUX
 *
 * @param _Ignore 0 si non
 */
void IgnoreException(unsigned char _Ignore)
{
    Exception = !_Ignore;
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
 * @param _Error
 * @param _String
 * @param _File
 * @return void* (always returns NULL)
 */
void *LeaveException(char *_Error, char *_String, Document _File)
{
    // If the errors are disabled, nothing is displayed
    if (Exception)
    {
#ifndef __SOARE_NO_COLORED_OUTPUT
        // Red
        fprintf(stderr, "\033[31m");
#endif /* __SOARE_NO_COLORED_OUTPUT */
        fprintf(
            //
            stderr,
            "Except: %s\n\t\"%.10s\"\n\t ^~~~\n\tAt file %s:%lld:%lld\n",
            _Error,
            _String,
            _File.file,
            _File.ln,
            _File.col
            //
        );
#ifndef __SOARE_NO_COLORED_OUTPUT
        // Normal
        fprintf(stderr, "\033[0;39m");
#endif /* __SOARE_NO_COLORED_OUTPUT */
    }
    // We put the error at level 1
    ErrorLvl = 1;
    return NULL;
}
