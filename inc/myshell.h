// 程序：命令行解释器
// 作者：邱日宏 3200105842

/**
 * @file myshell.h
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @brief myshell头文件
 * 包含了myshell.cpp中所需要引用的所有自定义类
 * 
 * @version 0.1
 * @date 2022-07-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _MYSHELL_H_
#define _MYSHELL_H_

/* 配置文件 */
#include "config.h"

/* 类的声明 */
class Console;
class Display;
class Executor;

namespace SHELL
{
    /** @brief 启动shell */
    int shell_setup(int argc, char *argv[], char *env[]);

    /** @brief 进入shell循环 */
    int shell_loop(Console* model, Display* view, Executor* controller, char *env[]);

} // namespace SHELL


#endif