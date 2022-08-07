/**
 * @file common.cpp
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @brief 共享函数库
 * @version 0.1
 * @date 2022-07-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "common.h"

#include <stdio.h>

void Argument_Display(const int argc, char* const argv[])
{
    printf("argc: %d\n", argc);
    for (int i = 0; i < argc; ++i)
    {
        printf("%s ", argv[i]);
    }
    putchar('\n');
    return;
}