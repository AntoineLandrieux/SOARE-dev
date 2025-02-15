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
 *
 * Antoine LANDRIEUX (MIT License) <Tokenizer.c>
 * <https://github.com/AntoineLandrieux/SOARE/>
 *
 */

#include <SOARE/SOARE.h>
#include <SOARE/utils/int.h>
#include <SOARE/utils/keywords.h>

/**
 * @brief Check if a character is a number
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
 * @brief Check if the character is a letter
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
 * @brief Check if the character is a letter or a number
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
 * @brief Check if the character is a space
 * @author Antoine LANDRIEUX
 *
 * @param _Char
 * @return u8
 */
static u8 chrSpace(const char _Char)
{
    return _Char == ' ' || _Char == '\t' || _Char == '\r' || _Char == '\n';
}

/**
 * @brief Check if the character is an operator
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
 * @brief Check if the string is an operator
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
 * @brief Check if the string is a keyword
 * @author Antoine LANDRIEUX
 *
 * @param _String
 * @return u8
 */
static u8 strKeyword(char *_String)
{
    return (
        //
        !strcmp(KEYWORD_CONTINUE, _String) ||
        !strcmp(KEYWORD_DO, _String) ||
        !strcmp(KEYWORD_ELSE, _String) ||
        !strcmp(KEYWORD_END, _String) ||
        !strcmp(KEYWORD_ENUMERATE, _String) ||
        !strcmp(KEYWORD_IF, _String) ||
        !strcmp(KEYWORD_IFERROR, _String) ||
        !strcmp(KEYWORD_INPUTLN, _String) ||
        !strcmp(KEYWORD_LOADIMPORT, _String) ||
        !strcmp(KEYWORD_ORIF, _String) ||
        !strcmp(KEYWORD_RAISE, _String) ||
        !strcmp(KEYWORD_RETURN, _String) ||
        !strcmp(KEYWORD_TRY, _String) ||
        !strcmp(KEYWORD_WHILE, _String) ||
        !strcmp(KEYWORD_WRITELN, _String)
        //
    );
}

/**
 * @brief Give the type of the string
 *
 * @param _String
 * @return token_type
 */
static token_type Symbol(char *_String)
{
    if (strKeyword(_String))
        return TKN_KEYWORD;
    else if (strOperator(_String))
        return TKN_OPERATOR;
    return TKN_NAME;
}

/**
 * @brief Return an empty document
 * @author Antoine LANDRIEUX
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
 * @brief Create a new token
 * @author Antoine LANDRIEUX
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
 * @brief Free the memory allocated by the tokens
 * @author Antoine LANDRIEUX
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
 * @brief Display the tokens
 * @author Antoine LANDRIEUX
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
 * @brief Cut a string
 * @author Antoine LANDRIEUX
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
        return LeaveException(InterpreterError, "OUT OF MEMORY", EmptyDocument());
    for (size_t ptr = 0; ptr < _Long; ptr++)
        result[ptr] = _String[ptr];
    result[_Long] = 0;
    return result;
}

/**
 * @brief Add +1 to ln and set col to 0
 * @author Antoine LANDRIEUX
 *
 * @param ln
 * @param col
 */
static void updateln(u64 *ln, u64 *col)
{
    *ln = (*ln) + 1;
    *col = 1;
}

/**
 * @brief Transform a string into a sequence of tokens
 * @author Antoine LANDRIEUX
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
            if (*_Text == '\n')
                updateln(&ln, &col);
            (volatile char *)_Text++;
            continue;
        }

        else if (*_Text == '?')
        {
            while (*_Text != '\n' && *_Text)
                (volatile char *)_Text++;
            updateln(&ln, &col);
            continue;
        }

        token_type type = TKN_EOF;
        u64 offset = 1;

        curr->file.ln = ln;
        curr->file.col = col;

        if ('=' == *_Text)
            type = TKN_ASSIGN;

        else if ('@' == *_Text)
            type = TKN_KEYWORD;

        else if (strchr("()", *_Text))
            type = *_Text == '(' ? TKN_PARENL : TKN_PARENR;

        else if (chrOperator(*_Text) || *_Text == '!' || *_Text == ';')
            type = *_Text == ';' ? TKN_SEMICOLON : (*_Text == '!' ? TKN_FUNCTION : TKN_OPERATOR);

        else if (chrAlpha(*_Text) || *_Text == '_')
            while (chrAlnum((&*_Text)[offset]) || (&*_Text)[offset] == '_')
                offset++;

        else if (chrNum(*_Text))
        {
            u8 point = 0;
            while (chrNum((&*_Text)[offset]) || ((&*_Text)[offset] == '.' && !point))
                offset++;
            type = TKN_NUMBER;
        }

        else if (strchr("\"'`", *_Text) != NULL)
        {
            offset--;
            char quote = *_Text;
            (volatile char *)_Text++;
            while ((&*_Text)[offset] != quote && (&*_Text)[offset])
            {
                if ((&*_Text)[offset] != '\n')
                    updateln(&ln, &col);
                offset++;
            }
            type = TKN_STRING;
            offset++;
        }

        else
        {
            LeaveException(CharacterError, &*_Text, curr->file);
            continue;
        }

        curr->value = type == TKN_STRING ? strcut(&*_Text, offset - 1) : strcut(&*_Text, offset);
        curr->type = type == TKN_EOF ? Symbol(curr->value) : type;
        curr->next = Token(_Filename, NULL, TKN_EOF);
        curr = curr->next;

        for (u64 i = 0; i < offset; i++)
            (volatile char *)_Text++;
        col += offset + (type == TKN_STRING);
    }

    return token;
}
