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
#include "config.h"

#include <unistd.h>

class ProcessManager
{
    private:
        // 进程控制
        enum job_state                                  // 进程状态
        {
            Running,                                    // 正在运行
            Stopped,                                    // 停止运行
            Done,                                       // 完成运行
            Terminated                                  // 终止运行
        };

        struct job_unit
        {
            unsigned int id;                            // 进程列表id
            pid_t pid;                                  // 进程列表pid
            job_state state;                            // 进程列表状态
            int argc;                                   // 进程列表参数
            char **argv;                                // 进程列表参数

            bool operator==(struct job_unit& that)
            {
                ///@todo paragraph describing what is to be done
            }
        };

        Heap<unsigned int> *job_heap;                   // 工作id分配堆
        struct job_unit jobs [MAX_PROCESS_NUMBER];      // 进程列表

    public:
        ProcessManager(/* args */);
        virtual ~ProcessManager();
};

#endif