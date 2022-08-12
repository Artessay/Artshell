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
            while (capacity_ < size)    // 内存不够则分配空间
                AllocMoreSpace();
            size_ = size;
            for (size_t i = 0; i < size; ++i)   // 拷贝数组
                node[i+1] = data[i];
            
            for (size_t i = (size_>>1); i>0; --i)    //从 n/2 开始
            {
                size_t p, child;
                T X = node[i];
                for (p = i; (p<<1) <= size_; p = child) //下滤
                {
                    child=(p<<1);   //寻找最小的孩子
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
                AllocMoreSpace();    // 内存不够则分配空间
            }

            int p;
            for (p = ++size_; node[p>>1] > value && p > 1; p = p>>1)    //下滤
                node[p] = node[p>>1];   //避免使用swap交换
            node[p] = value; //将节点插入在正确的位置上
        }

        virtual T top() const
        {
            if (size_ == 0)
                throw ExtractEmptyHeap();
            return node[1];
        }

        virtual T extract()
        {
            if (size_ == 0)   //如果堆是空的
                throw ExtractEmptyHeap();    //那么为异常

            T top, last;
            top = node[1];
            last = node[size_--];

            size_t p, child;
            for (p = 1; (p<<1) <= size_; p = child)  //下滤
            {
                child = (p<<1);   //寻找最小的孩子
                if (child != size_ && node[child+1] < node[child])
                    ++child;
                
                if (last >node[child])    // 如果未到合适的位置
                    node[p]=node[child];  // 将孩子提上来
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

        void AllocMoreSpace()   // 动态数组分配空间
        {
            capacity_<<=1;      // 容量翻倍
            T *newNode = new T[capacity_];
            if (newNode == NULL)
            {
                throw OutOfMemory();    // 堆内存不足异常
            }

            for (size_t i = 0; i < size_; ++i)
                std::swap(node[i], newNode[i]); // 直接地址交换，提高效率
            delete [] node;
            node = std::move(newNode);          // 移动拷贝，效率更佳
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
                    child=(p<<1);   //寻找最小的孩子
                    if (child != size_ && node[child+1] < node[child])
                        ++child;
                    
                    if (X > node[child])    // 如果未到合适的位置
                        node[p] = node[child];  // 将孩子提上来
                    else
                        break;
                }
                node[p] = X;  // 找到了合适的位置
            }
        }
};

#endif