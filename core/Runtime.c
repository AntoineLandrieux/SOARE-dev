#include <DRIVER/keyboard.h>
#include <DRIVER/video.h>

#include <STD/stdlib.h>

/**
 *  _____  _____  ___  ______ _____
 * /  ___||  _  |/ _ \ | ___ \  ___|
 * \ `--. | | | / /_\ \| |_/ / |__
 *  `--. \| | | |  _  ||    /|  __|
 * /\__/ /\ \_/ / | | || |\ \| |___
 * \____/  \___/\_| |_/\_| \_\____/
 *
 * Antoine LANDRIEUX (MIT License) <Runtime.c>
 * <https://github.com/AntoineLandrieux/SOARE/>
 *
 */

#include <SOARE/SOARE.h>

static MEM FUNCTION = NULL;

/**
 * @brief Exit current statement #Runtime(AST)
 *
 * @param returns
 * @return char*
 */
static char *ExitStatement(MEM statement, char *returns)
{
    statement->next = NULL;
    return returns;
}

/**
 * @brief Exit current statement an error #Runtime(AST)
 *
 * @param error
 * @param string
 * @param file
 * @return void*
 */
static void *ExitStatementError(MEM statement, SoareExceptions error, char *string)
{
    return ExitStatement(statement, LeaveException(error, string));
}

/**
 * @brief Executes code from a tree
 *
 * @param tree
 * @return char*
 */
static char *Runtime(AST tree);

/**
 * @brief Set Interpreter variables
 *
 */
static void InterpreterVar()
{
    MEMORY = Mem();

    /* SOARE version */
    MemPush(MEMORY, "__SOARE__", SOARE_VERSION);
    /* Current file */
    MemPush(MEMORY, "__FILE__", "stdin");
    /* Path to SOARE executable */
    MemPush(MEMORY, "__ENVIRONMENT__", GetEnvironment());
    /* Errors */
    MemPush(MEMORY, "__ERROR__", "NoError");
    /* Build date */
    MemPush(MEMORY, "__BUILD__", __DATE__);
    /* Current OS */
    MemPush(MEMORY, "__PLATFORM__", "BORIUM");
}

/**
 * @brief Execute a function
 *
 * @param tree
 * @return char*
 */
char *RunFunction(AST tree)
{
    // Get the memory
    MEM get = MemGet(MEMORY, tree->value);

    // Memory not found
    if (!get)
        return LeaveException(UndefinedReference, tree->value);

    // Memory is not a function
    if (!get->body)
        return LeaveException(ObjectIsNotCallable, tree->value);

    // Arguments
    AST ptr = get->body->child;
    AST src = tree->child;
    MEM memf = Mem();

    AST func = NULL;

    while (ptr)
    {
        // Execute statement (no more argument required)
        if (ptr->type == NODE_BODY)
        {
            FUNCTION = memf;
            return Runtime(ptr);
        }

        // Not enough argument
        if (!src)
        {
            memf = NULL;
            return LeaveException(UndefinedReference, ptr->value);
        }

        get = NULL;

        // If it is a reference to a memory
        if (src->type == NODE_MEMGET)
            // If memory exists
            if ((get = MemGet(MEMORY, src->value)))
                // If memory is a function
                if ((func = get->body))
                    // Add this function in argument
                    get = MemPushf(memf, ptr->value, func);

        // If argument is not a function
        if (!get || !func)
            MemPush(memf, ptr->value, Math(src));

        // Next argument
        src = src->sibling;
        ptr = ptr->sibling;
    }

    return NULL;
}

static char broken = 0;

/**
 * @brief Executes code from a tree
 *
 * @param tree
 * @return char*
 */
