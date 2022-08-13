/**
 * @file Heap.h
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @brief 堆，抽象类
 * @version 1.0
 * @date 2022-07-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _HEAP_H_
#define _HEAP_H_

#include <assert.h>
#include <stddef.h>

/**
 * @brief 抽象堆
 * 
 * @tparam T 
 * @version 1.0
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @date 2022-07-20
 * @copyright Copyright (c) 2022
 */
template <class T>
class Heap
{
    public:
        Heap() : size_(0) {};

        /**
         * @brief Destroy the Heap object
         * Heap的析构函数。由于我们的链接库是静态库，因此无法将析构函数定义成纯虚函数。
         * 如果使用动态链接库的话则能够较好的实现多态，这里暂且将其定义为空函数以便链接。
         * 
         * @version 1.0
         * @author 邱日宏 (3200105842@zju.edu.cn)
         * @date 2022-07-20
         * @copyright Copyright (c) 2022
         */
        virtual ~Heap() {};
        
        size_t size() const { return size_; }

        virtual void build(T data[], size_t size) = 0;

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
};


#endif