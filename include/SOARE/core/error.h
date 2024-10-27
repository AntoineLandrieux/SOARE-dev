#ifndef __ERROR_H__
#define __ERROR_H__ 0x1

/**
 *
 */

/**
 * @brief
 *
 */
typedef enum
{

    ERR_INTERPRETER,
    ERR_CHARACTER,

} soare_error;

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
void *LeaveException(soare_error _Error, char *_String, char *_File, unsigned long _Ln, unsigned long _Col);

#endif /* __ERROR_H__ */