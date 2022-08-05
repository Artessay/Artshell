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
public:
    Console(/* args */);

    virtual ~Console();

    int init();

    friend class Display;
    friend class Executor;
};

#endif