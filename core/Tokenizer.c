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
static u8 chrOperator(const char _Char)
{
    return strchr("<+-^*/%%>", _Char) != NULL;
}

/**
 * @brief
 * @author Antoine LANDRIEUX
 *
 * @param _String
 * @return u8
 */
static u8 strOperator(char *_String)
{
    return (
        !strcasecmp("equ", _String) ||
        !strcasecmp("neq", _String) ||
        !strcasecmp("and", _String) ||
        !strcasecmp("or", _String));
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
        !strcmp("do", _String) ||       // tk ok ps ok ex na
        !strcmp("if", _String) ||       // tk ok ps ok ex ok
        !strcmp("try", _String) ||      // tk ok ps ok ex ok
        !strcmp("orif", _String) ||     // tk ok ps ok ex ok
        !strcmp("else", _String) ||     // tk ok ps ok ex ok
        !strcmp("close", _String) ||    // tk ok ps ok ex na
        !strcmp("raise", _String) ||    // tk ok ps ok ex ok
        !strcmp("while", _String) ||    // tk ok ps ok ex ok
        !strcmp("prompt", _String) ||   // tk ok ps -- ex --
        !strcmp("return", _String) ||   // tk ok ps ok ex ok
        !strcmp("writeln", _String) ||  // tk ok ps ok ex ok
        !strcmp("iferror", _String) ||  // tk ok ps ok ex ok
        !strcmp("function", _String) || // tk ok ps -- ex --
        !strcmp("loadimport", _String)  // tk ok ps ok ex --
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
        !strcmp("Float", _String) ||
        !strcmp("String", _String)
        //
    );
}

/**
 * @brief 
 * 
 * @param _String 
 * @return token_type 
 */
static token_type Symbol(char *_String)
{
    if (strKeyword(_String))
        return TKN_KEYWORD;
    else if (strType(_String))
        return TKN_TYPE;
    else if (strOperator(_String))
        return TKN_OPERATOR;
    return TKN_NAME;
}

/**
 * @brief 
 * 
 * @return Document 
 */
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
        return LeaveException(InterpreterError, "OUT OF MEMORY", EmptyDocument());

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
        "[TOKENS] [%p, %s:%.5lld:%.5lld, %.2X, \"%s\"]\n",
        (void *)_Token,
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
        if (ErrorLevel())
        {
            TokensFree(token);
            return NULL;
        }

        if (chrSpace(*_Text))
        {
            col++;
            if (chrLn(*_Text))
                updateln(&ln, &col);
            (volatile char *)_Text++;
            continue;
        }

        else if (*_Text == '?')
        {
            while (!chrLn(*_Text) && *_Text)
                (volatile char *)_Text++;
            updateln(&ln, &col);
            continue;
        }

        char *content = NULL;
        token_type type = TKN_EOF;
        u64 offset = 0;

        curr->file.ln = ln;
        curr->file.col = col;

        if (strchr("()", *_Text))
        {
            offset++;
            content = strcut(&*_Text, offset);
            type = *_Text == '(' ? TKN_PARENL : TKN_PARENR;
        }

        else if (chrOperator(*_Text) || *_Text == '!' || *_Text == ';')
        {
            offset++;
            content = strcut(&*_Text, offset);
            type = *_Text == ';' ? TKN_SEMICOLON : (*_Text == '!' ? TKN_FUNCTION : TKN_OPERATOR);
        }

        else if (chrAlpha(*_Text))
        {
            while (chrAlnum((&*_Text)[offset]))
                offset++;
            content = strcut(&*_Text, offset);
            type = Symbol(content);
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
            LeaveException(CharacterError, &*_Text, curr->file);
            continue;
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
