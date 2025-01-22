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
 * Antoine LANDRIEUX (WTFPL) <Parser.c>
 * <https://github.com/AntoineLandrieux/SOARE/>
 *
 * [!] Contribute and help me translate the comments!
 *
 */

#include <SOARE/SOARE.h>
#include <SOARE/utils/int.h>
#include <SOARE/utils/keywords.h>

/**
 * @brief Créer un nouveau noeud
 * @author Antoine LANDRIEUX
 *
 * @param _Value
 * @param _Type
 * @param _File
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
 * @brief Lie 2 branches
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
 * @brief Retourne le noeud racine
 * @author Antoine LANDRIEUX
 *
 * @param _Tree
 * @return Node*
 */
Node *BranchRoot(AST *_Tree)
{
    if (_Tree == NULL)
        return NULL;
    for (; _Tree->parent != NULL; _Tree = _Tree->parent)
        ;
    return _Tree->type == NODE_ROOT ? _Tree : NULL;
}

/**
 * @brief Retrouve un noeud
 * @author Antoine LANDRIEUX
 *
 * @param _Tree
 * @param _Value
 * @param _Type
 * @return Node*
 */
Node *BranchFind(AST *_Tree, char *_Value, node_type _Type)
{
    if (_Tree == NULL)
        return NULL;
    if (!strcmp(_Value == NULL ? "" : _Value, _Tree->value == NULL ? "" : _Tree->value) && _Tree->type == _Type)
        return _Tree;
    Node *L = BranchFind(_Tree->child, _Value, _Type);
    Node *R = BranchFind(_Tree->sibling, _Value, _Type);
    return L == NULL ? R : L;
}

