// 程序：命令行编辑器
// 作者：邱日宏 3200105842

/**
 * @file myshell.cpp
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @brief myshell程序的main函数，负责调用各个接口并实现myshell功能
 * @version 0.1
 * @date 2022-07-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#define _DEBUG_

#include "myshell.h"

#include <string.h>
#include <string>
#include <iostream>

extern "C" 
{
    #include "lexer.h"
}

// 获取系统环境变量
// void env(char **environment)
// {
//     // 输入输出重定向
//     if (output == 1)
//     {
//         fp = fopen(outputFile, "w");
//     }
//     else if (append == 1)
//     {
//         fp = fopen(outputFile, "a");
//     }
    
//     //if ouput or append then fprintf
//     if (output == 1 || append == 1)
//     {
//         while(*environment)
//         {
//             fprintf(fp,"%s\n", *environment++);
//         }
//         fclose(fp);
//     }
//     //otherwise just print to screen
//     else
//     {
//         while(*environment)
//         {
//             printf("%s\n", *environment++);
//         }
//     }  
// }

/** @brief 命令行输入控制 */
void InputCommand(char *input, const int len) 
{
    // 初始化输入缓冲器与相关变量
    int i = 0;
    char ch = getchar();
    memset(input, 0, len);

    // 循环读入字符
    while (ch != ';' && ch != '\n')
    {
        if (ch == '\\') //  如果读到换行输入\命令就跳过继续
        {
            ch = getchar(); // 将随后的换行符读入
            continue;
        }
            
        input[i++] = ch;
        ch = getchar();
    }

    #ifdef _DEBUG_
    printf("input: %s", input);
    #endif
}

int main(int argc, char *argv[], char **env)
{
    // puts("Welcome to MyShell ! \n");
    Console *model = new Console();
    if (model == nullptr)
    {
        std::cerr << "Out of Space for Console model" << std::endl;
        return 1;
    }

    Display *view = new Display(model);

    try
    {
        while (1)
        {
            view->render(); // 显示提示符

            char input[BUFFER_SIZE];
            InputCommand(input, BUFFER_SIZE);

            int argument_counter = 0;
            char **argument_vector = nullptr;

            yy_lexer(argument_counter, argument_vector);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    
    delete view;
    delete model;

    
    // 末尾输出判断程序是否正常结束，仅在调试时使用
    // puts("Bye~");

    return 0;
}