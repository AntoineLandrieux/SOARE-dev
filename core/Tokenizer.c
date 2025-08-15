#include <DRIVER/video.h>

#include <STD/stdarg.h>
#include <STD/stdlib.h>

/**
 *  _____  _____  ___  ______ _____
 * /  ___||  _  |/ _ \ | ___ \  ___|
 * \ `--. | | | / /_\ \| |_/ / |__
 *  `--. \| | | |  _  ||    /|  __|
 * /\__/ /\ \_/ / | | || |\ \| |___
 * \____/  \___/\_| |_/\_| \_\____/
 *
 * Antoine LANDRIEUX (MIT License) <Tokenizer.c>
 * <https://github.com/AntoineLandrieux/SOARE/>
 *
 */

#include <SOARE/SOARE.h>

/* Environment */
static char *Environment = "/";

/**
 * @brief Set the Environment object
 *
 * @param path
 */
void SetEnvironment(char *path)
{
    Environment = path;
}

/**
 * @brief Get the Environment object
 *
 * @return char*
 */
char *GetEnvironment(void)
{
    return Environment;
}

/**
 * @brief Check if a character is a number
 *
 * @param character
 *
 * @return unsigned char
 */
static inline unsigned char chrNum(const char character)
{
    return character >= '0' && character <= '9';
}

/**
 * @brief Check if the character is a letter
 *
 * @param character
 * @return unsigned char
 */
