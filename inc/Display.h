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

class Console;
class Executor;

#include <string>

class Display
{
private:

    Console* console_;

    bool perform;   // 是否显示提示符的标志

protected:
    std::string buffer_;

public:
    Display(Console* console);
    
    virtual ~Display();

    /** 
     * @brief 命令行输入控制
     * @return 正数表示正常退出，返回读入的字符数；
     * 返回0表示读到EOF，返回负数表示出现错误
     */
    int InputCommand(char *input, const int len);
    
    /** @brief 命令行提示符显示模块 */
    void render();

    /** @brief 继续输入提示 */
    void prompt() const;

    /** @brief 打印信息msg与显示器 */
    void message(const char * msg);

    /** @brief 将所有打印信息统一显示在终端 */
    void show() const;

    /** @brief 清空缓冲区 */
    void clear() { buffer_ = ""; }
};

#endif