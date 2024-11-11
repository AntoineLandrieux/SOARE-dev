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
 */
void JoinBranch(Node *_Parent, Node *_Child)
{
    if (_Parent == NULL || _Child == NULL)
        return;
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
            _Tree->parent,
            _Tree->parent->file.file,
            _Tree->parent->file.ln,
            _Tree->parent->file.col,
            _Tree->parent->type,
            _Tree->parent->value);
    printf(
        "[%p, %s:%.5lld:%.5lld, %.2X, \"%s\"]\n",
        _Tree,
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
            
            else if (!strcmp(old->value, "loadimport") || !strcmp(old->value, "raise"))
            {
                if (_Tokens->type != TKN_STRING)
                {
                    TreeFree(root);
                    return LeaveException(CharacterError, old->value, old->file);
                }
                JoinBranch(curr, Branch(_Tokens->value, strcmp(old->value, "raise") ? NODE_IMPORT : NODE_RAISE, old->file));
                Next(&_Tokens);
            }

            else if (!strcmp(old->value, "try"))
            {
                Node *try = Branch(old->value, NODE_TRY, old->file);
                JoinBranch(try, Branch("body", NODE_BODY, old->file));
                JoinBranch(curr, try);
                curr = try->child;
            }

            else if (!strcmp(old->value, "iferror"))
            {
                if (curr->parent->type != NODE_TRY || curr->type == NODE_IFERROR)
                {
                    TreeFree(root);
                    return LeaveException(CharacterError, old->value, old->file);
                }
                Node *iferror = Branch(old->value, NODE_IFERROR, old->file);
                JoinBranch(curr->parent, iferror);
                curr = iferror;
            }
            
            else if (!strcmp(old->value, "close"))
            {
                if (curr->parent != root)
                    curr = curr->parent->parent;
            }
            
            break;

        default:
            TreeFree(root);
            return LeaveException(CharacterError, old->value, old->file);
        }
    }

    return (AST *)root;
}
