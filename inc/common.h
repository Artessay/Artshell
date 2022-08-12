// 程序：命令行解释器
// 作者：邱日宏 3200105842

/**
 * @file common.h
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @brief 共享函数库
 * @version 0.1
 * @date 2022-07-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _COMMON_H_
#define _COMMON_H_

#include <cmath>
#include <string>
#include <cassert>
#include <sstream>

// 错误判断与信息提示
#define ASSERT(expr, message) assert((expr) && (message))

/**
 * @brief 命令参数打印
 * 首行显示传入参数个数
 * 接下来一行一次显示命令行中的各个参数，以空格分开
 * 
 * @param argc 参数个数
 * @param argv 参数列表
 * @version 0.1
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @date 2022-07-15
 * @copyright Copyright (c) 2022
 */
void Argument_Display(const int argc, char* const argv[]);

/**
 * @brief 二分搜索查找，查询范围为[l, r)
 * 
 * @tparam T 
 * @tparam Tp 
 * @param lelt 查找左区间，包含
 * @param right 查找右区间，不包含
 * @param val 查找变量
 * @param array 搜索数组
 * @param cmp 比较方法
 * @return int 返回对应元素下标，若没有找到则返回-1
 * @version 0.1
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @date 2022-07-17
 * @copyright Copyright (c) 2022
 */
template<typename T>
int Binary_Search(int left, int right, T val, T array[], int cmp(T a, T b))
{
    while (left < right)
    {
        int mid = (left + right) >> 1;
        int compare_result = cmp(val, array[mid]);
        if (compare_result == 0)
            return mid;
        else if (compare_result > 0)
            left = mid + 1;
        else
            right = mid;
    }

    return -1;
}

/**
 * @brief 去掉字符串两端空格
 * 
 * @param s 需要去除空格的字符串
 * @return std::string& 去除完空格的字符串
 * @version 0.1
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @date 2022-07-17
 * @copyright Copyright (c) 2022
 */
std::string& String_Trim(std::string &s);

/**
 * @brief 将字符串转换成任意类型变量
 * 
 * @tparam Type 返回类型
 * @param str 提取的字符串
 * @return Type 转化后的类型变量
 * @version 0.1
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @date 2022-07-18
 * @copyright Copyright (c) 2022
 */
template <class Type>
Type String_to_Number(const std::string& str)
{
    std::istringstream iss(str);
    Type num;
    iss >> num;
    return num;    
}

/** 比较取小 */
template <typename T>
inline T Min(const T& a, const T& b)
{
    return a < b ? a : b;
}

/** 比较取大 */
template <typename T>
inline T Max(const T& a, const T& b)
{
    return a > b ? a : b;
}

/**
 * @brief 八进制转十进制
 * 
 * @tparam T 
 * @param octalNumber 八进制数
 * @return T 十进制数
 * @version 0.1
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @date 2022-07-19
 * @copyright Copyright (c) 2022
 */
template <typename T>
T Octal_to_Decimal(T octalNumber)
{
    T decimalNumber = 0, i = 0, remainderNumber;
    while (octalNumber != 0)
    {
        remainderNumber = octalNumber % 10; // 余数
        octalNumber /= 10;                  // 退位
        decimalNumber += remainderNumber * pow(8, i);   // 幂乘
        ++i;
    }
    return decimalNumber;
}

/**
 * @brief 十进制转八进制
 * 
 * @tparam T 
 * @param decimalNumber 十进制数
 * @return T 八进制数
 * @version 0.1
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @date 2022-07-19
 * @copyright Copyright (c) 2022
 */
template <typename T>
T Decimal_to_Octal(T decimalNumber)
{
    T remainderNumber, i = 1, octalNumber = 0;
    while (decimalNumber != 0)
    {
        remainderNumber = decimalNumber % 8; // 余数
        decimalNumber /= 8;                  // 退位
        octalNumber += remainderNumber * i;  // 幂乘
        i *= 10;
    }
    return octalNumber;
} 

/**
 * @brief 十六进制转十进制
 * 
 * @tparam T 
 * @param hexadecimalNumber 十六进制数
 * @return T 十进制数
 * @version 0.1
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @date 2022-07-19
 * @copyright Copyright (c) 2022
 */
template <typename T>
T Hexadecimal_to_Decimal(T hexadecimalNumber)
{
    T decimalNumber = 0, i = 0, remainderNumber;
    while (hexadecimalNumber != 0)
    {
        remainderNumber = hexadecimalNumber % 10; // 余数
        hexadecimalNumber /= 10;                  // 退位
        decimalNumber += remainderNumber * pow(16, i);  // 幂乘
        ++i;
    }
    return decimalNumber;
}

/**
 * @brief 十进制转十六进制
 * 
 * @tparam T 
 * @param decimalNumber 十进制数
 * @return T 十六进制数
 * @version 0.1
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @date 2022-07-19
 * @copyright Copyright (c) 2022
 */
template <typename T>
T Decimal_to_Hexadecimal(T decimalNumber)
{
    T remainderNumber, i = 1, hexadecimalNumber = 0;
    while (decimalNumber != 0)
    {
        remainderNumber = decimalNumber % 16; // 余数
        decimalNumber /= 16;                  // 退位
        hexadecimalNumber += remainderNumber * i;  // 幂乘
        i *= 10;
    }
    return hexadecimalNumber;
} 

/**
 * @brief timespec时间比较
 * 
 * @param time1 时间1
 * @param time2 时间2
 * @return true 如果time1的时间晚于time2的时间
 * @return false 如果time1的时间不晚于time2的时间
 * @version 0.1
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @date 2022-07-20
 * @copyright Copyright (c) 2022
 */
inline bool test_timespec_newer(struct timespec& time1, struct timespec& time2)
{
    if (time1.tv_sec > time2.tv_sec)    // 先比较秒
        return true;
    else if (time1.tv_sec < time2.tv_sec)
        return false;
    else
        return time1.tv_nsec > time2.tv_nsec;   // 再比较纳秒
}

/**
 * @brief timespec时间比较
 * 
 * @param time1 时间1
 * @param time2 时间2
 * @return true 如果time1的时间早于time2的时间
 * @return false 如果time1的时间不早于time2的时间
 * @version 0.1
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @date 2022-07-20
 * @copyright Copyright (c) 2022
 */
inline bool test_timespec_older(struct timespec& time1, struct timespec& time2)
{
    if (time1.tv_sec < time2.tv_sec)    // 先比较秒
        return true;
    else if (time1.tv_sec > time2.tv_sec)
        return false;
    else
        return time1.tv_nsec < time2.tv_nsec;   // 再比较纳秒
}

#endif