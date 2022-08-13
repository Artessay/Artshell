/**
 * @file common.cpp
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @brief 共享函数库
 * @version 1.0
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

std::string& String_Trim(std::string &s) 
{
    if (s.empty()) // 如果s为空
    {
        return s;   // 则不必处理
    }
 
    s.erase(0,s.find_first_not_of(" "));    // 去除字符串前的空格
    s.erase(s.find_last_not_of(" ") + 1);   // 去除字符串后的空格
    return s;
}