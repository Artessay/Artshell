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
#include "Executor.h"

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

Display::Display(Console* console)
: console_(console)
{
}

Display::~Display()
{
}

int Display::InputCommand(char *input, const int len) 
{
    // 初始化输入缓冲器与相关变量
    char ch;
    int i = 0;
    memset(input, 0, len);

    // 循环读入字符
    do
    {
        ssize_t state = read(console_->input_file_descriptor, &ch, 1);
        if (state == 0)
        {
            // 读到了EOF，结束
            if (i == 0) // 如果此时缓冲器中什么内容也没有
                return 0;   // 直接返回
            else    // 这是文本未加入换行的最后一行
            {
                input[i++] = '\n';  // 手动加入换行
                return i;           // 将最后一行命令处理完毕
            }
        }
        else if (state == -1)
        {
            throw "Read Input Error";
        }
        

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
            return -1;
        }
    } while (ch != '\n');

    #ifdef _DEBUG_
    printf("input: %s", input);
    #endif

    return i;
}

int Display::shell_parser(Console *model, Display* view, Executor* controller, int& argc, char *argv[], char *env[])
{
    if (argc == 0)
        return 0;   // 无参，不需处理
    
    for (int index = argc-1; index > 0; --index)    // 从末尾开始往前扫描，第一个不必扫
    {
        std::string arg(argv[index]);    // 使用string类处理
        
        /** 重定向处理 */
        
        /** 标准输入重定向 */
        if (arg == "<" || arg == "0<")
        {
            if (index + 1 == argc)  // 重定向符号是最后一个输入
            {
                throw "语法解析错误";
            }

            if (model->GetInputRedirect())      // 如果已经设置了重定向状态了
            {
                throw "多重重定向错误";
            }

            const char * input_file = argv[index + 1];
            int fd = open(input_file, O_RDONLY);
            if (fd < 0)
                throw std::exception();
                            
            model->SetInputFD(fd);
            model->SetInputRedirect();

            dup2(fd, STDIN_FILENO); // 重定向标准输入至fd

            for (int jump = index + 2; jump < argc; ++jump)
                argv[jump-2] = argv[jump];
            argc = argc - 2;
            argv[argc] = NULL;
        }

        /** 标准输出重定向 */
        if (arg == ">" || arg == "1>")
        {
            if (index + 1 == argc)  // 重定向符号是最后一个输入
            {
                throw "语法解析错误";
            }

            if (model->GetOutputRedirect())      // 如果已经设置了重定向状态了
            {
                throw "多重重定向错误";
            }

            const char * output_file = argv[index + 1];
            int fd = open(output_file, O_WRONLY | O_TRUNC | O_CREAT, 0777&(~model->GetMask()));
            if (fd < 0)
                throw std::exception();                
            
            model->SetOutputFD(fd);
            model->SetOutputRedirect();

            dup2(fd, STDOUT_FILENO); // 重定向标准输出至fd
                
            for (int jump = index + 2; jump < argc; ++jump)
                argv[jump-2] = argv[jump];
            argc = argc - 2;
            argv[argc] = NULL;
        }

        /** 标准错误输出重定向 */
        if (arg == "2>")
        {
            if (index + 1 == argc)  // 重定向符号是最后一个输入
            {
                throw "语法解析错误";
            }

            if (model->GetErrorRedirect())      // 如果已经设置了重定向状态了
            {
                throw "多重重定向错误";
            }

            const char * output_file = argv[index + 1];
            int fd = open(output_file, O_WRONLY | O_TRUNC | O_CREAT, 0777&(~model->GetMask()));
            if (fd < 0)
                throw std::exception();                
            
            model->SetErrorFD(fd);
            model->SetErrorRedirect();

            dup2(fd, STDERR_FILENO); // 重定向标准错误输出至fd

            for (int jump = index + 2; jump < argc; ++jump)
                argv[jump-2] = argv[jump];
            argc = argc - 2;
            argv[argc] = NULL;
        }

        /** 追加 */
        if (arg == ">>" || arg == "1>>")
        {
            /* 词法解析时应该识别<和>符号的闭包 */
            #ifdef _DEBUG_
            Argument_Display(argc, argv);
            #endif
            
            if (index + 1 == argc)  // 重定向符号是最后一个输入
            {
                throw "语法解析错误";
            }

            if (model->GetOutputRedirect())      // 如果已经设置了重定向状态了
            {
                throw "多重重定向错误";
            }

            const char * output_file = argv[index + 1];
            int fd = open(output_file, O_WRONLY | O_APPEND | O_CREAT, 0777&(~model->GetMask()));
            if (fd < 0)
                throw std::exception();                
            
            model->SetOutputFD(fd);
            model->SetOutputRedirect();

            dup2(fd, STDOUT_FILENO); // 重定向标准输出至fd

            for (int jump = index + 2; jump < argc; ++jump)
                argv[jump-2] = argv[jump];
            argc = argc - 2;
            argv[argc] = NULL;
        }
    }

    return 0;
}

void Display::render()
{
    buffer_ = "";   // 每轮循环前将输出缓冲区清空

    // 如果不是从标准输入中输入或是不是将内容输出到标准输出的话，
    if (console_->input_file_descriptor != STDIN_FILENO ||
        console_->output_file_descriptor != STDOUT_FILENO)
        return; // 就不需要打印提示符

    char buffer[BUFFER_SIZE];   // 打印缓冲区
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

    return;
}

void Display::prompt() const
{
    if (write(console_->output_file_descriptor, "> ", 2) == -1)
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