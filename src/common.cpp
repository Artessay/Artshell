#include "common.h"

#include <stdio.h>

void Argument_Display(int argc, char*argv[])
{
    printf("argc: %d\n", argc);
    for (int i = 0; i < argc; ++i)
    {
        printf("%s ", argv[i]);
    }
    putchar('\n');
    return;
}