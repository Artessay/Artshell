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

#include <unistd.h>
#include <string.h>
#include <assert.h>

/*定义所有SHELL支持的命令合集*/
// const std::pair<const char *, sh_err_t (Executor::*)(const int argc, char * const argv[], char * const env[]) const> Executor::FunctionArray[] = 
// {
//     std::make_pair("cd",    execute_cd  ),
//     std::make_pair("pwd",   execute_pwd ),
//     std::make_pair("time",  execute_time),
//     std::make_pair("clr",   execute_clr ),
//     std::make_pair("dir",   execute_dir ),
//     std::make_pair("set",   execute_set ),
//     std::make_pair("echo",  execute_echo),
//     std::make_pair("help",  execute_help),
//     std::make_pair("exit",  execute_exit),
//     std::make_pair("date",  execute_date),
//     std::make_pair("env",   execute_env ),
//     std::make_pair("who",   execute_who ),
// };

/*定义映射*/
// const std::map<const char *, sh_err_t (Executor::*)(const int argc, char * const argv[], char * const env[]) const> Executor::FunctionMap(
//     FunctionArray, FunctionArray + sizeof(FunctionArray)/sizeof(FunctionArray[0])
// );

Executor::Executor(Console *model, Display *view)
: console_(model), display_(view)
{
    assert(console_ != nullptr);
    assert(display_ != nullptr);

    FunctionMap.insert(std::make_pair("cd",    execute_cd  ));
    std::make_pair("pwd",   execute_pwd ),
    std::make_pair("time",  execute_time),
    std::make_pair("clr",   execute_clr ),
    std::make_pair("dir",   execute_dir ),
    std::make_pair("set",   execute_set ),
    std::make_pair("echo",  execute_echo),
    std::make_pair("help",  execute_help),
    std::make_pair("exit",  execute_exit),
    std::make_pair("date",  execute_date),
    std::make_pair("env",   execute_env ),
    std::make_pair("who",   execute_who )

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
        return execute_cd(argc, argv, env);
    }
    else if (strcmp(op, "pwd") == 0)
    {
        return execute_pwd(argc, argv, env);
    }
    else if (strcmp(op, "time") == 0)
    {
        return execute_time(argc, argv, env);
    }
    else if (strcmp(op, "clr") == 0)
    {
        return execute_clr(argc, argv, env);
    }
    else if (strcmp(op, "dir") == 0)
    {
        return execute_dir(argc, argv, env);
    }
    else if (strcmp(op, "set") == 0)
    {
        return execute_set(argc, argv, env);
    }
    else if (strcmp(op, "echo") == 0)
    {
        return execute_echo(argc, argv, env);
    }
    else if (strcmp(op, "help") == 0)
    {
        return execute_help(argc, argv, env);
    }
    else if (strcmp(op, "exit") == 0)
    {
        return execute_exit(argc, argv, env);
    }
    else if (strcmp(op, "date") == 0)
    {
        return execute_date(argc, argv, env);
    }
    else if (strcmp(op, "env") == 0)
    {
        return execute_env(argc, argv, env);
    }
    else if (strcmp(op, "who") == 0)
    {
        return execute_who(argc, argv, env);
    }
    

    return SH_SUCCESS;
}

sh_err_t Executor::execute_cd(const int argc, char * const argv[], char * const env[]) const
{
    assert(strcmp(argv[0], "cd")==0 && "unexpected node type");

    char *path = nullptr;
    if (argc == 1)
    {
        path = console_->home;  // 默认无参数时打开主目录
    }
    else if (argc == 2)
    {
        // 对于~目录需要特殊判断
        path = strcmp(argv[1], "~") ? argv[1] : console_->home;
    }
    else
    {
        return SH_ARGS; // 参数错误
    }

    // 更改目录
    int ret = chdir(path);
    if (ret != 0)   // 打开目录异常
    {
        throw ((std::string)"cd: 无法打开路径 " + (std::string)path);
    }

    // 重新设置控制台环境与系统环境变量
    if (getcwd(console_->current_working_dictionary, BUFFER_SIZE) != nullptr )
        setenv("PWD", console_->current_working_dictionary, 1);
    else
        throw "get cwd error";

    return SH_SUCCESS;
}

sh_err_t Executor::execute_pwd(const int argc, char * const argv[], char * const env[]) const
{
    assert(strcmp(argv[0], "pwd")==0 && "unexpected node type");
    display_->message(console_->current_working_dictionary);
    display_->message("\n");
    return SH_SUCCESS;
}

sh_err_t Executor::execute_time(const int argc, char * const argv[], char * const env[]) const
{
    assert(strcmp(argv[0], "time")==0 && "unexpected node type");

    // 相当于env命令，从兼容性的角度出发，这里选择调用env命令以兼容Linux用户需求以及后续开发
    return execute_date(argc, argv, env);
}

sh_err_t Executor::execute_clr(const int argc, char * const argv[], char * const env[]) const
{
    assert(strcmp(argv[0], "clr")==0 && "unexpected node type");
    
    return SH_SUCCESS;
}

sh_err_t Executor::execute_dir(const int argc, char * const argv[], char * const env[]) const
{
    assert(strcmp(argv[0], "dir")==0 && "unexpected node type");
    return SH_SUCCESS;
}

sh_err_t Executor::execute_set(const int argc, char * const argv[], char * const env[]) const
{
    assert(strcmp(argv[0], "set")==0 && "unexpected node type");
    
    // 相当于env命令，从兼容性的角度出发，这里选择调用env命令以兼容Linux用户需求
    return execute_env(argc, argv, env);
}

sh_err_t Executor::execute_echo(const int argc, char * const argv[], char * const env[]) const
{
    assert(strcmp(argv[0], "echo")==0 && "unexpected node type");
    return SH_SUCCESS;
}

sh_err_t Executor::execute_help(const int argc, char * const argv[], char * const env[]) const
{
    assert(strcmp(argv[0], "help")==0 && "unexpected node type");
    return SH_SUCCESS;
}

sh_err_t Executor::execute_exit(const int argc, char * const argv[], char * const env[]) const
{
    assert(strcmp(argv[0], "exit")==0 && "unexpected node type");
    return SH_EXIT;
}

sh_err_t Executor::execute_date(const int argc, char * const argv[], char * const env[]) const
{
    assert(strcmp(argv[0], "date")==0 && "unexpected node type");

    return SH_SUCCESS;
}

sh_err_t Executor::execute_env(const int argc, char * const argv[], char * const env[]) const
{
    assert(strcmp(argv[0], "env")==0 && "unexpected node type");
    
    while(*env)
    {
        char buffer[BUFFER_SIZE];
        snprintf(buffer, BUFFER_SIZE, "%s\n", *env++);
        display_->message(buffer);
    }

    return SH_SUCCESS;
}

sh_err_t Executor::execute_who(const int argc, char * const argv[], char * const env[]) const
{
    assert(strcmp(argv[0], "who")==0 && "unexpected node type");
    display_->message(console_->user_name);
    display_->message("\n");
    return SH_SUCCESS;
}