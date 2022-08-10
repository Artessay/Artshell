/**
 * @file ProcessManager.h
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @brief 进程管理
 * @version 0.1
 * @date 2022-08-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _PROCESS_MANAGER_H_
#define _PROCESS_MANAGER_H_

#include "Heap.h"

#include <set>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

class job_unit
{
    public:
        void PrintJob(int output_fd = STDOUT_FILENO)
        {
            if (argc <= 0)  // 参数错误
            {
                assert(false && "argument error");
                return;
            }

            const char *State_;
            switch (state)  // 状态映射
            {
                case Running:                                    // 正在运行
                    State_ = "Running";
                case Stopped:                                    // 停止运行
                    State_ = "Stopped";
                case Done:                                       // 完成运行
                    State_ = "Done";
                case Terminated:                                 // 终止运行
                    State_ = "Terminated";
            }

            // 状态打印
            char buffer[BUFFER_SIZE];
            ssize_t write_state;
            snprintf(buffer, BUFFER_SIZE-1, "[%u]%c\t%s\t\t\t\t\t", id, ' ', State_);
            write_state = write(output_fd, buffer, strlen(buffer));
            if (write_state == -1)
                throw std::exception();

            // 参数打印
            write_state = write(output_fd, argv[0], strlen(argv[0])); // 确保行末无多余的空格
            if (write_state == -1)
                throw std::exception();
            for (int i = 1; i < argc; ++i)
            {
                write_state = write(output_fd, " ", 1);   // 打印空格
                if (write_state == -1)
                    throw std::exception();

                write_state = write(output_fd, argv[i], strlen(argv[i])); // 打印参数
                if (write_state == -1)
                    throw std::exception();
            }
            write_state = write(output_fd, "\n", 1);   // 打印换行符
            if (write_state == -1)
                throw std::exception();
        }

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

    private:
        enum job_state                                  // 进程状态
        {
            Running,                                    // 正在运行
            Stopped,                                    // 停止运行
            Done,                                       // 完成运行
            Terminated                                  // 终止运行
        };

        unsigned int id;                            // 进程列表id
        pid_t pid;                                  // 进程列表pid
        job_state state;                            // 进程列表状态
        int argc;                                   // 进程列表参数
        char **argv;                                // 进程列表参数
};

class ProcessManager
{
    private:
        // 进程控制
        Heap<unsigned int> *job_heap;                   // 工作id分配堆
        std::set<struct job_unit> jobs;                 // 进程列表

    public:
        ProcessManager(/* args */);
        virtual ~ProcessManager();

        void PrintJobList(int output_fd = STDOUT_FILENO) const;
};

#endif