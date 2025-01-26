#ifndef __SOARE_TOKENIZER_H__
#define __SOARE_TOKENIZER_H__ 0x1

/* #prama once */

/**
 *  _____  _____  ___  ______ _____
 * /  ___||  _  |/ _ \ | ___ \  ___|
 * \ `--. | | | / /_\ \| |_/ / |__
 *  `--. \| | | |  _  ||    /|  __|
 * /\__/ /\ \_/ / | | || |\ \| |___
 * \____/  \___/\_| |_/\_| \_\____/
 *
 * Antoine LANDRIEUX (MIT License) <tokenizer.h>
 * <https://github.com/AntoineLandrieux/SOARE/>
 *
 * [!] Contribute and help me translate the comments!
 *
 */

/**
 * @brief Enumére les différents types de jetons
 * @author Antoine LANDRIEUX
 */
typedef enum token_type
{

    TKN_EOF,
    TKN_TYPE,
    TKN_NAME,
    TKN_NUMBER,
    TKN_STRING,
    TKN_PARENL,
    TKN_PARENR,
    TKN_ASSIGN,
    TKN_KEYWORD,
    TKN_FUNCTION,
    TKN_OPERATOR,
    TKN_SEMICOLON

} token_type;

/**
 * @brief Structure d'un jeton
 * @author Antoine LANDRIEUX
 */
typedef struct Tokens
{

    // Valeur du jeton
    char *value;
    // Type du jeton
    token_type type;

    // Position du jeton dans le document
    Document file;

    // Jeton suivant
    struct Tokens *next;

} Tokens;

/**
 * @brief Retourne un document vide
 * @author Antoine LANDRIEUX
 *
 * @return Document
 */
Document EmptyDocument();

/**
 * @brief Créer un nouveau jeton
 * @author Antoine LANDRIEUX
 *
 * @param _Filename
 * @param _Value
 * @param _Type
 * @return Tokens*
 */
Tokens *Token(char *_Filename, char *_Value, token_type _Type);

/**
 * @brief Libère la mémoire allouées par les jetons
 * @author Antoine LANDRIEUX
 *
 * @param _Token
 */
void TokensFree(Tokens *_Token);

/**
 * @brief Affiche les jetons
 * @author Antoine LANDRIEUX
 *
 * @param _Token
 */
void TokensLog(Tokens *_Token);

/**
 * @brief Transforme une chaîne de caractère en une séquence de jetons
 * @author Antoine LANDRIEUX
 *
 * @param _Filename
 * @param _Text
 * @return Tokens*
 */
Tokens *Tokenizer(char *_Filename, char *_Text);

#endif /* __SOARE_TOKENIZER_H__ */