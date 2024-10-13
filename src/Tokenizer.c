#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <SOARE/SOARE.h>

static uint8_t chrNum(const char _Char)
{
    return _Char >= '0' && _Char <= '9';
}

static uint8_t chrAlpha(const char _Char)
{
    return (_Char >= 'a' && _Char <= 'z') || (_Char >= 'A' && _Char <= 'Z');
}

static uint8_t chrAlnum(const char _Char)
{
    return chrAlpha(_Char) || chrNum(_Char);
}

static uint8_t chrSpace(const char _Char)
{
    return strchr(" \t\n\r", _Char) != NULL;
}

static uint8_t strKeyword(char *_String)
{
    return (
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
        !strcmp("loadimport", _String));
}

static uint8_t strType(char *_String)
{
    return (
        !strcmp("Array", _String) ||
        !strcmp("String", _String) ||
        !strcmp("Number", _String));
}

TOKENS Token(char *_Value, TokenType _Type)
{
    TOKENS token = (TOKENS)malloc(sizeof(Tokens));

    if (token == NULL)
        return NULL;

    token->value = _Value;
    token->type = _Type;
    token->next = NULL;

    return token;
}

void TokensFree(TOKENS _Token)
{
    if (_Token == NULL)
        return;

    free(_Token->value);
    TokensFree(_Token->next);
    free(_Token);
}

void TokensLog(TOKENS _Token)
{
    if (_Token == NULL)
        return;

    printf("[TOKEN] %p\t %.2X, \"%s\"\n", _Token, _Token->type, _Token->value);
    TokensLog(_Token->next);
}

static char *strcut(const char *_String, size_t _Long)
{
    if (strlen(_String) < _Long)
        _Long = strlen(_String);
    char *result = (char *)malloc(_Long);
    if (result == NULL)
        return NULL;
    for (size_t ptr = 0; ptr < _Long; ptr++)
        result[ptr] = _String[ptr];
    result[_Long] = 0;
    return result;
}

TOKENS Tokenizer(const char *_Text)
{
    if (_Text == NULL)
        return NULL;

    TOKENS token = Token(NULL, TKN_EOF);
    TOKENS curr = token;

    while (*_Text)
    {
        if (chrSpace(*_Text))
        {
            (volatile char *)_Text++;
            continue;
        }

        char *content = NULL;
        TokenType type = TKN_EOF;

        uint64_t offset = 0;

        if (chrAlpha(*_Text))
        {
            while (chrAlnum((&*_Text)[offset]))
                offset++;
            content = strcut(&*_Text, offset);
            type = strKeyword(content) ? TKN_KEYWORD : TKN_NAME;
        }

        else if (chrNum(*_Text))
        {
            uint8_t point = 0;
            while (chrNum((&*_Text)[offset]) || ((&*_Text)[offset] == '.' && !point))
                offset++;
            content = strcut(&*_Text, offset);
            type = TKN_NUMBER;
        }

        else if (strchr("\"'`", *_Text) != NULL)
        {
            char quote = *_Text;
            (volatile char *)_Text++;
            while ((&*_Text)[offset] != quote && (&*_Text)[offset])
                offset++;
            content = strcut(&*_Text, offset);
            type = TKN_STRING;
            offset++;
        }

        else
        {
            TokensFree(token);
            return NULL;
        }

        curr->value = content;
        curr->type = type;
        curr->next = Token(NULL, TKN_EOF);
        curr = curr->next;

        for (uint64_t i = 0; i < offset; i++)
            (volatile char *)_Text++;
    }

    return token;
}
