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

static constexpr int FunctionNumber = 16;

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

    /** 清屏 */
    sh_err_t execute_clr(const int argc, char * const argv[], char * const env[]) const;

    /* 列出目录内容 */
    sh_err_t execute_dir(const int argc, char * const argv[], char * const env[]) const;

    /* 列出所有环境变量 */
    sh_err_t execute_set(const int argc, char * const argv[], char * const env[]) const;

    /* 回声 */
    sh_err_t execute_echo(const int argc, char * const argv[], char * const env[]) const;

    /* 显示帮助手册 */
    sh_err_t execute_help(const int argc, char * const argv[], char * const env[]) const;

    /** 退出shell */
    sh_err_t execute_exit(const int argc, char * const argv[], char * const env[]) const;

    /** 显示当前日期 */
    sh_err_t execute_date(const int argc, char * const argv[], char * const env[]) const;

    /** 清屏 */
    sh_err_t execute_clear(const int argc, char * const argv[], char * const env[]) const;

    /** 获取系统环境变量 */
    sh_err_t execute_env(const int argc, char * const argv[], char * const env[]) const;

    /** 获取当前登入用户信息 */
    sh_err_t execute_who(const int argc, char * const argv[], char * const env[]) const;

    /** 创建新目录 */
    sh_err_t execute_mkdir(const int argc, char * const argv[], char * const env[]) const;

    /** 移除空目录 */
    sh_err_t execute_rmdir(const int argc, char * const argv[], char * const env[]) const;

    /** 将被挂起的作业转到后台 */
    sh_err_t execute_bg(const int argc, char * const argv[], char * const env[]) const;

    /** 将后台作业转到前台 */
    sh_err_t execute_fg(const int argc, char * const argv[], char * const env[]) const;

    /** 显示所有作业 */
    sh_err_t execute_jobs(const int argc, char * const argv[], char * const env[]) const;

    /** 执行命令替换当前进程 */
    sh_err_t execute_exec(const int argc, char * const argv[], char * const env[]) const;
   
    /** 检测命令执行结构 */
    sh_err_t execute_test(const int argc, char * const argv[], char * const env[]) const;

    /** 设置掩码 */
    sh_err_t execute_umask(const int argc, char * const argv[], char * const env[]) const;

        /** myshell */
    sh_err_t execute_myshell(const int argc, char * const argv[], char * const env[]) const;
    
    /** 从命令到对应函数的映射，采用红黑树的STL实现 */

    /** 定义函数指针类型 */
    typedef sh_err_t (Executor::*MemFuncPtr)(const int argc, char * const argv[], char * const env[]) const;
    /** 创建函数指针数组 */
    MemFuncPtr FunctionArray[FunctionNumber];

    

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