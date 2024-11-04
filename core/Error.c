#include <stdio.h>
#include <stdlib.h>

/**
 *  _____  _____  ___  ______ _____
 * /  ___||  _  |/ _ \ | ___ \  ___|
 * \ `--. | | | / /_\ \| |_/ / |__ 
 *  `--. \| | | |  _  ||    /|  __|
 * /\__/ /\ \_/ / | | || |\ \| |___
 * \____/  \___/\_| |_/\_| \_\____/
 */

#include <SOARE/SOARE.h>
#include <SOARE/utils/int.h>

/*  */
static const char *Exceptions[] = {
    //
    "InterpreterError",
    "CharacterError"
    //
};

/*  */
static u8 Exception = 1;

/**
 * @brief
 * @author Antoine LANDRIEUX
 *
 * @param _Ignore
 */
void IgnoreException(unsigned char _Ignore)
{
    Exception = _Ignore;
}

/**
 * @brief
 * @author Antoine LANDRIEUX
 *
 * @param _Error
 * @param _String
 * @param _File
 * 
 * @return void*
 */
void *LeaveException(soare_error _Error, char *_String, Document _File)
{
    if (Exception)
        fprintf(
            //
            stderr,
            "Except: %s\n\t\"%.5s...\"\n\t ^~~~\n\tAt file %s:%lld:%lld",
            Exceptions[_Error],
            _String,
            _File.file,
            _File.ln,
            _File.col
            //
        );
    return NULL;
}