static inline unsigned char chrAlpha(const char character)
{
    return (character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z');
}

/**
 * @brief Check if the character is a letter or a number
 *
 * @param character
 * @return unsigned char
 */
static inline unsigned char chrAlnum(const char character)
{
    return chrAlpha(character) || chrNum(character);
}

/**
 * @brief Check if the character is a space
 *
 * @param character
 * @return unsigned char
 */
static inline unsigned char chrSpace(const char character)
{
    return character == ' ' || character == '\t' || character == '\r' || character == '\n';
}

/**
 * @brief Check if the character is an operator
 *
 * @param string
 * @return unsigned char
 */
static inline unsigned char chrOperator(const char character)
{
    return strchr("<,+-^*/%>", character);
}

/**
 * @brief Check if the string is an operator
 *
 * @param string
 * @return unsigned char
 */
static inline unsigned char strOperator(char *string)
{
    return (
        //
        !strcmp("==", string) ||
        !strcmp("<=", string) ||
        !strcmp(">=", string) ||
        !strcmp("!=", string) ||
        !strcmp("~=", string) || // Same as !=
        !strcmp("&&", string) ||
        !strcmp("||", string)
        //
    );
}

/**
 * @brief Check if the string is a keyword
 *
 * @param string
 * @return unsigned char
 */
static inline unsigned char strKeyword(char *string)
{
    return (
        //
        !strcmp(KEYWORD_DO, string) ||
        !strcmp(KEYWORD_FN, string) ||
        !strcmp(KEYWORD_IF, string) ||
        !strcmp(KEYWORD_OR, string) ||
        !strcmp(KEYWORD_LET, string) ||
        !strcmp(KEYWORD_TRY, string) ||
        !strcmp(KEYWORD_END, string) ||
        !strcmp(KEYWORD_ELSE, string) ||
        !strcmp(KEYWORD_INPUT, string) ||
        !strcmp(KEYWORD_WRITE, string) ||
        !strcmp(KEYWORD_WHILE, string) ||
        !strcmp(KEYWORD_RAISE, string) ||
        !strcmp(KEYWORD_BREAK, string) ||
        !strcmp(KEYWORD_RETURN, string) ||
        !strcmp(KEYWORD_IFERROR, string)
        //
    );
}

/**
 * @brief Give the type of the string
 *
 * @param string
 * @return token_type
 */
static inline token_type Symbol(char *string)
{
    return strKeyword(string) ? TKN_KEYWORD : TKN_NAME;
}

/**
 * @brief Create a new token
 *
 * @param filename
 * @param value
 * @param type
 * @return Tokens*
 */
Tokens *Token(char *value, token_type type)
{
    Tokens *token = (Tokens *)malloc(sizeof(Tokens));

    if (!token)
        return __SOARE_OUT_OF_MEMORY();

    token->value = !value ? NULL : vardup(value);
    token->type = type;
    token->next = NULL;

    return token;
}

/**
 * @brief Move on to the next token
 *
 * @param tokens
 * @param step
 */
void TokenNext(Tokens **tokens, unsigned int step)
{
    for (unsigned int _ = 0; _ < step; _++)
        *tokens = (*tokens)->next;
}

/**
 * @brief Check if a sequence of tokens corresponds with a sequence of token types
 *
 * @param tokens
 * @param iteration
 * @param ...
 * @return unsigned char
 */
unsigned char TokensFollowPattern(Tokens *tokens, unsigned int iteration, ...)
{
    va_list args;
    va_start(args, iteration);
    unsigned char result = 1;

    for (unsigned int i = 0; result && i < iteration; i++)
    {
        result = va_arg(args, token_type) == tokens->type;
        tokens = tokens->next;
        result = result && (tokens || i - 1 == iteration);
    }

    va_end(args);
    return result;
}

/**
 * @brief Cut a string
 * @author Antoine LANDRIEUX
 *
 * @param string
 * @param size
 * @return char*
 */
static char *strcut(const char *string, long long size)
{
    if (strlen(string) < size)
        size = strlen(string);
    char *result = (char *)malloc(size + 1);
    if (!result)
        return __SOARE_OUT_OF_MEMORY();
    for (long long ptr = 0; ptr < size; ptr++)
        result[ptr] = string[ptr];
    result[size] = 0;
    return result;
}

/**
 * @brief Transform a string into a sequence of tokens
 *
 * @param filename
 * @param text
 * @return Tokens*
 */
Tokens *Tokenizer(char *text)
{
    if (!text)
        return NULL;

    Tokens *token = Token(NULL, TKN_EOF);
    Tokens *curr = token;

    while (*text)
    {
        // Check for errors
        if (ErrorLevel())
            return NULL;

        // Ignore space sequence
        if (chrSpace(*text))
        {
            text++;
            continue;
        }

        // Comments
        else if (*text == '?')
        {
            // Comments end with a new line ('\n')
            while (*text != '\n' && *text)
                text++;
            continue;
        }

        token_type type = TKN_EOF;
        unsigned long long offset = 1;

        // Let text = "<="
        char operator[3] = {
            //
            0 [text], // <
            1 [text], // =
            0         // Null character
            //
        };

        // Check if the string (operator) is an operator
        if (strOperator(operator))
        {
            offset += 1;
            type = TKN_OPERATOR;
        }

        // Assign
        else if (*text == '=')
            type = TKN_ASSIGN;

        // Shell
        else if (*text == '$')
            type = TKN_KEYWORD;

        // Parentheses
        else if (strchr("()", *text))
            type = *text == '(' ? TKN_PARENL : TKN_PARENR;

        // Semicolon or operator
        else if (chrOperator(*text) || *text == ';')
            type = *text == ';' ? TKN_SEMICOLON : TKN_OPERATOR;

        // Name
        else if (chrAlpha(*text) || *text == '_')
            while (chrAlnum((&*text)[offset]) || (&*text)[offset] == '_')
                offset++;

        // Number (int, float)
        else if (chrNum(*text))
        {
            unsigned char point = 0;
            while (chrNum((&*text)[offset]) || ((&*text)[offset] == '.' && !point))
                offset++;
            type = TKN_NUMBER;
        }

        // String `str`|'str'|"str"
        else if (strchr("\"'`", *text))
        {
            offset--;
            char ignore = 0;
            char quote = *text;
            text++;

            while ((&*text)[offset] && ((&*text)[offset] != quote || ignore))
            {
                ignore = !ignore && (&*text)[offset] == '\\';
                offset++;
            }

            type = TKN_STRING;
            offset++;
        }

        // Error
        else
        {
            LeaveException(CharacterError, &*text);
            continue;
        }

        // Add token
        curr->value = type == TKN_STRING ? strcut(&*text, offset - 1) : strcut(&*text, offset);
        curr->type = type == TKN_EOF ? Symbol(curr->value) : type;
        curr->next = Token(NULL, TKN_EOF);
        curr = curr->next;

        // Update text pointer
        for (unsigned long long i = 0; i < offset; i++)
            text++;
    }

    // Return token
    return token;
}
