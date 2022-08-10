/**
 * @file Heap.cpp
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @brief 二项堆
 * @version 0.1
 * @date 2022-07-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "BinaryHeap.h"

#include <assert.h>

template <class T>
BinaryHeap<T>::BinaryHeap(int heap_capacity = HeapBlockSize) 
: Heap(), capacity_(heap_capacity)
{
    assert(heap_capacity > 0);

    node = new T[heap_capacity+1];
    node[0] = -INF;
}

template <class T>
BinaryHeap<T>::~BinaryHeap()
{
    delete [] node;
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