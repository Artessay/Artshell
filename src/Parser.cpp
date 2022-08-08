/**
 * @file Parser.cpp
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @brief 语法分析
 * @version 0.1
 * @date 2022-07-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "Parser.h"
#include "Console.h"
#include "Display.h"
#include "Executor.h"

#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <exception>

/** @brief 根据错误类型给出错误信息 */
static const char * shell_error_message(sh_err_t err);

int Parser::shell_pipe(Console *model, Display* view, Executor* controller, int& argc, char *argv[], char *env[])
{
    int count = 0;
    char **args = argv;

    int i = 0;
    do
    {
        if (strcmp(argv[i], "|") != 0)   // 不是管道符
        {
            count++;
        }
        else
        {
            shell_execute(model, view, controller, count, args, env);
            count = 0;
            args = argv + i + 1;
        }

        ++i;
    } while (i < argc);

    return 0;
}

int Parser::shell_parser(Console *model, Display* view, Executor* controller, int& argc, char *argv[], char *env[])
{
    if (argc == 0)
        return 0;   // 无参，不需处理
    
    for (int index = argc-1; index > 0; --index)    // 从末尾开始往前扫描，第一个不必扫
    {
        std::string arg(argv[index]);    // 使用string类处理
        
        /** 重定向处理 */
        
        /** 标准输入重定向 */
        if (arg == "<" || arg == "0<")
        {
            if (index + 1 == argc)  // 重定向符号是最后一个输入
            {
                throw "语法解析错误";
            }

            if (model->GetInputRedirect())      // 如果已经设置了重定向状态了
            {
                throw "多重重定向错误";
            }

            const char * input_file = argv[index + 1];
            int fd = open(input_file, O_RDONLY);
            if (fd < 0)
                throw std::exception();
                            
            model->SetInputFD(fd);
            model->SetInputRedirect();

            dup2(fd, STDIN_FILENO); // 重定向标准输入至fd

            for (int jump = index + 2; jump < argc; ++jump)
                argv[jump-2] = argv[jump];
            argc = argc - 2;
            argv[argc] = NULL;
        }

        /** 标准输出重定向 */
        if (arg == ">" || arg == "1>")
        {
            if (index + 1 == argc)  // 重定向符号是最后一个输入
            {
                throw "语法解析错误";
            }

            if (model->GetOutputRedirect())      // 如果已经设置了重定向状态了
            {
                throw "多重重定向错误";
            }

            const char * output_file = argv[index + 1];
            int fd = open(output_file, O_WRONLY | O_TRUNC | O_CREAT, 0777&(~model->GetMask()));
            if (fd < 0)
                throw std::exception();                
            
            model->SetOutputFD(fd);
            model->SetOutputRedirect();

            dup2(fd, STDOUT_FILENO); // 重定向标准输出至fd
                
            for (int jump = index + 2; jump < argc; ++jump)
                argv[jump-2] = argv[jump];
            argc = argc - 2;
            argv[argc] = NULL;
        }

        /** 标准错误输出重定向 */
        if (arg == "2>")
        {
            if (index + 1 == argc)  // 重定向符号是最后一个输入
            {
                throw "语法解析错误";
            }

            if (model->GetErrorRedirect())      // 如果已经设置了重定向状态了
            {
                throw "多重重定向错误";
            }

            const char * output_file = argv[index + 1];
            int fd = open(output_file, O_WRONLY | O_TRUNC | O_CREAT, 0777&(~model->GetMask()));
            if (fd < 0)
                throw std::exception();                
            
            model->SetErrorFD(fd);
            model->SetErrorRedirect();

            dup2(fd, STDERR_FILENO); // 重定向标准错误输出至fd

            for (int jump = index + 2; jump < argc; ++jump)
                argv[jump-2] = argv[jump];
            argc = argc - 2;
            argv[argc] = NULL;
        }

        /** 追加 */
        if (arg == ">>" || arg == "1>>")
        {
            /* 词法解析时应该识别<和>符号的闭包 */
            #ifdef _DEBUG_
            Argument_Display(argc, argv);
            #endif
            
            if (index + 1 == argc)  // 重定向符号是最后一个输入
            {
                throw "语法解析错误";
            }

            if (model->GetOutputRedirect())      // 如果已经设置了重定向状态了
            {
                throw "多重重定向错误";
            }

            const char * output_file = argv[index + 1];
            int fd = open(output_file, O_WRONLY | O_APPEND | O_CREAT, 0777&(~model->GetMask()));
            if (fd < 0)
                throw std::exception();                
            
            model->SetOutputFD(fd);
            model->SetOutputRedirect();

            dup2(fd, STDOUT_FILENO); // 重定向标准输出至fd

            for (int jump = index + 2; jump < argc; ++jump)
                argv[jump-2] = argv[jump];
            argc = argc - 2;
            argv[argc] = NULL;
        }
    }

    return 0;
}

bool Parser::shell_execute(Console *model, Display* view, Executor* controller, int& argc, char *argv[], char *env[])
{
    int input_fd = model->GetInputFD();     // 记录下原始输入
    int output_fd = model->GetOutputFD();   // 记录下原始输出
    int error_fd = model->GetErrorFD();     // 记录下原始错误输出

    // 执行命令
    try
    {
        // Parser 语法分析
        Parser::shell_parser(model, view, controller, argc, argv, env);

        // 执行命令
        sh_err_t err = controller->execute(argc, argv, env);
        
        // 根据返回状态判断
        if (err == SH_EXIT)
        {
            return false;
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

    return true;
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