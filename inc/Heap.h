/**
 * @file Heap.h
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @brief 堆，抽象类
 * @version 0.1
 * @date 2022-07-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _HEAP_H_
#define _HEAP_H_

#include <assert.h>
#include <stddef.h>

template <class T>
class Heap
{
    public:
        Heap();
        virtual ~Heap() = 0;
        
        size_t size() const { return size_; }

        virtual void build(T data[], int size)
        {
            assert(false && "build not implemented.");
        }

        virtual void insert(T value)
        {
            assert(false && "insert not implemented.");
        }

        virtual T top() const
        {
            assert(false && "top not implemented.");
            return 0;
        }
        
        virtual T extract()
        {
            assert(false && "extract not implemented.");
            return 0;
        }
    
    protected:
        size_t size_;      // 当前容量
        size_t capacity_;  // 最大容量
};


#endif