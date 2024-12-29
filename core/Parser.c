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
 * @param _Value
 * @param _Type
 * @param _File
 *
 * @return Node*
 */
Node *Branch(char *_Value, node_type _Type, Document _File)
{
    Node *branch = (Node *)malloc(sizeof(Node));

    if (branch == NULL)
        return LeaveException(InterpreterError, "OUT OF MEMORY", EmptyDocument());

    branch->value = _Value == NULL ? NULL : strdup(_Value);
    branch->type = _Type;
    branch->file = _File;
    branch->parent = NULL;
    branch->child = NULL;
    branch->sibling = NULL;

    return branch;
}

/**
 * @brief
 * @author Antoine LANDRIEUX
 *
 * @param _Parent
 * @param _Child
 * @return AST*
 */
AST *JoinBranch(Node *_Parent, Node *_Child)
{
    if (_Child == NULL || _Parent == NULL)
        return NULL;
    if (_Parent->child == NULL)
        _Parent->child = _Child;
    else
    {
        Node *tmp = _Parent->child;
        while (tmp->sibling != NULL)
            tmp = tmp->sibling;
        tmp->sibling = _Child;
    }
    _Child->parent = _Parent;
    return _Parent;
}

/**
 * @brief
 * @author Antoine LANDRIEUX
 *
 * @param _Tree
 */
void TreeFree(AST *_Tree)
{
    if (_Tree == NULL)
        return;

    TreeFree(_Tree->child);
    TreeFree(_Tree->sibling);
    free(_Tree->value);
    free(_Tree);
}

/**
 * @brief
 * @author Antoine LANDRIEUX
 *
 * @param _Tree
 */
void TreeLog(AST *_Tree)
{
    if (_Tree == NULL)
        return;

    printf("[BRANCH] ");

    if (_Tree->parent != NULL)
        printf(
            "[%p, %s:%.5lld:%.5lld, %.2X, \"%s\"]\t",
            (void *)_Tree->parent,
            _Tree->parent->file.file,
            _Tree->parent->file.ln,
            _Tree->parent->file.col,
            _Tree->parent->type,
            _Tree->parent->value);
    printf(
        "[%p, %s:%.5lld:%.5lld, %.2X, \"%s\"]\n",
        (void *)_Tree,
        _Tree->file.file,
        _Tree->file.ln,
        _Tree->file.col,
        _Tree->type,
        _Tree->value);
    TreeLog(_Tree->child);
    TreeLog(_Tree->sibling);
}

/**
 * @brief
 * @author Antoine LANDRIEUX
 *
 * @param _Tokens
 */
static void Next(Tokens **_Tokens)
{
    *_Tokens = (*_Tokens)->next;
}

static AST *ParseExpr(Tokens **_Tokens, u8 _Priority);

/**
 * @brief
 * @author Antoine LANDRIEUX
 *
 * @param _Tokens
 * @return AST*
 */
static AST *ParseValue(Tokens **_Tokens)
{
    Node *value = Branch((*_Tokens)->value, NODE_ROOT, (*_Tokens)->file);
    Tokens *old = *_Tokens;
    Next(_Tokens);

    switch (old->type)
    {
    case TKN_NUMBER:
        value->type = NODE_NUMBER;
        break;

    case TKN_STRING:
        value->type = NODE_STRING;
        break;

    case TKN_NAME:
        value->type = NODE_MEMGET;

        if ((*_Tokens)->type != TKN_FUNCTION)
            break;

        Next(_Tokens);

        if ((*_Tokens)->type != TKN_PARENL)
        {
            free(value);
            return NULL;
        }

        Next(_Tokens);

        while (1)
        {
            TokensLog(*_Tokens);
            AST *expr = ParseExpr(_Tokens, 0xFF);

            if (expr == NULL)
            {
                TreeFree(value);
                return NULL;
            }

            JoinBranch(value, expr);
            if ((*_Tokens)->type != TKN_SEMICOLON)
                break;
            Next(_Tokens);
        }

        if ((*_Tokens)->type != TKN_PARENR)
        {
            TreeFree(value);
            return NULL;
        }

        Next(_Tokens);
        break;

    default:
        TreeFree(value);
        return NULL;
    }

    return value;
}

