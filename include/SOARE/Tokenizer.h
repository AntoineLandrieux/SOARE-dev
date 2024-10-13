#ifndef __SOARE_TOKENIZER_H__
#define __SOARE_TOKENIZER_H__ 0x1

typedef enum token_type
{
    
    TKN_EOF,
    TKN_NAME,
    TKN_NUMBER,
    TKN_STRING,
    TKN_KEYWORD

} TokenType;

typedef struct tokens
{
    
    char *value;
    TokenType type;
    
    struct tokens *next;

} Tokens, *TOKENS;

void TokensLog(TOKENS _Token);
void TokensFree(TOKENS _Token);
TOKENS Tokenizer(const char *_Text);

#endif /* __SOARE_TOKENIZER_H__ */