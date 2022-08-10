/**
 * @file BinaryHeap.h
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @brief 二叉堆
 * @version 0.1
 * @date 2022-07-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _BINARY_HEAP_H_
#define _BINARY_HEAP_H_

#include "Heap.h"
#include <assert.h>
#include <exception>

static constexpr size_t HeapBlockSize = 1024;   // 默认堆大小

// template <class T>
// static constexpr T INF = -0x7f7f7f7f;         // 负无穷

/**
 * @brief 二项堆，小根堆
 * 
 * @tparam T 
 * @version 0.1
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @date 2022-07-20
 * @copyright Copyright (c) 2022
 */
template <class T>
class BinaryHeap : public Heap<T>
{
    // 由于父类是模板类，因此子类在使用时必须使用using引入命名空间，
    // 或者用this指针实现多态，这样才能正确构造父类的模板类函数
    using Heap<T>::size_;   

    public:
        BinaryHeap(size_t heap_capacity = HeapBlockSize)
        : Heap<T>(), capacity_(heap_capacity)
        {
            assert(heap_capacity > 0);

            node = new T[heap_capacity+1];  // 分配内存
            if (node == NULL)               // 异常处理
                throw OutOfMemory();
        }

        BinaryHeap(T data[], size_t size, size_t heap_capacity = HeapBlockSize)
        : Heap<T>(), capacity_(heap_capacity)
        {
            node = new T[(size>heap_capacity?size:heap_capacity) + 1];  // 分配内存
            if (node == NULL)               // 异常处理
                throw OutOfMemory();

            size_ = size;
            for (size_t i = 1; i <= size; ++i) // 数组拷贝
                node[i] = data[i-1];
            
            build_heap();                        // 建堆
        }

        virtual ~BinaryHeap()
        {
            delete [] node;
        }

        virtual void build(T data[], size_t size)
        {
            while (capacity_ < size)
                AllocMoreSpace();
            size_ = size;
            for (size_t i = 0; i < size; ++i)
                node[i+1] = data[i];
            
            for (size_t i = (size_>>1); i>0; --i)    //从 n/2 开始
            {
                size_t p, child;
                T X = node[i];
                for (p = i; (p<<1) <= size_; p = child) //percolate down
                {
                    child=(p<<1);   //find the smaller child
                    if (child != size_ && node[child+1] < node[child])
                        ++child;
                    
                    if (X > node[child])
                        node[p] = node[child];
                    else
                        break;
                }
                node[p] = X;
            }
        }

        virtual void insert(T value)
        {
            if (size_ + 2 >= capacity_)
            {
                AllocMoreSpace();
            }

            int p;
            for (p = ++size_; node[p>>1] > value && p > 1; p = p>>1)    //percolate up
                node[p] = node[p>>1];   //avoid the use of swap
            node[p] = value; //insert the node in right place
        }

        virtual T top() const
        {
            if (size_ == 0)
                throw ExtractEmptyHeap();
            return node[1];
        }

        virtual T extract()
        {
            if (size_ == 0)   //if the heap is null
                throw ExtractEmptyHeap();    //then return -1

            T top, last;
            top = node[1];
            last = node[size_--];

            size_t p, child;
            for (p = 1; (p<<1) <= size_; p = child) //percolate down
            {
                child = (p<<1);   //find the smaller child
                if (child != size_ && node[child+1] < node[child])
                    ++child;
                
                if (last >node[child])
                    node[p]=node[child];
                else
                    break;
            }

            node[p] = last;
            return top;
        }

    protected:
        size_t capacity_;  // 最大容量
        T *node;           // 数据

        class ExtractEmptyHeap : public std::exception {};
        class OutOfMemory : public std::exception {};

        void AllocMoreSpace()
        {
            capacity_<<=1;
            T *newNode = new T[capacity_];
            if (newNode == NULL)
            {
                throw OutOfMemory();
            }

            for (size_t i = 0; i < size_; ++i)
                std::swap(node[i], newNode[i]);
            delete [] node;
            node = std::move(newNode);
        }

    private:
        void build_heap()
        {
            for (size_t i = (size_>>1); i>0; --i)    //从 n/2 开始
            {
                size_t p, child;
                T X = node[i];
                for (p = i; (p<<1) <= size_; p = child) //percolate down
                {
                    child=(p<<1);   //find the smaller child
                    if (child != size_ && node[child+1] < node[child])
                        ++child;
                    
                    if (X > node[child])
                        node[p] = node[child];
                    else
                        break;
                }
                node[p] = X;
            }
        }
};

#endif