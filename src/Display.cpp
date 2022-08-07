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

void Display::InputCommand(char *input, const int len) 
{
    // 初始化输入缓冲器与相关变量
    char ch;
    int i = 0;
    memset(input, 0, len);

    // 循环读入字符
    do
    {
        read(console_->input_file_descriptor, &ch, 1);

        if (ch == '\\') // 如果读到换行输入\命令就跳过继续
        {
            ch = getchar(); // 将随后的换行符读入
            continue;
        }

        if (ch == ';')  // 将；视为换行符，便于lexer和parser处理
        {
            ch = '\n';
        }
            
        input[i++] = ch;

        if (i == len)   // 达到最大长度了
        {
            buffer_ = "\e[1;31mERROR\e[0m input compand exceeds maximum length. 输入命令的长度超过了允许的最大长度";
            memset(input, 0, len);  // 清空缓冲区输入
            break;
        }
    } while (ch != '\n');

    #ifdef _DEBUG_
    printf("input: %s", input);
    #endif
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

void Display::prompt() const
{
    if (write(console_->output_file_descriptor, ">", 1) == -1)
    {
        throw std::exception();
    }
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