/**
 * @file Parser.h
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @brief 语法分析
 * @version 0.1
 * @date 2022-07-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _PARSER_H_
#define _PARSER_H_

class Console;
class Display;
class Executor;

class Parser
{
private:
    enum {SUCCESS = 0, EXIT = 1};

    /**
     * @brief 执行shell
     * 
     * @param model 
     * @param view 
     * @param controller 
     * @param argc 
     * @param argv 
     * @param env 
     * @return true 
     * @return false 
     * @version 0.1
     * @author 邱日宏 (3200105842@zju.edu.cn)
     * @date 2022-07-19
     * @copyright Copyright (c) 2022
     */
    static bool shell_execute(Console *model, Display* view, Executor* controller, int& argc, char *argv[], char *env[]);

public:
    Parser(/* args */) {};

    /**
     * @brief Destroy the Parser object
     * 
     * @version 0.1
     * @author 邱日宏 (3200105842@zju.edu.cn)
     * @date 2022-07-19
     * @copyright Copyright (c) 2022
     */
    virtual ~Parser() = 0;  // 抽象类，纯虚函数

    static bool shell_pipe(Console *model, Display* view, Executor* controller, int& argc, char *argv[], char *env[]);

    static int shell_parser(Console *model, Display* view, Executor* controller, int& argc, char *argv[], char *env[]);
};

#endif