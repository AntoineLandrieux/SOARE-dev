#ifndef __SOARE_PARSER_H__
#define __SOARE_PARSER_H__ 0x1

/* #prama once */

/**
 *  _____  _____  ___  ______ _____
 * /  ___||  _  |/ _ \ | ___ \  ___|
 * \ `--. | | | / /_\ \| |_/ / |__
 *  `--. \| | | |  _  ||    /|  __|
 * /\__/ /\ \_/ / | | || |\ \| |___
 * \____/  \___/\_| |_/\_| \_\____/
 *
 * Antoine LANDRIEUX (MIT License) <parser.h>
 * <https://github.com/AntoineLandrieux/SOARE/>
 *
 */

/**
 * @brief List the different types of nodes
 * @author Antoine LANDRIEUX
 */
typedef enum node_type
{

    NODE_ROOT,
    NODE_TRY,
    NODE_BODY,
    NODE_RAISE,
    NODE_ARRAY,
    NODE_OUTPUT,
    NODE_NUMBER,
    NODE_STRING,
    NODE_IMPORT,
    NODE_CALL,
    NODE_FUNCTION,
    NODE_MEMGET,
    NODE_MEMSET,
    NODE_ENUMERATE,
    NODE_IFERROR,
    NODE_OPERATOR,
    NODE_CONDITION,
    NODE_REPETITION,
    NODE_RETURN,
    NODE_CONTINUE

} node_type;

/**
 * @brief Structure of a node
 * @author Antoine LANDRIEUX
 */
typedef struct node
{

    // Value
    char *value;
    // Type
    node_type type;

    // Document
    Document file;

    // Node Parent
    struct node *parent;
    // Node Child
    struct node *child;
    // Node Sibling
    struct node *sibling;

} Node, *AST;

/**
 * @brief Create a new node
 * @author Antoine LANDRIEUX
 *
 * @param _Value
 * @param _Type
 * @param _File
 * @return Node*
 */
Node *Branch(char *_Value, node_type _Type, Document _File);

/**
 * @brief Find a node
 * @author Antoine LANDRIEUX
 *
 * @param _Source
 * @param _Value
 * @param _Type
 * @return Node*
 */
Node *BranchFind(AST _Source, char *_Value, node_type _Type);

/**
 * @brief Join 2 branches
 * @author Antoine LANDRIEUX
 *
 * @param _Parent
 * @param _Child
 * @return AST
 */
AST BranchJoin(Node *_Parent, Node *_Child);

/**
 * @brief Frees the memory allocated by a tree
 * @author Antoine LANDRIEUX
 *
 * @param _Tree
 */
void TreeFree(AST _Tree);

/**
 * @brief Display a tree
 * @author Antoine LANDRIEUX
 *
 * @param _Tree
 */
void TreeLog(AST _Tree);

/**
 * @brief Turns a sequence of tokens into a tree (AST)
 * @author Antoine LANDRIEUX
 *
 * @param _Tokens
 * @return AST
 */
AST Parse(Tokens *_Tokens);

#endif /* __SOARE_PARSER_H__ */