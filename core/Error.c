#include <stdio.h>
#include <stdlib.h>

/**
 *
 */

#include <SOARE/SOARE.h>

static const char *Exceptions[] = {
    //
    "InterpreterError",
    "CharacterError"
    //
};

//
static unsigned char Exception = 1;

/**
 * @brief 
 * 
 * @param _Ignore 
 */
void IgnoreException(unsigned char _Ignore)
{
    Exception = _Ignore;
}

/**
 * @brief
 *
 * @param _Error
 * @param _String
 * @param _File
 * @param _Ln
 * @param _Col
 * @return void*
 */
void *LeaveException(soare_error _Error, char *_String, char *_File, unsigned long _Ln, unsigned long _Col)
{
    if (Exception)
        fprintf(stderr, "Except: %s\n\t\"%.5s...\"\n\t ^~~~\n\tAt file %s:%ld:%ld", Exceptions[_Error], _String, _File, _Ln, _Col);
    return NULL;
}
