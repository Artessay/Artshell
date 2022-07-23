/**
 * @file Console.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "Console.h"

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <exception>

Console::Console(/* args */)
{
    try
    {
        // 获取用户名称
        strncpy(user_name, getenv("USER"), BUFFER_SIZE);
        if (user_name == NULL)
        {
            throw "Environment Variable 'USER' does not exist"; //"当前系统中不存在环境变量USER";
        }

        // 获取主机名称
        int ret;
        ret = gethostname(host_name, BUFFER_SIZE);
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

        
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    /*
    
    */
}

Console::~Console()
{
}

int Console::init()
{
    
    
    return 0;
}