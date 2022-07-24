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

class Executor
{
private:

    Console *console_;  /** @see 控制台 */

    Display *display_;  /** @see 显示器 */
    
    sh_err_t execute_cd(const int argc, const char *argv[]) const;

    sh_err_t execute_pwd(const int argc, const char *argv[]) const;

    sh_err_t execute_time(const int argc, const char *argv[]) const;

    sh_err_t execute_clr(const int argc, const char *argv[]) const;

    sh_err_t execute_dir(const int argc, const char *argv[]) const;

    sh_err_t execute_set(const int argc, const char *argv[]) const;

    sh_err_t execute_echo(const int argc, const char *argv[]) const;

    sh_err_t execute_help(const int argc, const char *argv[]) const;

    sh_err_t execute_exit(const int argc, const char *argv[]) const;

    /** 获取系统环境变量 */
    sh_err_t execute_env(const int argc, const char *argv[], const char *env[]) const;

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
    sh_err_t execute(const int argc, const char *argv[], const char *env[]) const;

    
};


#endif