/**
 * @brief
 *
 * @param _Operator
 * @return u8
 */
static u8 MathPriority(char _Operator)
{
    if (strchr("/*%^", _Operator))
        return 0;
    else if (strchr("+-", _Operator))
        return 1;
    else if (strchr("<=!>", _Operator))
        return 2;
    return 3;
}

/**
 * @brief
 *
 * @param _Tokens
 * @return AST*
 */
static AST *ParseExpr(Tokens **_Tokens, u8 _Priority)
{
    Node *left = ParseValue(_Tokens);
    Node *right = NULL;
    Node *symbol = NULL;

    if (left == NULL)
        return NULL;

    while ((*_Tokens)->type == TKN_OPERATOR && !ErrorLevel())
    {
        u8 priority = MathPriority(*(*_Tokens)->value);

        if (priority >= _Priority)
            break;

        symbol = Branch((*_Tokens)->value, NODE_OPERATOR, (*_Tokens)->file);
        Next(_Tokens);
        right = ParseExpr(_Tokens, priority);

        if (symbol == NULL || right == NULL)
        {
            TreeFree(left);
            TreeFree(right);
            TreeFree(symbol);
            return NULL;
        }

        JoinBranch(symbol, left);
        JoinBranch(symbol, right);

        left = symbol;
    }

    return left;
}

/**
 * @brief
 * @author Antoine LANDRIEUX
 *
 * @return AST*
 */
