#ifndef __SOARE_PARSER_H__
#define __SOARE_PARSER_H__ 0x1

/**
 *  _____  _____  ___  ______ _____
 * /  ___||  _  |/ _ \ | ___ \  ___|
 * \ `--. | | | / /_\ \| |_/ / |__ 
 *  `--. \| | | |  _  ||    /|  __|
 * /\__/ /\ \_/ / | | || |\ \| |___
 * \____/  \___/\_| |_/\_| \_\____/
 */

/**
 * @brief 
 * 
 */
typedef enum node_type
{

    NODE_ROOT,
    NODE_NOP,
    NODE_TRY,
    NODE_BODY,
    NODE_TYPE,
    NODE_RAISE,
    NODE_OUTPUT,
    NODE_NUMBER,
    NODE_STRING,
    NODE_IMPORT,
    NODE_MEMGET,
    NODE_MEMSET,
    NODE_IFERROR,
    NODE_OPERATOR,
    NODE_CONDITION,
    NODE_REPETITION,
    NODE_RETURN,
    NODE_BREAK,
    NODE_CONTINUE,
    NODE_ARRAY,
    NODE_CALL

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
 * @return AST* 
 */
AST *JoinBranch(Node *_Parent, Node *_Child);

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

/**
 * @brief 
 * 
 * @param _Tokens 
 * @return AST* 
 */
AST *Parse(Tokens *_Tokens);

#endif /* __SOARE_PARSER_H__ */