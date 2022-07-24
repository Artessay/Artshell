/**
 * @file Display.cpp
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2022-07-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "Display.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

Display::Display(Console* console)
: console_(console)
{
}

Display::~Display()
{
}

void Display::render()
{
    char buffer[BUFFER_SIZE];
    int sret = snprintf(buffer, BUFFER_SIZE, "\e[1;32m%s@%s\e[0m:\e[1;34m%s\e[0m> ", \
        console_->user_name, console_->host_name, console_->current_working_dictionary);
    if (sret == -1)
    {
        throw "Error when writing into output buffer";
    }

    ssize_t ret;
    ret = write(console_->output_file_descriptor, buffer, strlen(buffer));
    if (ret == -1)
    {
        throw "Error when writing from buffer";
    }

    buffer_ = "";
}

void Display::message(const char * msg)
{
    buffer_ += std::string(msg);
}

void Display::show() const
{
    ssize_t ret;
    ret = write(console_->output_file_descriptor, buffer_.c_str(), buffer_.length());
    if (ret == -1)
    {
        throw "Error when showing buffer in Display";
    }
}