/**
 * @file Heap.cpp
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @brief 二项堆, unsigned int 类的实现
 * @version 0.1
 * @date 2022-07-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "BinaryHeap.h"

BinaryHeap<unsigned int>::BinaryHeap(int heap_capacity = HeapBlockSize) 
: Heap(), capacity_(heap_capacity)
{
    assert(heap_capacity > 0);

    node = new unsigned int[heap_capacity+1];
    node[0] = 0;
}

template 
class Heap<unsigned int>;

template
class BinaryHeap<unsigned int>;