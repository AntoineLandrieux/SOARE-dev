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

static void __attribute__((constructor)) init(void)
{
    asm("nop");
}

int main(void)
{
    // TODO: Fix ParseExpr ?
    // Why ??
    Tokens *tokens = Tokenizer(NULL, "writeln 4+5*2 ? Hello");
    TokensLog(tokens);

    AST *ast = Parse(tokens);
    TreeLog(ast);

    Runtime(ast);

    TreeFree(ast);
    TokensFree(tokens);

    return EXIT_SUCCESS;
}

static void __attribute__((destructor)) kill(void)
{
    fprintf(stderr, "\nBye!\n");
}
