#ifndef __SOARE_TOKENIZER_H__
#define __SOARE_TOKENIZER_H__ 0x1

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
typedef enum token_type
{
    
    TKN_EOF,
    TKN_TYPE,
    TKN_NAME,
    TKN_NUMBER,
    TKN_STRING,
    TKN_KEYWORD,
    TKN_SEMICOLON

} token_type;

/**
 * @brief 
 * 
 */
typedef struct Tokens
{
    
    char *value;
    token_type type;

    Document file;
    
    struct Tokens *next;

} Tokens;

/**
 * @brief 
 * 
 * @return Document 
 */
Document EmptyDocument();

/**
 * @brief 
 * 
 * @param _Token 
 */
void TokensLog(Tokens *_Token);

/**
 * @brief 
 * 
 * @param _Token 
 */
void TokensFree(Tokens *_Token);

/**
 * @brief 
 * 
 * @param _Filename 
 * @param _Text 
 * @return Tokens* 
 */
Tokens *Tokenizer(char *_Filename, char *_Text);

#endif /* __SOARE_TOKENIZER_H__ */