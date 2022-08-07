#include "myshell.h"

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[], char **env)
{
    // 开头输出判断程序是否正常开始，仅在调试时使用
    // puts("Welcome to MyShell ! \n");

    if (SHELL::shell_setup(argc, argv, env) != 0)
        puts("shell setup failed."); 

    // 末尾输出判断程序是否正常结束，仅在调试时使用
    // puts("Bye~");

    return 0;
}