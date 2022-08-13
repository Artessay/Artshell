// 程序：命令行解释器
// 作者：邱日宏 3200105842

/**
 * @file config.h
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @brief 配置文件
 * @version 0.1
 * @date 2022-07-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

static constexpr int BUFFER_SIZE = 1024;        // 缓冲区大小
static constexpr int MAX_PROCESS_NUMBER = 1024; // 最大进程数量
static constexpr int MAX_ARGUMENT_NUMBER = 128; // 最大参数数量

enum sh_err_t   //  shell错误类型
{
    SH_SUCCESS = 0, // 正常
    SH_FAILED,      // 失败
    SH_UNDEFINED,   // 未定义
    SH_ARGS,        // 参数错误
    SH_EXIT,        // 退出
};

enum job_state                                  // 进程状态
{
    Running,                                    // 正在运行
    Stopped,                                    // 停止运行
    Done,                                       // 完成运行
    Terminated                                  // 终止运行
};

constexpr unsigned int hash_prime = 33u;   // 相乘质数
constexpr unsigned int hash_basis = 5381u; // 偏移

/**
 * @brief 字符串散列，用于将字符串转为正整数，在编译时进行
 * 
 * @param input 需要转换的字符串
 * @return unsigned constexpr 散列后的哈希值
 * @version 0.1
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @date 2022-07-19
 * @copyright Copyright (c) 2022
 */
unsigned int constexpr String_Hash(char const *input, unsigned int prime = hash_prime, unsigned int basis = hash_basis) 
{
    return *input ?                                                          // 是否达到字符串的结尾
        static_cast<unsigned int>(*input) + prime * String_Hash(input + 1) : // 还未达到，递归求和继续
        basis;                                                               // 到达末尾，返回一个质数哈希
}

#endif