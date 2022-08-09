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

#endif