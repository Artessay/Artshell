/**
 * @file Console.cpp
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @brief 控制台
 * @version 1.0
 * @date 2022-07-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "Console.h"
#include "BinaryHeap.h"
#include "ProcessManager.h"

#include <pwd.h>
#include <wait.h>
#include <assert.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <exception>

int Console::input_std_fd;
int Console::output_std_fd;
int Console::error_std_fd;
pid_t Console::child_process_id = -1;
static Console* cp = nullptr;   // 为了能够在友元函数中引用控制台，在此处设置本地变量以利于信号处理

Console::Console(/* args */)
{
    [[maybe_unused]] int ret;
    ret = init();       // 初始化
    assert(ret == 0);   // 判断初始化是否成功

    process_manager = new ProcessManager();
    for (int i = 0; i < MAX_ARGUMENT_NUMBER; ++i)
        argv[i] = new char [BUFFER_SIZE];
    cp = this;

    return;
}

Console::~Console()
{
    delete process_manager;
    for (int i = 0; i < MAX_ARGUMENT_NUMBER; ++i)
        delete [] argv[i];
}

void SignalHandler(int signal_)
{
    switch (signal_)
    {
        case SIGINT:    // Ctrl C 交互注意信号
            #ifdef _DEBUG_
            printf("Ctrl + C\n");
            #endif
            if (write(STDOUT_FILENO, "\n", 1) < 0)
                throw std::exception();
            // 当kill的pid < 0时  取|pid|发给对应进程组。
            // kill(-getpid(), SIGINT);
            
            // 子进程的CTRL C重置了，由子进程处理中断
            break;
        
        case SIGTSTP:    // Ctrl Z 键盘中断
            #ifdef _DEBUG_
            printf("Ctrl + Z\n");
            #endif
            if (write(STDOUT_FILENO, "\n", 1) < 0)
                throw std::exception();
            
            printf("Z pid: %d child pid: %d\n", getpid(), Console::child_process_id);

            if (Console::child_process_id >= 0)
            {
                setpgid(Console::child_process_id, 0);
                kill(Console::child_process_id, SIGTSTP);
            
                #ifdef _DEBUG_
                printf("kill success\n");
                #endif
                unsigned int jobid = cp->AddJob(Console::child_process_id, Stopped, cp->argc, cp->argv);
                printf("Add job success\n");
                
                // 打印当前进程
                char buffer[BUFFER_SIZE];
                snprintf(buffer, BUFFER_SIZE-1, "[%u] %d\n", jobid, Console::child_process_id);
                if (write(cp->output_std_fd, buffer, strlen(buffer)) == -1)
                    throw std::exception();
            
                snprintf(buffer, BUFFER_SIZE-1, "[%u]%c\tStopped\t\t\t\t\t", jobid, ' ');
                if (write(cp->output_std_fd, buffer, strlen(buffer)) == -1)
                    throw std::exception();

                // 参数打印
                if (cp->argc > 0)
                {
                    // 确保行末无多余的空格
                    if (write(cp->output_std_fd, cp->argv[0], strlen(cp->argv[0])) == -1)
                        throw std::exception();
                    for (int i = 1; i < cp->argc; ++i)
                    {
                        if (write(cp->output_std_fd, " ", 1) == -1)    // 打印空格
                            throw std::exception();

                        // 打印参数
                        if (write(cp->output_std_fd, cp->argv[i], strlen(cp->argv[i])) == -1)
                            throw std::exception();
                    }
                }
                if (write(cp->output_std_fd, "\n", 1) == -1)
                    throw std::exception();

                Console::child_process_id = -1;
            }
            break;
        
        case SIGCHLD:   // 子进程结束
            // 父进程收到子进程退出命令后，回收子进程
            // waitpid(-1, NULL, WNOHANG);
            cp->ResetChildPid();
            break;

        default:
            break;
    }
    
}

int Console::init()
{
    try
    {
        // 获取用户名称
        struct passwd *pw = getpwuid(getuid());
        if (pw == nullptr)
        {
            throw "get user database entry error";
        }
        memset(user_name, 0, BUFFER_SIZE);
        strncpy(user_name, pw->pw_name, BUFFER_SIZE-1);

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
        child_process_id = -1;  // 暂无子进程

        // signal(SIGINT, SignalHandler);  // Ctrl + C
        signal(SIGTSTP, SignalHandler); // Ctrl + Z
        signal(SIGCHLD, SignalHandler); // 子进程结束时发送给父进程的信号

        // 屏幕shell从后台调用tcsetpcgrp时收到的信号
        signal(SIGTTIN, SIG_IGN);   // 屏蔽SIGTTIN信号 
        signal(SIGTTOU, SIG_IGN);   // 屏蔽SIGTTOU信号 
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
    /* 显示工作列表，以打印与重定向处。 */
    process_manager->PrintJobList(STDOUT_FILENO);
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