static char *Runtime(AST tree)
{
    if (!tree)
        return NULL;

    AST root = tree;
    AST tmp = NULL;
    MEM get = NULL;

    char *returned = NULL;
    unsigned char error = 0;

    broken = 0;

    MEM statement = MemLast(MEMORY);
    MemJoin(statement, FUNCTION);
    FUNCTION = NULL;

    for (AST curr = root->child; curr && !ErrorLevel(); curr = curr->sibling)
    {
        switch (curr->type)
        {
        // Execute shell command
        case NODE_SHELL:

            Execute(Math(curr->child));

        // Store function into MEMORY
        case NODE_FUNCTION:

            MemPushf(statement, curr->value, curr);
            break;

        // User input
        case NODE_INPUT:

            if ((get = MemGet(MEMORY, curr->value)))
            {
                get->value = malloc(__SOARE_MAX_INPUT__);

                if (!get->value)
                    return ExitStatementError(statement, InterpreterError, "OUT OF MEMORY");
                
                GETS(get->value, __SOARE_MAX_INPUT__);
                break;
            }
            else
                return ExitStatementError(statement, UndefinedReference, curr->value);

        // Call a function
        case NODE_CALL:

            RunFunction(curr);
            break;

        // Push a new variable into MEMORY
        case NODE_MEMNEW:

            MemPush(statement, curr->value, Math(curr->child));
            break;

        // Set a value to a variable
        case NODE_MEMSET:

            if (!(get = MemGet(MEMORY, curr->value)))
                return ExitStatementError(statement, UndefinedReference, curr->value);

            if (get->body)
                return ExitStatementError(statement, VariableDefinedAsFunction, curr->value);

            MemSet(get, Math(curr->child));
            break;

        // Condition statement (if)
        case NODE_CONDITION:

            returned = Math(curr->child);
            tmp = curr->child;

            while (returned)
            {
                if (strcmp(returned, "0"))
                {
                    if ((returned = Runtime(tmp->sibling)) || broken)
                        return ExitStatement(statement, returned);

                    break;
                }

                if (!(tmp = tmp->sibling->sibling))
                    break;

                returned = Math(tmp);
            }
            break;

        // Repetition statement (while)
        case NODE_REPETITION:

            while ((returned = Math(curr->child)))
            {
                if (!strcmp(returned, "0") || ErrorLevel() || broken)
                    break;

                if ((returned = Runtime(curr->child->sibling)))
                    return ExitStatement(statement, returned);
            }

            break;

        // try/iferror statement
        case NODE_TRY:

            error = AsIgnoredException();
            IgnoreException(1);
            returned = Runtime(curr->child);
            IgnoreException(error);

            if (ErrorLevel() && !broken)
            {
                ClearException();
                returned = Runtime(curr->child->sibling);
            }

            if (!returned && !broken)
                break;

            return ExitStatement(statement, returned);

        // Print
        case NODE_OUTPUT:

            PUTS(Math(curr->child));
            PUTC('\n');
            break;

        // Break loop
        case NODE_BREAK:

            broken = 1;
            return ExitStatement(statement, NULL);

        // Return
        case NODE_RETURN:

            return ExitStatement(statement, Math(curr->child));

        // Leave new exception
        case NODE_RAISE:

            return ExitStatementError(statement, RaiseException, curr->value);

        default:

            break;
        }
    }

    // Quit
    return ExitStatement(statement, NULL);
}

/**
 * @brief Execute the code from a string
 *
 * @param rawcode
 */
int Execute(char *rawcode)
{
    if (!MEMORY)
        // Set default vars..
        InterpreterVar();

    // Clear interpreter exception
    ClearException();

    // Interpretation step 1: Tokenizer
    Tokens *tokens = Tokenizer(rawcode);
    // Interpretation step 2: Parser
    AST ast = Parse(tokens);

    // Interpretation step 3: Runtime
    Runtime(ast);

    // Set MEMORY to NULL
    MEMORY = NULL;

    // Return error level
    // 0: EXIT_SUCCESS
    // 1: EXIT_FAILURE
    return (int)ErrorLevel();
}