#ifndef __ERROR_H__
#define __ERROR_H__ 0x1

/**
 *  _____  _____  ___  ______ _____
 * /  ___||  _  |/ _ \ | ___ \  ___|
 * \ `--. | | | / /_\ \| |_/ / |__ 
 *  `--. \| | | |  _  ||    /|  __|
 * /\__/ /\ \_/ / | | || |\ \| |___
 * \____/  \___/\_| |_/\_| \_\____/
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
 * @return void*
 */
void *LeaveException(soare_error _Error, char *_String, Document _File);

#endif /* __ERROR_H__ */