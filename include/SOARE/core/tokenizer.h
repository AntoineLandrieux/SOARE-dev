#ifndef __SOARE_TOKENIZER_H__
#define __SOARE_TOKENIZER_H__ 0x1

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
 */

/**
 * @brief List the different types of tokens
 * @author Antoine LANDRIEUX
 */
typedef enum token_type
{

    TKN_EOF,
    TKN_NAME,
    TKN_NUMBER,
    TKN_STRING,
    TKN_PARENL,
    TKN_PARENR,
    TKN_ARRAYL,
    TKN_ARRAYR,
    TKN_ASSIGN,
    TKN_KEYWORD,
    TKN_OPERATOR,
    TKN_SEMICOLON

} token_type;

/**
 * @brief Structure of a token
 * @author Antoine LANDRIEUX
 */
typedef struct Tokens
{

    // Value
    char *value;
    // Type
    token_type type;

    // Next
    struct Tokens *next;

} Tokens;

/**
 * @brief Create a new token
 * @author Antoine LANDRIEUX
 *
 * @param value
 * @param type
 * @return Tokens*
 */
Tokens *Token(char *value, token_type type);

/**
 * @brief Move on to the next token
 * @author Antoine LANDRIEUX
 *
 * @param tokens
 */
void TokenNext(Tokens **tokens);

/**
 * @brief Transform a string into a sequence of tokens
 * @author Antoine LANDRIEUX
 *
 * @param text
 * @return Tokens*
 */
Tokens *Tokenizer(char *text);

#endif /* __SOARE_TOKENIZER_H__ */