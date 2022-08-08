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
#include "Console.h"
#include "Display.h"
#include "Executor.h"

#include <fcntl.h>
#include <exception>

/** @brief 根据错误类型给出错误信息 */
static const char * shell_error_message(sh_err_t err);

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

                if (argument_counter == 0)
                    continue;

                int input_fd = model->GetInputFD();     // 记录下原始输入
                int output_fd = model->GetOutputFD();   // 记录下原始输出
                int error_fd = model->GetErrorFD();     // 记录下原始错误输出

                // 执行命令
                try
                {
                    // Parser 语法分析
                    Display::shell_parser(model, view, controller, argument_counter, argument_vector, env);

                    // 执行命令
                    sh_err_t err = controller->execute(argument_counter, argument_vector, env);
                    
                    // 根据返回状态判断
                    if (err == SH_EXIT)
                    {
                        break;
                    }
                    else if (err != SH_SUCCESS)
                    {
                        throw err;
                    }

                    view->show();   // 显示输出信息
                }
                catch(const std::exception& e)
                {
                    fprintf(stderr, "\e[1;31m[ERROR]\e[0m %s: %s\n", strerror(errno), e.what());
                }
                catch(const sh_err_t e)
                {
                    fprintf(stderr, "\e[1;31m[ERROR]\e[0m MyShell: %s\n", shell_error_message(e));
                }
                catch(const char * message)
                {
                    fprintf(stderr, "\e[1;31m[ERROR]\e[0m %s: %s\n", strerror(errno), message);
                }
                catch(...)
                {
                    fprintf(stderr, "\e[1;31m[ERROR]\e[0m %s\n", strerror(errno));
                }

                if (model->GetInputRedirect())    // 如果发生了输入重定向
                {
                    int state_code = close(model->GetInputFD()); // 关闭文件
                    if (state_code != 0)                         // 关闭错误处理
                        throw std::exception();

                    dup2(model->GetSTDIN(), STDIN_FILENO);
                    model->SetInputFD(input_fd);  // 恢复输入
                    model->ResetInputRedirect();    // 恢复状态
                }

                if (model->GetOutputRedirect())    // 如果发生了输出重定向
                {
                    int state_code = close(model->GetOutputFD()); // 关闭文件
                    if (state_code != 0)                          // 关闭错误处理
                        throw std::exception();

                    dup2(model->GetSTDOUT(), STDOUT_FILENO);
                    model->SetOutputFD(output_fd);  // 恢复输出
                    model->ResetOutputRedirect();    // 恢复状态
                }

                if (model->GetErrorRedirect())    // 如果发生了错误输出重定向
                {
                    int state_code = close(model->GetErrorFD()); // 关闭文件
                    if (state_code != 0)                          // 关闭错误处理
                        throw std::exception();

                    dup2(model->GetSTDERR(), STDERR_FILENO);
                    model->SetErrorFD(error_fd);  // 恢复错误输出
                    model->ResetErrorRedirect();    // 恢复状态
                }

                yylex_destroy();
            }
        }
        catch(const std::exception& e)
        {
            fprintf(stderr, "\e[1;31m[ERROR]\e[0m %s: %s\n", strerror(errno), e.what());
        };

        return 0;
    }

}

/** @brief 根据错误类型给出错误信息 */
static const char * shell_error_message(sh_err_t err)
{
    switch (err)
    {
        case SH_FAILED:
            return "Shell Failed. 错误";
        case SH_UNDEFINED:
            return "Undifined command. 未定义的命令";
        case SH_ARGS:
            return "Argument error. 参数错误";
        
        default:
            return "Unknown error. 未知错误";
    }
}