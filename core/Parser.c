#include <STD/stdlib.h>

/**
 *  _____  _____  ___  ______ _____
 * /  ___||  _  |/ _ \ | ___ \  ___|
 * \ `--. | | | / /_\ \| |_/ / |__
 *  `--. \| | | |  _  ||    /|  __|
 * /\__/ /\ \_/ / | | || |\ \| |___
 * \____/  \___/\_| |_/\_| \_\____/
 *
 * Antoine LANDRIEUX (MIT License) <Parser.c>
 * <https://github.com/AntoineLandrieux/SOARE/>
 *
 */

#include <SOARE/SOARE.h>

/**
 *  Example of AST (Abstract Syntax Tree) :
 *
 *
 *          (node A)
 *          /
 *      [child]
 *        /
 *    (node B)-[sibling]-(node C)-[sibling]-(node D)
 *
 * Vocabulary:
 *
 * - NULL : Non-existent node
 *
 *  ---
 *
 * - parent : Direct parent of the node
 *
 * Example :
 *
 *  (node A) is the parent of (node B)
 *  (node A) is the parent of (node C)
 *  (node A) is the parent of (node D)
 *  NULL is the parent of (node A)
 *
 *  ---
 *
 * - child : Direct child of the node
 *
 * Example :
 *
 *  (node B) is the child of (node A)
 *  (node A) is the child of NULL
 *  (node C) is the child of NULL
 *  (node D) is the child of NULL
 *
 *  - sibling : Direct sibling of the node
 *
 * Example :
 *
 *  NULL is the sibling of (node A)
 *  NULL is the sibling of (node B)
 *  (node B) is the sibling of (node C)
 *  (node C) is the sibling of (node D)
 *  NULL is the sibling of (node D)
 *
 */

/**
 * @brief Create a new node
 *
 * @param value
 * @param type
 * @param file
 * @return Node*
 */
Node *Branch(char *value, node_type type)
{
    Node *branch = (Node *)malloc(sizeof(Node));

    if (!branch)
        return __SOARE_OUT_OF_MEMORY();

    branch->value = !value ? NULL : vardup(value);
    branch->type = type;
    branch->parent = NULL;
    branch->child = NULL;
    branch->sibling = NULL;

    return branch;
}

/**
 * @brief Add a sibling branch
 *
 * @param source
 * @param element
 * @return AST
 */
AST BranchJuxtapose(Node *source, AST element)
{
    if (!source)
        return element;

    if (!element)
        return source;

    /**
     *
     *  source:
     *
     *    (a)
     *    /
     *  (b)-(c)
     *
     * element:
     *
     *    (d)
     *    /
     *  (e)-(f)
     *
     * returns:
     *
     *     (a)-(d)
     *    /     \
     *  (b)-(c) (e)-(f)
     *
     */

    AST tmp = source->sibling;
    source->sibling = element;

    for (; element->sibling; element = element->sibling)
    {
        element->parent = source->parent;
    }

    element->parent = source->parent;
    element->sibling = tmp;

    return source;
}

/**
 * @brief Join 2 branches
 *
 * @param parent
 * @param child
 * @return AST
 */
AST BranchJoin(Node *parent, Node *child)
{
    if (!child)
        return parent;

    if (!parent)
        return child;

    /**
     *
     *  parent:
     *
     *    (a)
     *    /
     *  (b)-(c)
     *
     * child:
     *
     *    (d)
     *    /
     *  (e)-(f)
     *
     * returns:
     *
     *     (a)
     *    /
     * (b)-(c)-(d)
     *         /
     *       (e)-(f)
     *
     */

    if (!parent->child)
    {
        parent->child = child;
    }

    else
    {
        Node *tmp = parent->child;

        while (tmp->sibling)
        {
            tmp = tmp->sibling;
        }

        tmp->sibling = child;
    }

    child->parent = parent;
    return parent;
}

/**
 * @brief Turns a sequence of tokens into a tree (AST)
 *
 * @param tokens
 * @return AST
 */
