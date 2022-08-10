/**
 * @file BinaryHeap.h
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @brief 二叉堆
 * @version 0.1
 * @date 2022-08-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _BINARY_HEAP_H_
#define _BINARY_HEAP_H_

#include "Heap.h"
#include <assert.h>

static constexpr size_t HeapBlockSize = 1024;   // 默认堆大小

template <class T>
static constexpr T INF = -0x7f7f7f7f;         // 负无穷

template <class T>
class BinaryHeap : public Heap<T>
{
    public:
        BinaryHeap(int heap_capacity = HeapBlockSize)
        {
            assert(heap_capacity > 0);

            node = new T[heap_capacity+1];
            node[0] = -INF;
        }

        BinaryHeap(T data[], int size);
        virtual ~BinaryHeap();

        void build(T data[], int size);
        void insert(T value);
        T top() const;
        T extract();

    private:
        T *node;
};


#endif