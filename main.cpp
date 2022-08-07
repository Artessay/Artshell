// 程序：命令行编辑器
// 作者：邱日宏 3200105842

/**
 * @file main.cpp
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @brief 主函数
 * @version 0.1
 * @date 2022-08-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "myshell.h"

#include <stdio.h>

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