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
 * @param _Value
 * @param _Type
 * @param _File
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
            "[0x%p, %s:%.5lld:%.5lld, %.2X, \"%s\"]\t",
            _Tree->parent,
            _Tree->parent->file.file,
            _Tree->parent->file.ln,
            _Tree->parent->file.col,
            _Tree->parent->type,
            _Tree->parent->value);
    printf(
        "[0x%p, %s:%.5lld:%.5lld, %.2X, \"%s\"]\n",
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
 * 
 * @param _Tokens 
 */
static void Next(Tokens **_Tokens)
{
    *_Tokens = (*_Tokens)->next;
}

/**
 * @brief 
 * 
 * @return AST* 
 */
AST *Parse(Tokens *_Tokens)
{
    Node *root = Branch("root", NODE_ROOT, EmptyDocument());
    Node *curr = root;

    while (_Tokens != NULL)
    {
        Tokens *old = _Tokens;
        Next(_Tokens);

        switch (_Tokens->type)
        {
        case TKN_KEYWORD:
            if (!strcmp(old->value, "loadimport"))
            {
                if (_Tokens->type != TKN_STRING)
                {
                    TreeFree(root);
                    return LeaveException(ERR_CHARACTER, old->value, old->file);
                }
                JoinBranch(curr, Branch(_Tokens->value, NODE_IMPORT, old->file));
                Next(_Tokens);
            }
            break;
        
        default:
            break;
        }
    }

    return (AST *)root;
}
