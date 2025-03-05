#include <DRIVER/memory.h>

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
 * @brief Create a new node
 * @author Antoine LANDRIEUX
 *
 * @param value
 * @param type
 * @return Node*
 */
Node *Branch(char *value, node_type type)
{
    Node *branch = (Node *)malloc(sizeof(Node));

    if (!branch)
        return __SOARE_OUT_OF_MEMORY();

    branch->value = value;
    branch->type = type;
    branch->parent = NULL;
    branch->child = NULL;
    branch->sibling = NULL;

    return branch;
}

/**
 * @brief Find a node
 * @author Antoine LANDRIEUX
 *
 * @param source
 * @param value
 * @param type
 * @return Node*
 */
Node *BranchFind(AST source, char *value, node_type type)
{
    if (!source)
        return NULL;
    if (!strcmp(value, !source->value ? "" : source->value) && source->type == type)
        return source;
    Node *R = BranchFind(source->sibling, value, type);
    Node *L = BranchFind(source->parent ? (source->parent->parent ? source->parent->parent->child : source->parent) : NULL, value, type);
    return R ? R : L;
}

/**
 * @brief Join 2 branches
 * @author Antoine LANDRIEUX
 *
 * @param parent
 * @param child
 * @return AST
 */
AST BranchJoin(Node *parent, Node *child)
{
    if (!child || !parent)
        return NULL;
    if (!parent->child)
        parent->child = child;
    else
    {
        Node *tmp = parent->child;
        while (tmp->sibling)
            tmp = tmp->sibling;
        tmp->sibling = child;
    }
    child->parent = parent;
    return parent;
}

