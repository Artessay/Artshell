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
#include <exception>

extern "C" 
{
    #include "lexer.h"
    int yy_lexer(int *argc, char ***argv);
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
    char ch;
    int i = 0;
    memset(input, 0, len);

    // 循环读入字符
    do
    {
        ch = getchar();

        if (ch == '\\') // 如果读到换行输入\命令就跳过继续
        {
            ch = getchar(); // 将随后的换行符读入
            continue;
        }

        if (ch == ';')  // 将；视为换行符，便于lexer和parser处理
        {
            ch = '\n';
        }
            
        input[i++] = ch;
    } while (ch != '\n');

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
        fprintf(stderr, "\e[1;31m[ERROR]\e[0m %s: %s\n", strerror(errno), "Out of Space for Console model");
        return 1;
    }

    Display *view = new Display(model);
    if (view == nullptr)
    {
        fprintf(stderr, "\e[1;31m[ERROR]\e[0m %s: %s\n", strerror(errno), "Out of Space for Display view");
        return 1;
    }

    try
    {
        while (1)
        {
            // 显示提示符
            view->render();

            // 从输入读入命令
            char input[BUFFER_SIZE];
            InputCommand(input, BUFFER_SIZE);

            

            // 从输入中创建buffer
            YY_BUFFER_STATE bp = yy_scan_string(input);
            if (bp == nullptr)
            {
                throw "Failed to create yy buffer state.";
            }

            yy_switch_to_buffer(bp);

            int argument_counter = 0;
            char **argument_vector = nullptr;

            yy_lexer(&argument_counter, &argument_vector);
            
            #ifdef _DEBUG_
            printf("argc: %d\n", argument_counter);
            for (int i = 0; i < argument_counter; ++i)
            {
                printf("%s ", argument_vector[i]);
            }
            putchar('\n');
            #endif

            if (strcmp(input, "exit\n") == 0)
                break;
        }
    }
    catch(const std::exception& e)
    {
        fprintf(stderr, "\e[1;31m[ERROR]\e[0m %s: %s\n", strerror(errno), e.what());
    }
    
    
    delete view;
    delete model;

    // 末尾输出判断程序是否正常结束，仅在调试时使用
    // puts("Bye~");

    return 0;
}