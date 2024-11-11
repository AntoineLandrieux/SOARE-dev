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
static u8 Exception = 1;

/*  */
static i8 ErrorLvl = 0;

/**
 * @brief
 * @author Antoine LANDRIEUX
 *
 * @param _Ignore
 */
void IgnoreException(unsigned char _Ignore)
{
    Exception = !_Ignore;
}

/**
 * @brief 
 * 
 */
void ClearException()
{
    ErrorLvl = 0;
}

/**
 * @brief 
 * 
 * @return char 
 */
char ErrorLevel()
{
    return ErrorLvl;
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
void *LeaveException(char *_Error, char *_String, Document _File)
{
    if (Exception)
        fprintf(
            //
            stderr,
            "Except: %s\n\t\"%.8s...\"\n\t ^~~~\n\tAt file %s:%lld:%lld",
            _Error,
            _String,
            _File.file,
            _File.ln,
            _File.col
            //
        );
    ErrorLvl = 1;
    return NULL;
}
