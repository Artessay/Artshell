/**
 * @file Display.h
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2022-07-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "Console.h"

#include <string>

class Display
{
private:

    Console* console_;

    std::string buffer_;

public:
    Display(Console* console);
    
    virtual ~Display();

    /**
     * @brief 命令行提示符显示模块
     */
    void render();

    void message(const char * msg);

    void show() const;
};

#endif