/**
 * @brief Libère la mémoire allouée par le noeud et les noeuds lié avec lui
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
 * @brief Affiche le noeud et les noeuds liés avec lui
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
 * @brief Passe au jeton suivant
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
 * @brief Retourne la valeur sous forme de noeud
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

        value->type = NODE_CALL;

        Next(_Tokens);

        if ((*_Tokens)->type != TKN_PARENL)
        {
            free(value);
            return NULL;
        }

        Next(_Tokens);

        while ((*_Tokens)->type != TKN_PARENR)
        {
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
 * @brief Cherche le priorité mathématique d'un operateur
 * @author Antoine LANDRIEUX
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
 * @brief Construit un arbre mathématique
 * @author Antoine LANDRIEUX
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
 * @brief Transforme une séquence de jetons en arbre (AST)
 * @author Antoine LANDRIEUX
 *
 * @param _Tokens
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

            if (!strcmp(old->value, KEYWORD_NOP))
                JoinBranch(curr, Branch(curr->value, NODE_NOP, old->file));

            else if (!strcmp(old->value, KEYWORD_RETURN))
                JoinBranch(
                    //
                    curr,
                    //
                    JoinBranch(
                        //
                        Branch(old->value, NODE_RETURN, old->file),
                        ParseExpr(&_Tokens, 0xFF)
                        //
                        )
                    //
                );

            else if (!strcmp(old->value, KEYWORD_LOADIMPORT) || !strcmp(old->value, KEYWORD_RAISE))
            {
                if (_Tokens->type != TKN_STRING)
                {
                    TreeFree(root);
                    return LeaveException(SyntaxError, old->value, old->file);
                }

                JoinBranch(curr, Branch(_Tokens->value, strcmp(old->value, KEYWORD_RAISE) ? NODE_IMPORT : NODE_RAISE, old->file));
                Next(&_Tokens);
            }

            else if (!strcmp(old->value, KEYWORD_TRY))
            {
                Node *try = Branch(old->value, NODE_TRY, old->file);
                JoinBranch(try, Branch("BODY", NODE_BODY, old->file));
                JoinBranch(curr, try);

                curr = try->child;
            }

            else if (!strcmp(old->value, KEYWORD_ENUMERATE))
            {
                Node *enumerate = Branch(old->value, NODE_ENUMERATE, old->file);

                for (; _Tokens->type != TKN_KEYWORD || strcmp(KEYWORD_END, _Tokens->value); Next(&_Tokens))
                {
                    if (_Tokens->type != TKN_NAME)
                    {
                        TreeFree(root);
                        TreeFree(enumerate);
                        return LeaveException(SyntaxError, old->value, old->file);
                    }
                    JoinBranch(enumerate, Branch(_Tokens->value, NODE_MEMCREATE, _Tokens->file));
                }

                JoinBranch(curr, enumerate);
                Next(&_Tokens);
            }

            else if (!strcmp(old->value, KEYWORD_IFERROR))
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

            else if (!strcmp(old->value, KEYWORD_IF) || !strcmp(old->value, KEYWORD_WHILE))
            {
                AST *content = ParseExpr(&_Tokens, 0xFF);

                if (content == NULL || _Tokens->type != TKN_KEYWORD || strcmp(_Tokens->value, "do"))
                {
                    TreeFree(root);
                    return LeaveException(SyntaxError, old->value, old->file);
                }

                AST *statement = Branch(old->value, strcmp(old->value, KEYWORD_IF) ? NODE_REPETITION : NODE_CONDITION, old->file);
                AST *body = Branch("BODY", NODE_BODY, old->file);

                JoinBranch(statement, content);
                JoinBranch(statement, body);
                JoinBranch(curr, statement);

                curr = body;
                Next(&_Tokens);
            }

            else if (!strcmp(old->value, KEYWORD_ORIF))
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

            else if (!strcmp(old->value, KEYWORD_FUNCTION))
            {
                if (_Tokens->type != TKN_TYPE || _Tokens->next->type != TKN_NAME || _Tokens->next->next->type != TKN_PARENL)
                {
                    TreeFree(root);
                    return LeaveException(SyntaxError, old->value, old->file);
                }

                AST *type = Branch(_Tokens->value, NODE_TYPE, _Tokens->file);

                Next(&_Tokens);

                AST *function = Branch(_Tokens->value, NODE_FUNCTION, old->file);
                AST *body = Branch("BODY", NODE_BODY, old->file);

                Next(&_Tokens);
                Next(&_Tokens);
                JoinBranch(function, type);
                JoinBranch(curr, function);

                while (1)
                {
                    if (_Tokens->type == TKN_PARENR)
                        break;

                    if (_Tokens->type != TKN_TYPE || _Tokens->next->type != TKN_NAME)
                    {
                        TreeFree(root);
                        TreeFree(body);
                        return LeaveException(SyntaxError, _Tokens->value, _Tokens->file);
                    }

                    JoinBranch(
                        //
                        function,
                        //
                        JoinBranch(
                            //
                            Branch(_Tokens->next->value, NODE_MEMCREATE, _Tokens->next->file),
                            Branch(_Tokens->value, NODE_TYPE, _Tokens->file)
                            //
                            )
                        //
                    );

                    Next(&_Tokens);
                    Next(&_Tokens);

                    if (_Tokens->type == TKN_SEMICOLON)
                        Next(&_Tokens);
                }

                JoinBranch(function, body);
                Next(&_Tokens);
                curr = body;
            }

            else if (!strcmp(old->value, KEYWORD_ELSE))
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

            else if (!strcmp(old->value, KEYWORD_WRITELN))
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

            else if (!strcmp(old->value, KEYWORD_CONTINUE) || !strcmp(old->value, KEYWORD_BREAK))
            {
                if (curr == root)
                {
                    TreeFree(root);
                    return LeaveException(UnexpectedNear, old->value, old->file);
                }

                JoinBranch(curr, Branch(old->value, strcmp(old->value, KEYWORD_CONTINUE) ? NODE_BREAK : NODE_CONTINUE, old->file));
            }

            else if (!strcmp(old->value, KEYWORD_END))
            {
                if (curr == root)
                {
                    TreeFree(root);
                    return LeaveException(UnexpectedNear, old->value, old->file);
                }

                curr = curr->parent->parent;
            }

            break;

        case TKN_TYPE:

            if (_Tokens->type != TKN_NAME)
            {
                TreeFree(root);
                return LeaveException(UnexpectedNear, old->value, old->file);
            }

            Node *mem = Branch(_Tokens->value, NODE_MEMCREATE, _Tokens->file);

            JoinBranch(mem, Branch(old->value, NODE_TYPE, old->file));
            JoinBranch(curr, mem);
            Next(&_Tokens);

            if (_Tokens->type != TKN_ASSIGN)
            {
                TreeFree(root);
                return LeaveException(MissingAssignation, old->value, old->file);
            }

            Next(&_Tokens);
            AST *content = ParseExpr(&_Tokens, 0xFF);

            if (content == NULL)
            {
                TreeFree(root);
                return LeaveException(SyntaxError, old->value, old->file);
            }

            JoinBranch(mem, content);
            break;

        case TKN_NAME:

            _Tokens = old;
            AST *parsed = ParseValue(&_Tokens);

            if (parsed == NULL)
            {
                TreeFree(root);
                return LeaveException(SyntaxError, old->value, old->file);
            }

            if (parsed->type == NODE_MEMGET)
            {
                if (_Tokens->type != TKN_ASSIGN)
                {
                    TreeFree(root);
                    TreeFree(parsed);
                    return LeaveException(UnexpectedNear, old->value, old->file);
                }

                Next(&_Tokens);
                AST *content = ParseExpr(&_Tokens, 0xFF);

                if (content == NULL)
                {
                    TreeFree(root);
                    TreeFree(parsed);
                    return LeaveException(SyntaxError, old->value, old->file);
                }

                parsed->type = NODE_MEMSET;
                JoinBranch(curr, JoinBranch(parsed, content));
            }
            else
                JoinBranch(curr, parsed);
            break;

        default:
            TreeFree(root);
            return LeaveException(UnexpectedNear, old->value, old->file);
        }
    }

    return (AST *)root;
}