AST *Parse(Tokens *_Tokens)
{
    Node *root = Branch("root", NODE_ROOT, EmptyDocument());
    Node *curr = root;

    while (_Tokens != NULL)
    {
        if (_Tokens->type == TKN_EOF)
            break;

        Tokens *old = _Tokens;
        Next(&_Tokens);

        switch (old->type)
        {
        case TKN_SEMICOLON:
            break;

        case TKN_KEYWORD:

            if (!strcmp(old->value, "nop"))
                JoinBranch(curr, Branch(curr->value, NODE_NOP, old->file));

            else if (!strcmp(old->value, "return"))
                JoinBranch(
                    //
                    curr,
                    //
                    JoinBranch(
                        //
                        Branch(old->value, NODE_OUTPUT, old->file),
                        ParseExpr(&_Tokens, 0xFF)
                        //
                        )
                    //
                );

            else if (!strcmp(old->value, "loadimport") || !strcmp(old->value, "raise"))
            {
                if (_Tokens->type != TKN_STRING)
                {
                    TreeFree(root);
                    return LeaveException(SyntaxError, old->value, old->file);
                }

                JoinBranch(curr, Branch(_Tokens->value, strcmp(old->value, "raise") ? NODE_IMPORT : NODE_RAISE, old->file));
                Next(&_Tokens);
            }

            else if (!strcmp(old->value, "try"))
            {
                Node *try = Branch(old->value, NODE_TRY, old->file);
                JoinBranch(try, Branch("BODY", NODE_BODY, old->file));
                JoinBranch(curr, try);

                curr = try->child;
            }

            else if (!strcmp(old->value, "iferror"))
            {
                if (curr->parent->type != NODE_TRY || curr->type == NODE_IFERROR)
                {
                    TreeFree(root);
                    return LeaveException(UnexpectedNear, old->value, old->file);
                }

                Node *iferror = Branch(old->value, NODE_IFERROR, old->file);
                JoinBranch(curr->parent, iferror);

                curr = iferror;
            }

            else if (!strcmp(old->value, "def"))
            {
                while (1)
                {
                    old = _Tokens;
                    Next(&_Tokens);

                    if (old->type != TKN_TYPE || _Tokens->type != TKN_NAME)
                    {
                        TreeFree(root);
                        return LeaveException(UnexpectedNear, old->value, old->file);
                    }

                    JoinBranch(
                        //
                        curr,
                        //
                        JoinBranch(
                            //
                            Branch(_Tokens->value, NODE_MEMSET, _Tokens->file),
                            Branch(old->value, NODE_TYPE, old->file)
                            //
                            )
                        //
                    );

                    Next(&_Tokens);

                    if (_Tokens->type == TKN_OPERATOR && *(_Tokens->value) == '+')
                    {
                        Next(&_Tokens);
                        continue;
                    }

                    break;
                }
            }

            else if (!strcmp(old->value, "if") || !strcmp(old->value, "while"))
            {
                AST *content = ParseExpr(&_Tokens, 0xFF);

                if (content == NULL || _Tokens->type != TKN_KEYWORD || strcmp(_Tokens->value, "do"))
                {
                    TreeFree(root);
                    return LeaveException(SyntaxError, old->value, old->file);
                }

                AST *statement = Branch(old->value, strcmp(old->value, "if") ? NODE_REPETITION : NODE_CONDITION, old->file);
                AST *body = Branch("BODY", NODE_BODY, old->file);

                JoinBranch(statement, content);
                JoinBranch(statement, body);
                JoinBranch(curr, statement);

                curr = body;
                Next(&_Tokens);
            }

            else if (!strcmp(old->value, "orif"))
            {
                if (curr->parent->type != NODE_CONDITION)
                {
                    TreeFree(root);
                    return LeaveException(UnexpectedNear, old->value, old->file);
                }

                AST *content = ParseExpr(&_Tokens, 0xFF);

                if (content == NULL || _Tokens->type != TKN_KEYWORD || strcmp(_Tokens->value, "do"))
                {
                    TreeFree(root);
                    return LeaveException(SyntaxError, old->value, old->file);
                }

                AST *body = Branch("BODY", NODE_BODY, old->file);

                JoinBranch(curr->parent, content);
                JoinBranch(curr->parent, body);

                curr = body;
                Next(&_Tokens);
            }

            else if (!strcmp(old->value, "else"))
            {
                if (curr->parent->type != NODE_CONDITION)
                {
                    TreeFree(root);
                    return LeaveException(UnexpectedNear, old->value, old->file);
                }

                AST *body = Branch("BODY", NODE_BODY, old->file);

                JoinBranch(curr->parent, Branch("1", NODE_NUMBER, old->file));
                JoinBranch(curr->parent, body);

                curr = body;
            }

            else if (!strcmp(old->value, "writeln"))
            {
                AST *content = ParseExpr(&_Tokens, 0xFF);

                if (content == NULL)
                {
                    TreeFree(root);
                    return LeaveException(SyntaxError, old->value, old->file);
                }

                JoinBranch(
                    //
                    curr,
                    //
                    JoinBranch(
                        //
                        Branch(old->value, NODE_OUTPUT, old->file),
                        content
                        //
                        )
                    //
                );
            }

            else if (!strcmp(old->value, "continue") || !strcmp(old->value, "break"))
            {
                if (curr == root)
                {
                    TreeFree(root);
                    return LeaveException(UnexpectedNear, old->value, old->file);
                }

                JoinBranch(curr, Branch(old->value, strcmp(old->value, "continue") ? NODE_BREAK : NODE_CONTINUE, old->file));
            }

            else if (!strcmp(old->value, "end"))
            {
                if (curr == root)
                {
                    TreeFree(root);
                    return LeaveException(UnexpectedNear, old->value, old->file);
                }
                
                curr = curr->parent->parent;
            }

            break;

        default:
            TreeFree(root);
            return LeaveException(UnexpectedNear, old->value, old->file);
        }
    }

    return (AST *)root;
}
