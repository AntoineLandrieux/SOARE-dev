#ifndef __SOARE_PARSER_H__
#define __SOARE_PARSER_H__ 0x1

/**
 * 
 */

/**
 * @brief 
 * 
 */
typedef enum node_type
{

    NODE_ROOT,
    NODE_IMPORT,

} node_type;

/**
 * @brief 
 * 
 */
typedef struct Node
{
    
    char *value;
    node_type type;

    Document file;

    struct Node *parent;
    struct Node *child;
    struct Node *sibling;

} Node;

/*  */
typedef Node AST;

/**
 * @brief 
 * 
 * @param _Value 
 * @param _Type 
 * @param _File 
 * @return Node* 
 */
Node *Branch(char *_Value, node_type _Type, Document _File);

/**
 * @brief 
 * 
 * @param _Parent 
 * @param _Child 
 */
void JoinBranch(Node *_Parent, Node *_Child);

/**
 * @brief 
 * 
 * @param _Tree 
 */
void TreeFree(AST *_Tree);

/**
 * @brief 
 * 
 * @param _Tree 
 */
void TreeLog(AST *_Tree);

#endif /* __SOARE_PARSER_H__ */