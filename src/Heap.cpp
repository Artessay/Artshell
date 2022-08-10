/**
 * @file Heap.cpp
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @brief 堆，抽象类
 * @version 0.1
 * @date 2022-07-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "Heap.h"
#include <assert.h>

#define ASSERT(expr, message) assert((expr) && (message))

template <class T>
Heap<T>::Heap() : size_(0) {}

template <class T>
int Heap<T>::size() const
{
    return size_;
}

template <class T>
void Heap<T>::build(T data[], int size)
{
    ASSERT(false, "build not implemented.");
}

template <class T>
void Heap<T>::insert(T value)
{
    ASSERT(false, "insert not implemented.");
}

template <class T>
T Heap<T>::top() const
{
    ASSERT(false, "top not implemented.");
    return nullptr;
}

template <class T>
T Heap<T>::extract()
{
    ASSERT(false, "extract not implemented.");
    return nullptr;
}