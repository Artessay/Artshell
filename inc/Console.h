/**
 * @file Console.h
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2022-07-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include "config.h"

#include <sys/stat.h>
#include <sys/types.h>

class ProcessManager;   // 为了加快编译速度，这里不引用头文件而只是声明

/**
 * @brief 信号控制与处理
 * 
 * @param signal_ 
 * @version 0.1
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @date 2022-07-21
 * @copyright Copyright (c) 2022
 */
void SignalHandler(int signal_);

/**
 * @brief 控制台
 * 存储必要的环境变量以及渲染用户前端所需要的数据
 * 
 */
class Console
{
    private:
        // 显示模块
        char user_name[BUFFER_SIZE];                    // 用户名称
        char host_name[BUFFER_SIZE];                    // 主机名称
        char current_working_dictionary[BUFFER_SIZE];   // 当前工作目录

        char home[BUFFER_SIZE];                         // 主目录
        
        // 环境变量
        char shell_path_env[BUFFER_SIZE];               // shell的完整路径

        // 进程管理
        pid_t process_id;                               // 当前进程pid
        static pid_t child_process_id;                  // 子进程pid
        ProcessManager* process_manager;                // 进程管理器
        
        // 文件描述符
        int input_file_descriptor;                      // 输入文件描述符
        int output_file_descriptor;                     // 输出文件描述符
        int error_file_descriptor;                      // 错误文件描述符

        // 标准输入、输出与错误输出
        static int input_std_fd;                        // 标准输入备份
        static int output_std_fd;                       // 标准输出备份                              
        static int error_std_fd;                        // 标准错误备份

        // 重定向标志
        bool redirect_input;                             // 输入重定向状态
        bool redirect_output;                            // 输出重定向状态
        bool redirect_error;                             // 错误重定向状态

        // 掩码
        mode_t umask_;                                   // 文件掩码

        int argc;                                        // 当前命令参数个数
        char argv[MAX_ARGUMENT_NUMBER][BUFFER_SIZE];     // 当前命令参数列表

    public:
        Console(/* args */);

        virtual ~Console();

        /* 初始化 */
        int init();

        /* 打印进程列表 */
        void ConsoleJobList() const;

        /* 打印已完成的进程列表 */
        void ConsoleJobListDone();

        /* 添加进程 */
        unsigned int AddJob(int pid, job_state state, int argc, char *argv[]);

        // void RemoveJob();

        /* 设置文件描述符 */
        void SetInputFD(int _fd)  { input_file_descriptor = _fd; }
        /* 设置文件描述符 */
        void SetOutputFD(int _fd) { output_file_descriptor = _fd; }
        /* 设置文件描述符 */
        void SetErrorFD(int _fd)  { error_file_descriptor = _fd; }

        /* 获取文件描述符 */
        int GetInputFD() const  { return input_file_descriptor; }
        /* 获取文件描述符 */
        int GetOutputFD() const { return output_file_descriptor; }
        /* 获取文件描述符 */
        int GetErrorFD() const  { return error_file_descriptor; }

        /* 设置重定向状态 */
        void SetInputRedirect()  { redirect_input = true; }
        /* 设置重定向状态 */
        void SetOutputRedirect() { redirect_output = true; }
        /* 设置重定向状态 */
        void SetErrorRedirect()  { redirect_error = true; }

        /* 重置重定向状态 */
        void ResetInputRedirect()  { redirect_input = false; }
        /* 重置重定向状态 */
        void ResetOutputRedirect() { redirect_output = false; }
        /* 重置重定向状态 */
        void ResetErrorRedirect()  { redirect_error = false; }

        /* 获取重定向状态 */
        bool GetInputRedirect() const  { return redirect_input ; }
        /* 获取重定向状态 */
        bool GetOutputRedirect() const { return redirect_output; }
        /* 获取重定向状态 */
        bool GetErrorRedirect() const  { return redirect_error ; }

        /* 获取标注输入、输出、错误输出 */
        int GetSTDIN()  const { return input_std_fd;  }
        /* 获取标注输入、输出、错误输出 */
        int GetSTDOUT() const { return output_std_fd; }
        /* 获取标注输入、输出、错误输出 */
        int GetSTDERR() const { return error_std_fd;  }

        /* 设置掩码 */
        void SetMask(mode_t _mask) { umask_ = _mask; }
        /* 获取掩码 */
        mode_t GetMask() const { return umask_; }

        friend class Display;
        friend class Executor;
        friend class ProcessManager;
        friend void SignalHandler(int);
};

#endif