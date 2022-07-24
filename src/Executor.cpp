/**
 * @file Executor.cpp
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2022-07-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "Executor.h"

#include <string.h>
#include <assert.h>

Executor::Executor(Console *model, Display *view)
: console_(model), display_(view)
{
    assert(console_ != nullptr);
    assert(display_ != nullptr);
    return;
}

Executor::~Executor()
{
}

sh_err_t Executor::execute(const int argc, char * const argv[], char * const env[]) const
{
    if (argc == 0 && argv == nullptr)
        return SH_SUCCESS;  // 没有输入命令则无需处理
    else if (argc == 0 || argv == nullptr || argv[0] == nullptr)
    {
        assert(false);
        return SH_FAILED;   // 解析可能产生了错误
    }
    
    const char *op = argv[0];

    if (strcmp(op, "cd") == 0)
    {
        return execute_cd(argc, argv);
    }
    else if (strcmp(op, "pwd") == 0)
    {
        return execute_pwd(argc, argv);
    }
    else if (strcmp(op, "time") == 0)
    {
        return execute_time(argc, argv);
    }
    else if (strcmp(op, "clr") == 0)
    {
        return execute_clr(argc, argv);
    }
    else if (strcmp(op, "dir") == 0)
    {
        return execute_dir(argc, argv);
    }
    else if (strcmp(op, "set") == 0)
    {
        return execute_set(argc, argv);
    }
    else if (strcmp(op, "echo") == 0)
    {
        return execute_echo(argc, argv);
    }
    else if (strcmp(op, "help") == 0)
    {
        return execute_help(argc, argv);
    }
    else if (strcmp(op, "exit") == 0)
    {
        return execute_exit(argc, argv);
    }
    else if (strcmp(op, "env") == 0)
    {
        return execute_env(argc, argv, env);
    }

    return SH_SUCCESS;
}

sh_err_t Executor::execute_cd(const int argc, char * const argv[]) const
{
    assert(strcmp(argv[0], "cd") && "unexpected node type");
    return SH_SUCCESS;
}

sh_err_t Executor::execute_pwd(const int argc, char * const argv[]) const
{
    assert(strcmp(argv[0], "pwd") && "unexpected node type");
    return SH_SUCCESS;
}

sh_err_t Executor::execute_time(const int argc, char * const argv[]) const
{
    assert(strcmp(argv[0], "time") && "unexpected node type");
    return SH_SUCCESS;
}

sh_err_t Executor::execute_clr(const int argc, char * const argv[]) const
{
    assert(strcmp(argv[0], "clr") && "unexpected node type");
    return SH_SUCCESS;
}

sh_err_t Executor::execute_dir(const int argc, char * const argv[]) const
{
    assert(strcmp(argv[0], "dir") && "unexpected node type");
    return SH_SUCCESS;
}

sh_err_t Executor::execute_set(const int argc, char * const argv[]) const
{
    assert(strcmp(argv[0], "set") && "unexpected node type");
    return SH_SUCCESS;
}

sh_err_t Executor::execute_echo(const int argc, char * const argv[]) const
{
    assert(strcmp(argv[0], "echo") && "unexpected node type");
    return SH_SUCCESS;
}

sh_err_t Executor::execute_help(const int argc, char * const argv[]) const
{
    assert(strcmp(argv[0], "help") && "unexpected node type");
    return SH_SUCCESS;
}

sh_err_t Executor::execute_exit(const int argc, char * const argv[]) const
{
    assert(strcmp(argv[0], "exit") && "unexpected node type");
    return SH_EXIT;
}

sh_err_t Executor::execute_env(const int argc, char * const argv[], char * const env[]) const
{
    assert(strcmp(argv[0], "env") && "unexpected node type");
    
    // 输入输出重定向
    // if (output == 1)
    // {
    //     fp = fopen(outputFile, "w");
    // }
    // else if (append == 1)
    // {
    //     fp = fopen(outputFile, "a");
    // }
    
    // //if ouput or append then fprintf
    // if (output == 1 || append == 1)
    // {
    //     while(*env)
    //     {
    //         fprintf(fp,"%s\n", *env++);
    //     }
    //     fclose(fp);
    // }
    // //otherwise just print to screen
    // else
    // {
        while(*env)
        {
            char buffer[BUFFER_SIZE];
            snprintf(buffer, BUFFER_SIZE, "%s\n", *env++);
            display_->message(buffer);
        }
    // }

    return SH_SUCCESS;
}