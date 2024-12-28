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
    return Execute(
"\
? BD\n \
if 0 do \
    writeln 'A' \
orif 1 do \
    writeln 'B' \
else \
    writeln 'C' \
end \
writeln 'D' \
");
}

static void __attribute__((destructor)) kill(void)
{
    fprintf(stderr, "\nBye!\n");
}
