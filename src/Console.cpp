/**
 * @file Console.cpp
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2022-07-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "Console.h"
#include "BinaryHeap.h"
#include "ProcessManager.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <exception>

Console::Console(/* args */)
{
    [[maybe_unused]] int ret;
    ret = init();       // 初始化
    assert(ret == 0);   // 判断初始化是否成功

    process_manager = new ProcessManager();

    return;
}

Console::~Console()
{
    delete process_manager;
}

int Console::init()
{
    try
    {
        // 获取用户名称
        memset(user_name, 0, BUFFER_SIZE);
        strncpy(user_name, getenv("USER"), BUFFER_SIZE-1);
        if (user_name == NULL)
        {
            throw "Environment Variable 'USER' does not exist"; //"当前系统中不存在环境变量USER";
        }

        // 获取主机名称
        int ret;
        ret = gethostname(host_name, BUFFER_SIZE-1);
        if (ret != 0)
        {
            throw "Error when getting host name";
        }
        
        // 获取当前工作目录
        char *result;
        result = getcwd(current_working_dictionary, BUFFER_SIZE);
        if (result == NULL)
        {
            throw "Error when getting current working dictionary";
        }

        // 获取主目录
        memset(home, 0, BUFFER_SIZE);
        strncpy(home, getenv("HOME"), BUFFER_SIZE-1);

        // 设置shell环境变量
        strncpy(shell_path_env, current_working_dictionary, BUFFER_SIZE);
        strncat(shell_path_env, "/myshell", BUFFER_SIZE);
        setenv("shell", shell_path_env, 1);

        // 设置掩码
        umask_ = umask(022);  // 获取默认掩码
        umask(umask_); // 改回原来掩码
        
        // 设置默认文件描述符
        input_file_descriptor = STDIN_FILENO;
        output_file_descriptor = STDOUT_FILENO;
        error_file_descriptor = STDERR_FILENO;

        // 设置重定向状态
        redirect_input = false;
        redirect_output = false;
        redirect_error = false;

        // 备份STD IO
        input_std_fd = dup(STDIN_FILENO);
        output_std_fd = dup(STDOUT_FILENO);
        error_std_fd = dup(STDERR_FILENO);

        // 获取进程
        process_id = getpid();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    
    return 0;
}

void Console::ConsoleJobList() const
{
    /* 输出应显示在屏幕上，无论如何重定向。 */
    process_manager->PrintJobList(output_std_fd);
}

void Console::ConsoleJobListDone()
{
    /* 输出应显示在屏幕上，无论如何重定向。 */
    process_manager->PrintJobListDone(output_std_fd);
}

unsigned int Console::AddJob(int pid, job_state state, int argc, char *argv[])
{
    return process_manager->JobInsert(pid, state, argc, argv);
}