/**
 * @brief Turns a sequence of tokens into a tree (AST)
 * @author Antoine LANDRIEUX
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
        TokenNext(&tokens);

        switch (old->type)
        {
        case TKN_SEMICOLON:
            break;

        case TKN_KEYWORD:

            if (*(old->value) == '@')
            {
                if (tokens->type != TKN_NAME || tokens->next->type != TKN_PARENL)
                    return LeaveException(SyntaxError, old->value);

                AST function = Branch(tokens->value, NODE_FUNCTION);
                AST body = Branch("BODY", NODE_BODY);

                TokenNext(&tokens);
                TokenNext(&tokens);
                BranchJoin(curr, function);

                while (1)
                {
                    if (tokens->type == TKN_PARENR)
                        break;

                    if (tokens->type != TKN_NAME)
                        return LeaveException(SyntaxError, tokens->value);

                    BranchJoin(function, Branch(tokens->value, NODE_MEMSET));
                    TokenNext(&tokens);

                    if (tokens->type == TKN_SEMICOLON)
                        TokenNext(&tokens);
                }

                BranchJoin(function, body);
                TokenNext(&tokens);
                curr = body;
            }

            else if (*(old->value) == '$')
            {
                if (tokens->type != TKN_NAME || tokens->next->type != TKN_ASSIGN)
                    return LeaveException(SyntaxError, old->value);

                old = tokens;
                TokenNext(&tokens);
                TokenNext(&tokens);
                AST content = ParseExpr(&tokens, 0xF);

                if (!content)
                    return LeaveException(SyntaxError, old->value);

                BranchJoin(curr, BranchJoin(Branch(old->value, NODE_MEMNEW), content));
            }

            else if (!strcmp(old->value, KEYWORD_RETURN))
                BranchJoin(curr, BranchJoin(Branch(old->value, NODE_RETURN), ParseExpr(&tokens, 0xF)));

            else if (!strcmp(old->value, KEYWORD_INPUTCH))
            {
                if (tokens->type != TKN_NAME)
                    return LeaveException(SyntaxError, old->value);

                BranchJoin(curr, Branch(tokens->value, NODE_INPUT));
                TokenNext(&tokens);
            }

            else if (!strcmp(old->value, KEYWORD_LOADIMPORT) || !strcmp(old->value, KEYWORD_RAISE))
            {
                if (tokens->type != TKN_STRING)
                    return LeaveException(SyntaxError, old->value);

                BranchJoin(curr, Branch(tokens->value, strcmp(old->value, KEYWORD_RAISE) ? NODE_IMPORT : NODE_RAISE));
                TokenNext(&tokens);
            }

            else if (!strcmp(old->value, KEYWORD_TRY))
            {
                Node *try = Branch(old->value, NODE_TRY);
                BranchJoin(try, Branch("BODY", NODE_BODY));
                BranchJoin(curr, try);

                curr = try->child;
            }

            else if (!strcmp(old->value, KEYWORD_ENUMERATE))
            {
                Node *enumerate = Branch(old->value, NODE_ENUMERATE);

                for (; tokens->type != TKN_KEYWORD || strcmp(KEYWORD_END, tokens->value); TokenNext(&tokens))
                {
                    if (tokens->type != TKN_NAME)
                        return LeaveException(SyntaxError, old->value);
                    BranchJoin(enumerate, Branch(tokens->value, NODE_MEMSET));
                }

                BranchJoin(curr, enumerate);
                TokenNext(&tokens);
            }

            else if (!strcmp(old->value, KEYWORD_IFERROR))
            {
                if (curr == root || curr->parent->type != NODE_TRY || curr->type == NODE_IFERROR)
                    return LeaveException(UnexpectedNear, old->value);

                Node *iferror = Branch(old->value, NODE_IFERROR);
                BranchJoin(curr->parent, iferror);

                curr = iferror;
            }

            else if (!strcmp(old->value, KEYWORD_IF) || !strcmp(old->value, KEYWORD_WHILE))
            {
                AST content = ParseExpr(&tokens, 0xF);

                if (!content || tokens->type != TKN_KEYWORD || strcmp(tokens->value, "do"))
                    return LeaveException(SyntaxError, old->value);

                AST statement = Branch(old->value, strcmp(old->value, KEYWORD_IF) ? NODE_REPETITION : NODE_CONDITION);
                AST body = Branch("BODY", NODE_BODY);

                BranchJoin(statement, content);
                BranchJoin(statement, body);
                BranchJoin(curr, statement);

                curr = body;
                TokenNext(&tokens);
            }

            else if (!strcmp(old->value, KEYWORD_OR))
            {
                if (curr->parent->type != NODE_CONDITION)
                    return LeaveException(UnexpectedNear, old->value);

                AST content = ParseExpr(&tokens, 0xF);

                if (!content || tokens->type != TKN_KEYWORD || strcmp(tokens->value, "do"))
                    return LeaveException(SyntaxError, old->value);

                AST body = Branch("BODY", NODE_BODY);

                BranchJoin(curr->parent, content);
                BranchJoin(curr->parent, body);

                curr = body;
                TokenNext(&tokens);
            }

            else if (!strcmp(old->value, KEYWORD_ELSE))
            {
                if (curr->parent->type != NODE_CONDITION)
                    return LeaveException(UnexpectedNear, old->value);

                AST body = Branch("BODY", NODE_BODY);

                BranchJoin(curr->parent, Branch("1", NODE_NUMBER));
                BranchJoin(curr->parent, body);

                curr = body;
            }

            else if (!strcmp(old->value, KEYWORD_WRITE))
            {
                AST content = ParseExpr(&tokens, 0xF);

                if (!content)
                    return LeaveException(SyntaxError, old->value);

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

            tokens = old;
            AST parsed = ParseValue(&tokens);

            if (!parsed)
                return LeaveException(SyntaxError, old->value);

            if (parsed->type == NODE_MEMGET)
            {
                if (tokens->type != TKN_ASSIGN)
                    return LeaveException(UnexpectedNear, old->value);

                TokenNext(&tokens);
                AST content = ParseExpr(&tokens, 0xF);
                if (!content)
                    return LeaveException(SyntaxError, old->value);
                parsed->type = NODE_MEMSET;
                BranchJoin(curr, BranchJoin(parsed, content));
                break;
            }
            BranchJoin(curr, parsed);
            break;

        default:
            return LeaveException(UnexpectedNear, old->value);
        }
    }

    return (AST)root;
}
