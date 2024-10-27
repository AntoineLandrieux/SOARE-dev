#include <stdio.h>
#include <stdlib.h>

#include <SOARE/SOARE.h>

static void __attribute__((constructor)) init(void)
{
    return;
}

int main(void)
{
    Tokens *token = Tokenizer("input", "@hello world'H!y'57.5 d4 4if");
    TokensLog(token);
    TokensFree(token);
    return EXIT_SUCCESS;
}

static void __attribute__((destructor)) kill(void)
{
    fprintf(stderr, "\nBye!\n");
}
