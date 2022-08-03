/**
 * @file Executor.h
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2022-07-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _EXECUTOR_H_
#define _EXECUTOR_H_

#include "config.h"
#include "Console.h"
#include "Display.h"

#include <map>

class Executor
{
private:

    Console *console_;  /** @see 控制台 */

    Display *display_;  /** @see 显示器 */
    
    /** 更改目录 */
    sh_err_t execute_cd(const int argc, char * const argv[], char * const env[]) const;

    /** 显示当前目录 */
    sh_err_t execute_pwd(const int argc, char * const argv[], char * const env[]) const;

    /** 显示当前日期 */
    sh_err_t execute_time(const int argc, char * const argv[], char * const env[]) const;

    sh_err_t execute_clr(const int argc, char * const argv[], char * const env[]) const;

    sh_err_t execute_dir(const int argc, char * const argv[], char * const env[]) const;

    sh_err_t execute_set(const int argc, char * const argv[], char * const env[]) const;

    sh_err_t execute_echo(const int argc, char * const argv[], char * const env[]) const;

    sh_err_t execute_help(const int argc, char * const argv[], char * const env[]) const;

    /** 退出shell */
    sh_err_t execute_exit(const int argc, char * const argv[], char * const env[]) const;

    /** 显示当前日期 */
    sh_err_t execute_date(const int argc, char * const argv[], char * const env[]) const;

    /** 获取系统环境变量 */
    sh_err_t execute_env(const int argc, char * const argv[], char * const env[]) const;

    /** 获取当前登入用户信息 */
    sh_err_t execute_who(const int argc, char * const argv[], char * const env[]) const;
    
    /** 从命令到对应函数的映射，采用红黑树的STL实现 */
    sh_err_t (Executor::* FunctionArray[])(const int argc, char * const argv[], char * const env[]) = 
    {
    //     {"cd",    execute_cd  },
    //     {"pwd",   execute_pwd },
    //     {"time",  execute_time},
    //     {"clr",   execute_clr },
    //     {"dir",   execute_dir },
    //     {"set",   execute_set },
    //     {"echo",  execute_echo},
    //     {"help",  execute_help},
    //     {"exit",  execute_exit},
    //     {"date",  execute_date},
    //     {"env",   execute_env },
    //     {"who",   execute_who }
    //     // std::make_pair("cd",    execute_cd  ),
    //     // std::make_pair("pwd",   execute_pwd ),
    //     // std::make_pair("time",  execute_time),
    //     // std::make_pair("clr",   execute_clr ),
    //     // std::make_pair("dir",   execute_dir ),
    //     // std::make_pair("set",   execute_set ),
    //     // std::make_pair("echo",  execute_echo),
    //     // std::make_pair("help",  execute_help),
    //     // std::make_pair("exit",  execute_exit),
    //     // std::make_pair("date",  execute_date),
    //     // std::make_pair("env",   execute_env ),
    //     // std::make_pair("who",   execute_who )
    };

public:
    Executor(Console *model, Display *view);

    virtual ~Executor();

    /**
     * @brief 执行器命令执行函数
     * 
     * @param argc 传入参数个数
     * @param argv 传入具体参数
     * @param env  环境变量
     * @return sh_err_t 返回执行情况
     * @version 0.1
     * @author 邱日宏 (3200105842@zju.edu.cn)
     * @date 2022-07-04
     * @copyright Copyright (c) 2022
     */
    sh_err_t execute(const int argc, char * const argv[], char * const env[]) const;

    
};


#endif