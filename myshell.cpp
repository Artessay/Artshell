// 程序：命令行编辑器
// 作者：邱日宏 3200105842

/**
 * @file myshell.cpp
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @brief myshell程序的main函数，负责调用各个接口并实现myshell功能
 * @version 0.1
 * @date 2022-07-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "myshell.h"

#include <string>
#include <iostream>

int main(int argc, char *argv[], char **env)
{
    // puts("Welcome to MyShell ! \n");
    Console *model = new Console();
    if (model == nullptr)
    {
        std::cerr << "Out of Space for Console model" << std::endl;
        return 1;
    }

    Display *view = new Display(model);

    try
    {
        while (1)
        {
            view->render();
            std::string input;
            std::cin >> input;
            if (input == "exit")
                break;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    
    delete view;
    delete model;

    puts("Bye~");

    return 0;
}