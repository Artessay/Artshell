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

#include <string>
#include <sstream>

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

#endif