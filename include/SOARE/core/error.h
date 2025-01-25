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
 * [!] Contribute and help me translate the comments!
 *
 */

/* Caractère inconnu ou invalide */
#define CharacterError "CharacterError"

/* Division par 0 */
#define DivideByZero "DivideByZero"

/* Erreur de l'interpreteur (allocation mémoire) */
#define InterpreterError "InterpreterError"

/* Erreur de syntaxe */
#define SyntaxError "SyntaxError"

/* Erreur de type */
#define TypeError "TypeError"

/* Erreur mathématique (opération invalide) */
#define MathError "MathError"

/* Symbole inattendu */
#define UnexpectedNear "UnexpectedNear"

/* Symbole inexistant */
#define UndefinedReference "UndefinedReference"

/* Assignation inexistant */
#define MissingAssignation "MissingAssignation"

/* Erreur lors de l'ouverture/lecture/écriture d'un fichier */
#define FileError "FileError"

/**
 * @brief Ignore les erreurs
 * @author Antoine LANDRIEUX
 *
 * @param _Ignore 0 si non
 */
void IgnoreException(unsigned char _Ignore);

/**
 * @brief Efface les erreurs
 * @author Antoine LANDRIEUX
 *
 */
void ClearException();

/**
 * @brief Retourne le niveau d'erreur
 * @author Antoine LANDRIEUX
 *
 * @return char
 */
char ErrorLevel();

/**
 * @brief Créer une nouvelle erreur, et l'affiche
 * @author Antoine LANDRIEUX
 *
 * @param _Error
 * @param _String
 * @param _File
 * @return void* (retourne toujours NULL)
 */
void *LeaveException(char *_Error, char *_String, Document _File);

#endif /* __SOARE_ERROR_H__ */