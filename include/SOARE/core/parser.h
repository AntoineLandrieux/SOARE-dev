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
 * Antoine LANDRIEUX (WTFPL) <parser.h>
 * <https://github.com/AntoineLandrieux/SOARE/>
 *
 * [!] Contribute and help me translate the comments!
 *
 */

/**
 * @brief Enumére les différents types de noeud
 * @author Antoine LANDRIEUX
 */
typedef enum node_type
{

    NODE_ROOT,
    NODE_NOP,
    NODE_TRY,
    NODE_BODY,
    NODE_TYPE,
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
    NODE_MEMCREATE,
    NODE_ENUMERATE,
    NODE_IFERROR,
    NODE_OPERATOR,
    NODE_CONDITION,
    NODE_REPETITION,
    NODE_RETURN,
    NODE_BREAK,
    NODE_CONTINUE

} node_type;

/**
 * @brief Structure d'un noeud
 * @author Antoine LANDRIEUX
 */
typedef struct Node
{
    
    // Valeur du noeud
    char *value;
    // Type du noeud
    node_type type;

    // Position du noeud dans le document
    Document file;

    // Noeud parent
    struct Node *parent;
    // Noeud enfant
    struct Node *child;
    // Noeud frère
    struct Node *sibling;

} Node;

/* Abstract syntax tree */
typedef Node AST;

/**
 * @brief Créer un nouveau noeud
 * @author Antoine LANDRIEUX
 * 
 * @param _Value 
 * @param _Type 
 * @param _File 
 * @return Node* 
 */
Node *Branch(char *_Value, node_type _Type, Document _File);

/**
 * @brief Lie 2 branches
 * @author Antoine LANDRIEUX
 * 
 * @param _Parent 
 * @param _Child 
 * @return AST* 
 */
AST *JoinBranch(Node *_Parent, Node *_Child);

/**
 * @brief Libère la mémoire allouée par le noeud et les noeuds lié avec lui
 * @author Antoine LANDRIEUX
 * 
 * @param _Tree 
 */
void TreeFree(AST *_Tree);

/**
 * @brief Affiche le noeud et les noeuds liés avec lui
 * @author Antoine LANDRIEUX
 * 
 * @param _Tree 
 */
void TreeLog(AST *_Tree);

/**
 * @brief Transforme une séquence de jetons en arbre (AST)
 * @author Antoine LANDRIEUX
 * 
 * @param _Tokens 
 * @return AST* 
 */
AST *Parse(Tokens *_Tokens);

#endif /* __SOARE_PARSER_H__ */