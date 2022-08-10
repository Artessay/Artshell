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

template <class T>
class Heap
{
    public:
        Heap();
        virtual ~Heap() = 0;

        virtual void build(T data[], int size);
        virtual void insert(T value);
        virtual T top() const;
        virtual T extract();
        int size() const;
    
    protected:
        int size_;      // 当前容量
        int capacity_;  // 最大容量
};


#endif