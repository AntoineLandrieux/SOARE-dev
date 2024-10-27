#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 *
 */

#include <SOARE/SOARE.h>

/**
 * @brief
 *
 * @param _Char
 * @return unsigned char
 */
static unsigned char chrNum(const char _Char)
{
    return _Char >= '0' && _Char <= '9';
}

/**
 * @brief
 *
 * @param _Char
 * @return unsigned char
 */
static unsigned char chrAlpha(const char _Char)
{
    return (_Char >= 'a' && _Char <= 'z') || (_Char >= 'A' && _Char <= 'Z');
}

/**
 * @brief
 *
 * @param _Char
 * @return unsigned char
 */
static unsigned char chrAlnum(const char _Char)
{
    return chrAlpha(_Char) || chrNum(_Char);
}

/**
 * @brief
 *
 * @param _Char
 * @return unsigned char
 */
static unsigned char chrLn(const char _Char)
{
    return _Char == '\r' || _Char == '\n';
}

/**
 * @brief
 *
 * @param _Char
 * @return unsigned char
 */
static unsigned char chrSpace(const char _Char)
{
    return _Char == ' ' || _Char == '\t' || chrLn(_Char);
}

/**
 * @brief
 *
 * @param _String
 * @return unsigned char
 */
static unsigned char strKeyword(char *_String)
{
    return (
        //
        !strcmp("if", _String) ||
        !strcmp("try", _String) ||
        !strcmp("elif", _String) ||
        !strcmp("else", _String) ||
        !strcmp("echo", _String) ||
        !strcmp("exit", _String) ||
        !strcmp("close", _String) ||
        !strcmp("raise", _String) ||
        !strcmp("while", _String) ||
        !strcmp("break", _String) ||
        !strcmp("prompt", _String) ||
        !strcmp("typeof", _String) ||
        !strcmp("return", _String) ||
        !strcmp("iferror", _String) ||
        !strcmp("continue", _String) ||
        !strcmp("function", _String) ||
        !strcmp("loadimport", _String)
        //
    );
}

/**
 * @brief
 *
 * @param _String
 * @return unsigned char
 */
static unsigned char strType(char *_String)
{
    return (
        //
        !strcmp("Int", _String) ||
        !strcmp("Array", _String) ||
        !strcmp("Float", _String) ||
        !strcmp("String", _String)
        //
    );
}

/**
 * @brief
 *
 * @param _Filename
 * @param _Value
 * @param _Type
 * @return Tokens*
 */
Tokens *Token(char *_Filename, char *_Value, token_type _Type)
{
    Tokens *token = (Tokens *)malloc(sizeof(Tokens));

    if (token == NULL)
        return NULL;

    token->file = _Filename;
    token->value = _Value;
    token->type = _Type;
    token->ln = 0;
    token->col = 0;
    token->next = NULL;

    return token;
}

/**
 * @brief
 *
 * @param _Token
 */
void TokensFree(Tokens *_Token)
{
    if (_Token == NULL)
        return;

    free(_Token->value);
    TokensFree(_Token->next);
    free(_Token);
}

/**
 * @brief
 *
 * @param _Token
 */
void TokensLog(Tokens *_Token)
{
    if (_Token == NULL)
        return;

    printf(
        "[TOKEN] %p %s:%.5ld:%.5ld %.2X, \"%s\"\n",
        _Token,
        _Token->file,
        _Token->ln,
        _Token->col,
        _Token->type,
        _Token->value);
    TokensLog(_Token->next);
}

/**
 * @brief
 *
 * @param _String
 * @param _Long
 * @return char*
 */
static char *strcut(const char *_String, size_t _Long)
{
    if (strlen(_String) < _Long)
        _Long = strlen(_String);
    char *result = (char *)malloc(_Long + 1);
    if (result == NULL)
        return NULL;
    for (size_t ptr = 0; ptr < _Long; ptr++)
        result[ptr] = _String[ptr];
    result[_Long] = 0;
    return result;
}

/**
 * @brief
 *
 * @param ln
 * @param col
 */
static void updateln(unsigned long *ln, unsigned long *col)
{
    *ln = *ln + 1;
    *col = 0;
}

/**
 * @brief
 *
 * @param _Filename
 * @param _Text
 * @return Tokens*
 */
Tokens *Tokenizer(char *_Filename, char *_Text)
{
    if (_Text == NULL)
        return NULL;

    Tokens *token = Token(_Filename, NULL, TKN_EOF);
    Tokens *curr = token;

    unsigned long col = 0;
    unsigned long ln = 0;

    updateln(&ln, &col);

    while (*_Text)
    {
        if (chrSpace(*_Text))
        {
            chrLn(*_Text) ? updateln(&ln, &col) : col++;
            (volatile char *)_Text++;
            continue;
        }

        char *content = NULL;
        token_type type = TKN_EOF;
        unsigned long long offset = 0;

        if (chrAlpha(*_Text))
        {
            while (chrAlnum((&*_Text)[offset]))
                offset++;
            content = strcut(&*_Text, offset);
            type = strKeyword(content) ? TKN_KEYWORD : (strType(content) ? TKN_TYPE : TKN_NAME);
        }

        else if (chrNum(*_Text))
        {
            unsigned char point = 0;
            while (chrNum((&*_Text)[offset]) || ((&*_Text)[offset] == '.' && !point))
                offset++;
            content = strcut(&*_Text, offset);
            type = TKN_NUMBER;
        }

        else if (strchr("\"'`", *_Text) != NULL)
        {
            char quote = *_Text;
            (volatile char *)_Text++;
            while ((&*_Text)[offset] != quote && (&*_Text)[offset] && !chrLn((&*_Text)[offset]))
                offset++;
            content = strcut(&*_Text, offset);
            type = TKN_STRING;
            offset++;
        }

        else
        {
            TokensFree(token);
            return LeaveException(ERR_CHARACTER, &*_Text, _Filename, ln, col);
        }

        curr->value = content;
        curr->type = type;
        curr->col = col;
        curr->ln = ln;
        curr->next = Token(_Filename, NULL, TKN_EOF);
        curr = curr->next;

        for (unsigned long long i = 0; i < offset; col += (volatile unsigned long long)i++)
            (volatile char *)_Text++;
    }

    return token;
}
