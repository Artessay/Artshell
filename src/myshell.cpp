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

// #define _DEBUG_

extern "C" 
{
    #include "lexer.h"
    int yy_lexer(int *argc, char ***argv);
}

#include "myshell.h"
#include "common.h"
#include "Parser.h"
#include "Console.h"
#include "Display.h"
#include "Executor.h"

#include <exception>


namespace SHELL
{
    int shell_setup(int argc, char *argv[], char *env[])
    {
        // 创建模型
        Console *model = new Console;
        if (model == nullptr)
        {
            fprintf(stderr, "\e[1;31m[ERROR]\e[0m %s: %s\n", strerror(errno), "Out of Space for Console model");
            return 1;
        }

        // 创建视图
        Display *view = new Display(model);
        if (view == nullptr)
        {
            fprintf(stderr, "\e[1;31m[ERROR]\e[0m %s: %s\n", strerror(errno), "Out of Space for Display view");
            return 1;
        }

        // 创建控制
        Executor *controller = new Executor(model, view);
        if (controller == nullptr)
        {
            fprintf(stderr, "\e[1;31m[ERROR]\e[0m %s: %s\n", strerror(errno), "Out of Space for Executor controller");
            return 1;
        }

        SHELL::shell_loop(model, view, controller, env);
        
        // 回收内存，MVC模型
        delete model;
        delete view;
        delete controller;

        return 0;
    }

    int shell_loop(Console* model, Display* view, Executor* controller, char *env[])
    {
        try
        {
            while (1)
            {
                // 显示提示符
                view->render();

                // 从输入读入命令
                char input[BUFFER_SIZE];
                int input_len = view->InputCommand(input, BUFFER_SIZE);
                
                if (input_len == 0)   // 输入完毕
                    return 0;
                if (input_len < 0)   // 输入异常
                    continue;

                // 从输入中创建buffer
                YY_BUFFER_STATE bp = yy_scan_string(input);
                if (bp == nullptr)
                {
                    throw "Failed to create yy buffer state.";
                }

                yy_switch_to_buffer(bp);

                // 进行分词解析处理
                int argument_counter = 0;
                char **argument_vector = nullptr;
                yy_lexer(&argument_counter, &argument_vector);
                
                #ifdef _DEBUG_
                Argument_Display(argument_counter, argument_vector);
                #endif

                /* 输出完成的进程，即使是空指令也应如此。 */
                model->ConsoleJobListDone();

                if (argument_counter == 0)
                    continue;
                
                bool exit_state = Parser::shell_pipe(model, view, controller, argument_counter, argument_vector, env);

                // view->show();   // 显示输出信息

                yylex_destroy();    // 释放词法分析器占用的空间，防止内存泄露

                if (exit_state == true)
                    break;
            }
        }
        catch(const char * message)
        {
            fprintf(stderr, "\e[1;31m[ERROR]\e[0m %s: %s\n", strerror(errno), message);
        }
        catch(const std::exception& e)
        {
            fprintf(stderr, "\e[1;31m[ERROR]\e[0m %s: %s\n", strerror(errno), e.what());
        }
        catch(...)
        {
            fprintf(stderr, "\e[1;31m[ERROR]\e[0m %s\n", strerror(errno));
        }

        return 0;
    }

}