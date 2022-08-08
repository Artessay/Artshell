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
    // char parent_path_env[BUFFER_SIZE];              // 执行程序的父进程
    
    // 文件描述符
    int input_file_descriptor;                      // 输入文件描述符
    int output_file_descriptor;                     // 输出文件描述符
    int error_file_descriptor;                      // 错误文件描述符

    bool redirect_input;                             // 输入重定向状态
    bool redirect_output;                            // 输出重定向状态
    bool redirect_error;                             // 错误重定向状态

    mode_t umask_;                                   // 文件掩码

public:
    Console(/* args */);

    virtual ~Console();

    int init();

    void SetInputFD(int _fd)  { input_file_descriptor = _fd; }
    void SetOutputFD(int _fd) { output_file_descriptor = _fd; }
    void SetErrorFD(int _fd)  { error_file_descriptor = _fd; }

    int GetInputFD() const  { return input_file_descriptor; }
    int GetOutputFD() const { return output_file_descriptor; }
    int GetErrorFD() const  { return error_file_descriptor; }

    void SetInputRedirect()  { redirect_input = true; }
    void SetOutputRedirect() { redirect_output = true; }
    void SetErrorRedirect()  { redirect_error = true; }

    void ResetInputRedirect()  { redirect_input = false; }
    void ResetOutputRedirect() { redirect_output = false; }
    void ResetErrorRedirect()  { redirect_error = false; }

    bool GetInputRedirect() const  { return redirect_input ; }
    bool GetOutputRedirect() const { return redirect_output; }
    bool GetErrorRedirect() const  { return redirect_error ; }

    void SetMask(mode_t _mask) { umask_ = _mask; }
    mode_t GetMask() const { return umask_; }

    friend class Display;
    friend class Executor;
};

#endif