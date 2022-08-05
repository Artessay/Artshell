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

extern "C" 
{
    #include "lexer.h"
    int yy_lexer(int *argc, char ***argv);
}

#include "config.h"
#include "common.h"
#include "Console.h"
#include "Display.h"
#include "Executor.h"

#endif