AST Parse(Tokens *tokens)
{
    Node *root = Branch("root", NODE_ROOT);
    Node *curr = root;

    while (tokens)
    {
        if (tokens->type == TKN_EOF)
            break;

        Tokens *old = tokens;
        tokens = tokens->next;

        switch (old->type)
        {
        case TKN_SEMICOLON:
            break;

        case TKN_KEYWORD:

            if (*(old->value) == '$')
            {
                AST content = ParseExpr(&tokens, 0xF);

                if (!content)
                    return LeaveException(ValueError, old->value);

                /**
                 *
                 *      \
                 *      (SHELL)
                 *         |
                 *       (code)
                 *
                 */

                BranchJoin(
                    //
                    curr,
                    BranchJoin(
                        //
                        Branch(old->value, NODE_SHELL),
                        content
                        //
                        )
                    //
                );
            }
            else if (!strcmp(old->value, KEYWORD_FN))
            {
                if (!TokensFollowPattern(tokens, 2, TKN_NAME, TKN_PARENL))
                    return LeaveException(SyntaxError, old->value);

                /**
                 *
                 *       \
                 *       (function)
                 *      /
                 *  (arg1)-(arg2)-(body)
                 *                   \...
                 *
                 */

                AST function = Branch(tokens->value, NODE_FUNCTION);
                TokenNext(&tokens, 2);
                BranchJoin(curr, function);

                while (1)
                {
                    if (tokens->type == TKN_PARENR)
                        break;

                    if (tokens->type != TKN_NAME)
                        return LeaveException(SyntaxError, tokens->value);

                    BranchJoin(
                        //
                        function,
                        Branch(tokens->value, NODE_MEMSET)
                        //
                    );

                    TokenNext(&tokens, 1 + (tokens->next->type == TKN_SEMICOLON));
                }

                AST body = Branch("BODY", NODE_BODY);
                BranchJoin(function, body);
                tokens = tokens->next;
                curr = body;
            }

            else if (!strcmp(old->value, KEYWORD_BREAK))
            {
                /**
                 *
                 *     \
                 *     (break)
                 *
                 */
                BranchJoin(curr, Branch(old->value, NODE_BREAK));
            }

            else if (!strcmp(old->value, KEYWORD_LET))
            {
                if (!TokensFollowPattern(tokens, 2, TKN_NAME, TKN_ASSIGN))
                    return LeaveException(SyntaxError, old->value);

                TokenNext(&tokens, 2);
                AST content = ParseExpr(&tokens, 0xF);

                if (!content)
                    return LeaveException(ValueError, old->value);

                    /**
                 *
                 *     \
                 *     (varname)
                 *         |
                 *      (value)
                 */

                BranchJoin(
                    //
                    curr,
                    BranchJoin(
                        //
                        Branch(old->next->value, NODE_MEMNEW),
                        content
                        //
                        )
                    //
                );
            }

            else if (!strcmp(old->value, KEYWORD_RETURN))
            {

                /**
                 *
                 *     \
                 *     (return)
                 *         |
                 *      (value)
                 */

                BranchJoin(curr, BranchJoin(Branch(old->value, NODE_RETURN), ParseExpr(&tokens, 0xF)));
            }

            else if (!strcmp(old->value, KEYWORD_INPUT))
            {
                if (tokens->type != TKN_NAME)
                    return LeaveException(ValueError, old->value);

                /**
                 *
                 *      \
                 *      (input)
                 *         |
                 *     (varname)
                 *
                 */

                BranchJoin(curr, Branch(tokens->value, NODE_INPUT));
                tokens = tokens->next;
            }

            else if (!strcmp(old->value, KEYWORD_RAISE))
            {
                if (tokens->type != TKN_STRING)
                    return LeaveException(SyntaxError, old->value);

                /**
                 *
                 *      \
                 *    (loadimport)
                 *         |
                 *       (path)
                 *
                 */

                BranchJoin(curr, Branch(tokens->value, NODE_RAISE));
                tokens = tokens->next;
            }

            else if (!strcmp(old->value, KEYWORD_TRY))
            {
                Node *try = Branch(old->value, NODE_TRY);
                BranchJoin(try, Branch("BODY", NODE_BODY));
                BranchJoin(curr, try);

                /**
                 *
                 *      \
                 *     (try)
                 *       |...
                 *
                 */

                curr = try->child;
            }

            else if (!strcmp(old->value, KEYWORD_IFERROR))
            {
                if (curr == root || curr->parent->type != NODE_TRY || curr->type == NODE_IFERROR)
                    return LeaveException(UnexpectedNear, old->value);

                /**
                 *
                 *      /
                 *  (try)-(iferror)
                 *    |...    |...
                 *
                 */

                Node *iferror = Branch(old->value, NODE_IFERROR);
                BranchJoin(curr->parent, iferror);
                curr = iferror;
            }

            else if (!strcmp(old->value, KEYWORD_IF) || !strcmp(old->value, KEYWORD_WHILE))
            {
                AST content = ParseExpr(&tokens, 0xF);

                if (!content)
                    return LeaveException(ValueError, old->value);

                if (tokens->type != TKN_KEYWORD || strcmp(tokens->value, "do"))
                    return LeaveException(SyntaxError, old->value);

                /**
                 *
                 *
                 *           (while/if)
                 *           /
                 *   (condition)-(body)
                 * .../             \...
                 *
                 */

                AST statement = Branch(old->value, strcmp(old->value, KEYWORD_IF) ? NODE_REPETITION : NODE_CONDITION);
                AST body = Branch("BODY", NODE_BODY);

                BranchJoin(statement, content);
                BranchJoin(statement, body);
                BranchJoin(curr, statement);

                curr = body;
                tokens = tokens->next;
            }

            else if (!strcmp(old->value, KEYWORD_OR))
            {
                if (curr->parent->type != NODE_CONDITION)
                    return LeaveException(UnexpectedNear, old->value);

                AST content = ParseExpr(&tokens, 0xF);

                if (!content)
                    return LeaveException(ValueError, old->value);

                if (tokens->type != TKN_KEYWORD || strcmp(tokens->value, "do"))
                    return LeaveException(SyntaxError, old->value);

                /**
                 *
                 *                /
                 *             (if)---------------(or)
                 *             /                  /
                 *   (condition)--(body) (condition)--(body)
                 *        |...       |...     |...       |...
                 *
                 */

                AST body = Branch("BODY", NODE_BODY);

                BranchJoin(curr->parent, content);
                BranchJoin(curr->parent, body);

                curr = body;
                tokens = tokens->next;
            }

            else if (!strcmp(old->value, KEYWORD_ELSE))
            {
                if (curr->parent->type != NODE_CONDITION)
                    return LeaveException(UnexpectedNear, old->value);

                /**
                 *
                 *                /
                 *             (if)--------(else)
                 *             /            /
                 *   (condition)--(body)  (1)--(body)
                 *                   |...        |...
                 *
                 */

                AST body = Branch("BODY", NODE_BODY);

                BranchJoin(curr->parent, Branch("1", NODE_VALUE));
                BranchJoin(curr->parent, body);

                curr = body;
            }

            else if (!strcmp(old->value, KEYWORD_WRITE))
            {
                AST content = ParseExpr(&tokens, 0xF);

                if (!content)
                    return LeaveException(ValueError, old->value);

                /**
                 *
                 *      \
                 *      (write)
                 *         |
                 *      (value)
                 *
                 */

                BranchJoin(curr, BranchJoin(Branch(old->value, NODE_OUTPUT), content));
            }

            else if (!strcmp(old->value, KEYWORD_END))
            {
                if (curr == root)
                    return LeaveException(UnexpectedNear, old->value);

                curr = curr->parent->parent;
            }

            break;

        case TKN_NAME:

            if (tokens->type != TKN_PARENL)
            {
                if (tokens->type != TKN_ASSIGN)
                    return LeaveException(UnexpectedNear, old->value);

                tokens = tokens->next;
                AST content = ParseExpr(&tokens, 0xF);

                if (!content)
                    return LeaveException(ValueError, old->value);

                /**
                 *
                 *      \
                 *     (varname)
                 *         |
                 *      (value)
                 *
                 */

                BranchJoin(
                    //
                    curr,
                    BranchJoin(
                        //
                        Branch(old->value, NODE_MEMSET),
                        content
                        //
                        )
                    //
                );
                break;
            }

            tokens = old;
            AST call = ParseValue(&tokens);

            if (!call)
                return LeaveException(ValueError, old->value);

            /**
             *
             *      \
             *    (function name)
             *      /
             *  (arg1)-(arg2)-(arg...)
             *
             */

            BranchJoin(curr, call);
            break;

        default:
            return LeaveException(UnexpectedNear, old->value);
        }
    }

    if (ErrorLevel())
        return NULL;

    return (AST)root;
}