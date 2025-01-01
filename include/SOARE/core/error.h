#ifndef __SOARE_ERROR_H__
#define __SOARE_ERROR_H__ 0x1

/**
 *  _____  _____  ___  ______ _____
 * /  ___||  _  |/ _ \ | ___ \  ___|
 * \ `--. | | | / /_\ \| |_/ / |__ 
 *  `--. \| | | |  _  ||    /|  __|
 * /\__/ /\ \_/ / | | || |\ \| |___
 * \____/  \___/\_| |_/\_| \_\____/
 */

/* */
#define CharacterError "CharacterError"

/* */
#define DivideByZero "DivideByZero"

/* */
#define InterpreterError "InterpreterError"

/* */
#define SyntaxError "SyntaxError"

/* */
#define TypeError "TypeError"

/* */
#define MathError "MathError"

/* */
#define UnexpectedNear "UnexpectedNear"

/* */
#define UndefinedReference "UndefinedReference"

/**
 * @brief 
 * 
 * @param _Ignore 
 */
void IgnoreException(unsigned char _Ignore);

/**
 * @brief 
 * 
 */
void ClearException();

/**
 * @brief 
 * 
 * @return char 
 */
char ErrorLevel();

/**
 * @brief
 *
 * @param _Error
 * @param _String
 * @param _File
 * @return void*
 */
void *LeaveException(char *_Error, char *_String, Document _File);

#endif /* __SOARE_ERROR_H__ */