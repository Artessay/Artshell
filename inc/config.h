/**
 * @file config.h
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2022-07-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

static constexpr int BUFFER_SIZE = 1024;
static constexpr int MAX_ARGUMENT_NUMBER = 128;

enum sh_err_t
{
    SH_SUCCESS = 0,
    SH_FAILED,
    SH_UNDEFINED,
    SH_ARGS,
    SH_EXIT,
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