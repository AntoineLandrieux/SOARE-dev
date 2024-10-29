#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SOARE/SOARE.h>

static void __attribute__((constructor)) init(void)
{
}

int main(void)
{
    Tokens *tokens = Tokenizer("input", "hello world'H!y'57.5 d4 4if");
    TokensLog(tokens);
    TokensFree(tokens);
    
    Document doc;
    
    doc.file = (char*)"input";
    doc.ln = 1;
    doc.col = 1;

    Node *a = Branch(strdup("1"), NODE_ROOT, doc);
    Node *b = Branch(strdup("2"), NODE_ROOT, doc);
    Node *c = Branch(strdup("3"), NODE_ROOT, doc);
    
    JoinBranch(a, b);
    JoinBranch(a, c);

    TreeLog(a);
    TreeFree(a);

    return EXIT_SUCCESS;
}

static void __attribute__((destructor)) kill(void)
{
    fprintf(stderr, "\nBye!\n");
}
