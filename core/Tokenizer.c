#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 *  _____  _____  ___  ______ _____
 * /  ___||  _  |/ _ \ | ___ \  ___|
 * \ `--. | | | / /_\ \| |_/ / |__ 
 *  `--. \| | | |  _  ||    /|  __|
 * /\__/ /\ \_/ / | | || |\ \| |___
 * \____/  \___/\_| |_/\_| \_\____/
 */

#include <SOARE/SOARE.h>
#include <SOARE/utils/int.h>

/**
 * @brief
 * @author Antoine LANDRIEUX
 *
 * @param _Char
 * 
 * @return u8
 */
static u8 chrNum(const char _Char)
{
    return _Char >= '0' && _Char <= '9';
}

/**
 * @brief
 * @author Antoine LANDRIEUX
 *
 * @param _Char
 * @return u8
 */
static u8 chrAlpha(const char _Char)
{
    return (_Char >= 'a' && _Char <= 'z') || (_Char >= 'A' && _Char <= 'Z');
}

/**
 * @brief
 * @author Antoine LANDRIEUX
 *
 * @param _Char
 * @return u8
 */
static u8 chrAlnum(const char _Char)
{
    return chrAlpha(_Char) || chrNum(_Char);
}

/**
 * @brief
 * @author Antoine LANDRIEUX
 *
 * @param _Char
 * @return u8
 */
static u8 chrLn(const char _Char)
{
    return _Char == '\r' || _Char == '\n';
}

/**
 * @brief
 * @author Antoine LANDRIEUX
 *
 * @param _Char
 * @return u8
 */
static u8 chrSpace(const char _Char)
{
    return _Char == ' ' || _Char == '\t' || chrLn(_Char);
}

/**
 * @brief
 * @author Antoine LANDRIEUX
 *
 * @param _String
 * @return u8
 */
static u8 strKeyword(char *_String)
{
    return (
        //
        !strcmp("if", _String) ||
        !strcmp("nop", _String) ||
        !strcmp("try", _String) ||
        !strcmp("else", _String) ||
        !strcmp("write", _String) ||
        !strcmp("close", _String) ||
        !strcmp("raise", _String) ||
        !strcmp("while", _String) ||
        !strcmp("prompt", _String) ||
        !strcmp("return", _String) ||
        !strcmp("iferror", _String) ||
        !strcmp("function", _String) ||
        !strcmp("loadimport", _String)
        //
    );
}

/**
 * @brief
 *
 * @param _String
 * @return u8
 */
static u8 strType(char *_String)
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

Document EmptyDocument()
{
    Document document;

    document.file = NULL;
    document.ln = 0;
    document.col = 0;

    return document;
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

    token->value = _Value == NULL ? NULL : strdup(_Value);
    token->type = _Type;

    token->file.ln = 0;
    token->file.col = 0;
    token->file.file = _Filename;

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
        "[TOKENS] [0x%p, %s:%.5lld:%.5lld, %.2X, \"%s\"]\n",
        _Token,
        _Token->file.file,
        _Token->file.ln,
        _Token->file.col,
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
static void updateln(u64 *ln, u64 *col)
{
    *ln = *ln + 1;
    *col = 1;
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

    Tokens *token = Token(_Filename, "EOF", TKN_EOF);
    Tokens *curr = token;

    u64 col = 0;
    u64 ln = 0;

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
        u64 offset = 0;

        curr->file.ln = ln;
        curr->file.col = col;

        if (chrAlpha(*_Text))
        {
            while (chrAlnum((&*_Text)[offset]))
                offset++;
            content = strcut(&*_Text, offset);
            type = strKeyword(content) ? TKN_KEYWORD : (strType(content) ? TKN_TYPE : TKN_NAME);
        }

        else if (chrNum(*_Text))
        {
            u8 point = 0;
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
            return LeaveException(ERR_CHARACTER, &*_Text, curr->file);
        }

        curr->value = content;
        curr->type = type;
        curr->next = Token(_Filename, NULL, TKN_EOF);
        curr = curr->next;

        for (u64 i = 0; i < offset; col += (volatile u64)i++)
            (volatile char *)_Text++;
    }

    return token;
}
