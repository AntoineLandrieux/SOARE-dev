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
    TKN_ASSIGN,
    TKN_KEYWORD,
    TKN_FUNCTION,
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

    // Document
    Document file;

    // Next
    struct Tokens *next;

} Tokens;

/**
 * @brief Return an empty document
 * @author Antoine LANDRIEUX
 *
 * @return Document
 */
Document EmptyDocument();

/**
 * @brief Create a new token
 * @author Antoine LANDRIEUX
 *
 * @param _Filename
 * @param _Value
 * @param _Type
 * @return Tokens*
 */
Tokens *Token(char *_Filename, char *_Value, token_type _Type);

/**
 * @brief Free the memory allocated by the tokens
 * @author Antoine LANDRIEUX
 *
 * @param _Token
 */
void TokensFree(Tokens *_Token);

/**
 * @brief Display the tokens
 * @author Antoine LANDRIEUX
 *
 * @param _Token
 */
void TokensLog(Tokens *_Token);

/**
 * @brief Transform a string into a sequence of tokens
 * @author Antoine LANDRIEUX
 *
 * @param _Filename
 * @param _Text
 * @return Tokens*
 */
Tokens *Tokenizer(char *_Filename, char *_Text);

#endif /* __SOARE_TOKENIZER_H__ */