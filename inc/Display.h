/**
 * @file Display.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "Console.h"

class Display
{
private:
    Console *console_;
public:
    Display(Console* console);
    virtual ~Display();

    void render();
};

#endif