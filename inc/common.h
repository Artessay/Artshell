/**
 * @file common.h
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2022-07-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _COMMON_H_
#define _COMMON_H_

/**
 * @brief 命令参数打印
 * 首行显示传入参数个数
 * 接下来一行一次显示命令行中的各个参数，以空格分开
 * 
 * @param argc 参数个数
 * @param argv 参数列表
 * @version 0.1
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @date 2022-08-15
 * @copyright Copyright (c) 2022
 */
void Argument_Display(int argc, char*argv[]);

#endif