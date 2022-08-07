// 程序：命令行编辑器
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

#include "config.h"
#include "Console.h"
#include "Display.h"
#include "Executor.h"

namespace _SHELL_
{
    const char * shell_error_message(sh_err_t err);

    int shell_loop(Console* model, Display* view, Executor* controller, char *env[]);

} // namespace _SHELL_


#endif