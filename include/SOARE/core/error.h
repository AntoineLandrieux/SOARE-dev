#ifndef __SOARE_ERROR_H__
#define __SOARE_ERROR_H__ 0x1

/* #prama once */

/**
 *  _____  _____  ___  ______ _____
 * /  ___||  _  |/ _ \ | ___ \  ___|
 * \ `--. | | | / /_\ \| |_/ / |__
 *  `--. \| | | |  _  ||    /|  __|
 * /\__/ /\ \_/ / | | || |\ \| |___
 * \____/  \___/\_| |_/\_| \_\____/
 *
 * Antoine LANDRIEUX (MIT License) <error.h>
 * <https://github.com/AntoineLandrieux/SOARE/>
 *
 */

/* Character Error */
#define CharacterError "CharacterError"

/* Divide By Zero */
#define DivideByZero "DivideByZero"

/* Interpreter Error (memory issues) */
#define InterpreterError "InterpreterError"

/* Syntax Error */
#define SyntaxError "SyntaxError"

/* Math Error */
#define MathError "MathError"

/* Unexpected Near */
#define UnexpectedNear "UnexpectedNear"

/* Undefined Reference */
#define UndefinedReference "UndefinedReference"

/* Missing Assignation */
#define MissingAssignation "MissingAssignation"

/* File Error */
#define FileError "FileError"

/**
 * @brief Enable/disable error display
 * @author Antoine LANDRIEUX
 *
 * @param _Ignore 0 si non
 */
void IgnoreException(unsigned char _Ignore);

/**
 * @brief Clears errors
 * @author Antoine LANDRIEUX
 *
 */
void ClearException();

/**
 * @brief Returns the error level
 * @author Antoine LANDRIEUX
 *
 * @return char
 */
char ErrorLevel();

/**
 * @brief Create a new error, and display it
 * @author Antoine LANDRIEUX
 *
 * @param _Error
 * @param _String
 * @param _File
 * @return void* (always returns NULL)
 */
void *LeaveException(char *_Error, char *_String, Document _File);

#endif /* __SOARE_ERROR_H__ */