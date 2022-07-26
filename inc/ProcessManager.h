/**
 * @file ProcessManager.h
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @brief 进程管理
 * @version 1.0
 * @date 2022-07-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _PROCESS_MANAGER_H_
#define _PROCESS_MANAGER_H_

#include "Heap.h"
#include "config.h"

#include <set>
#include <unistd.h>

// 置于config.h头文件中，加快编译速度
// enum job_state                                  // 进程状态
// {
//     Running,                                    // 正在运行
//     Stopped,                                    // 停止运行
//     Done,                                       // 完成运行
//     Terminated                                  // 终止运行
// };

class job_unit
{
    public:
        job_unit(unsigned int _id, int _pid, job_state _state, int _argc, char * _argv[]);

        // ~job_unit();

        void PrintJob(int output_fd = STDOUT_FILENO);
        
        /* 为了使用集合，我们需要重载job unit的大小比较运算符 */
        bool operator== ( const job_unit& rhs ) const
        {
            return id == rhs.id;
        }

        bool operator!= ( const job_unit& rhs ) const
        {
            return !(*this == rhs);
        }

        bool operator< ( const job_unit& rhs ) const
        {
            return id < rhs.id;
        }

        bool operator> ( const job_unit& rhs ) const
        {
            return rhs < *this;
        }

        bool operator<= ( const job_unit& rhs ) const
        {
            return !(rhs < *this);
        }

        bool operator>= ( const job_unit& rhs ) const
        {
            return !(*this < rhs);
        }

    // private:
        unsigned int id;                            // 进程列表id
        pid_t pid;                                  // 进程列表pid
        job_state state;                            // 进程列表状态
        int argc;                                   // 进程列表参数
        char argv[MAX_ARGUMENT_NUMBER][BUFFER_SIZE];// 进程列表参数
};

class ProcessManager
{
    private:
        // 进程控制
        Heap<unsigned int> *job_heap;                   // 工作id分配堆
        std::set<class job_unit> jobs;                 // 进程列表，采用STL红黑树实现

    public:
        ProcessManager(/* args */);
        virtual ~ProcessManager();

        void PrintJobList(int output_fd = STDOUT_FILENO) const; // 打印进程列表

        void PrintJobListDone(int output_fd = STDOUT_FILENO); // 打印已完成的进程列表

        /**
         * @brief 添加进程
         * 
         * @param pid 进程号
         * @param state 状态
         * @param argc 参数个数
         * @param argv 参数列表
         * @return unsigned int 进程作业号，0表示添加失败
         * @version 1.0
         * @author 邱日宏 (3200105842@zju.edu.cn)
         * @date 2022-07-20
         * @copyright Copyright (c) 2022
         */
        unsigned int JobInsert(int pid, job_state state, int argc, char *argv[]);

        /**
         * @brief 删除进程
         * 
         * @param job 
         * @version 1.0
         * @author 邱日宏 (3200105842@zju.edu.cn)
         * @date 2022-07-21
         * @copyright Copyright (c) 2022
         */
        void JobRemove(job_unit * job);
        void JobRemove(std::set<job_unit>::iterator& job);

        int ForeGround(unsigned int jobid);
        int BackGround(unsigned int jobid);
};

#endif