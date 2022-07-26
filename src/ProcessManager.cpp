/**
 * @file ProcessManager.cpp
 * @author your name (you@domain.com)
 * @brief 进程管理器
 * @version 1.0
 * @date 2022-07-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "config.h"
#include "Console.h"
#include "BinaryHeap.h"
#include "ProcessManager.h"

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sys/wait.h>
#include <sys/types.h>

job_unit::job_unit(unsigned int _id, int _pid, job_state _state, int _argc, char * _argv[])
            : id(_id), pid(_pid), state(_state), argc(_argc)
{
    // argv 必须进行深拷贝，否则释放argv后将不再有，还会造成段错误
    assert(argc < MAX_ARGUMENT_NUMBER);
    for (int i = 0; i < argc; ++i)
        strncpy(argv[i], _argv[i], BUFFER_SIZE);
}

void job_unit::PrintJob(int output_fd)
{
    // if (argc <= 0)  // 参数错误
    // {
    //     assert(false && "argument error");
    //     return;
    // }

    const char *State_;
    switch (state)  // 状态映射
    {
        case Running:                                    // 正在运行
            State_ = "Running";
            break;
        case Stopped:                                    // 停止运行
            State_ = "Stopped";
            break;
        case Done:                                       // 完成运行
            State_ = "Done";
            break;
        case Terminated:                                 // 终止运行
            State_ = "Terminated";
            break;
    }

    // 状态打印
    char buffer[BUFFER_SIZE];
    ssize_t write_state;
    snprintf(buffer, BUFFER_SIZE-1, "[%u]%c\t%s\t\t\t\t\t", id, ' ', State_);
    write_state = write(output_fd, buffer, strlen(buffer));
    if (write_state == -1)
        throw std::exception();

    // 参数打印
    if (argc > 0)
    {
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
    }
    
    write_state = write(output_fd, "\n", 1);   // 打印换行符
    if (write_state == -1)
        throw std::exception();
}

ProcessManager::ProcessManager(/* args */)
{
    unsigned int job_id[MAX_PROCESS_NUMBER];
    for (unsigned int i = 1; i <= MAX_PROCESS_NUMBER; ++i)
        job_id[i-1] = i;        // 初始化工作进程id池
    job_heap = new BinaryHeap<unsigned int>(job_id, MAX_PROCESS_NUMBER);
    
    #ifdef _DEBUG_
    for (unsigned int i = 1; i <= MAX_PROCESS_NUMBER; ++i)
        printf("heap: %u\n", job_heap->extract());
    #endif
}

ProcessManager::~ProcessManager()
{
    delete job_heap;
}

void ProcessManager::PrintJobList(int output_fd) const
{
    for (auto job : jobs)
    {
        job.PrintJob(output_fd);
    }
}

void ProcessManager::PrintJobListDone(int output_fd)
{
    job_unit *pre_job = nullptr;
    
    for (auto job : jobs)
    {
        #ifdef _DEBUG_
        printf("Id: %u pid: %d\n", job.id, job.pid);
        #endif
        if (pre_job != nullptr)     // 内存回收
        {
            this->JobRemove(pre_job);
            pre_job = nullptr;
        }

        /* waitpid 在WNOHANG参数下 如果子进程已经结束，则返回子进程的pid；
        如果子进程还未结束，则返回0； 如果发生错误，则返回-1 */
        int stat_loc, wait_pid = waitpid(job.pid, &stat_loc, WNOHANG);
        #ifdef _DEBUG_
        printf("id: %u pid: %d wait: %d stat: %d\n", job.id, job.pid, wait_pid, stat_loc);
        #endif
        if (wait_pid == job.pid) // 已经结束
        {
            job.state = Done;
            job.PrintJob();
            pre_job = &job;
        }
        else if (wait_pid < 0)  // 发生错误
        {
            throw std::exception();
        }
    }

    if (pre_job != nullptr)     // 内存回收
        this->JobRemove(pre_job);
}

unsigned int ProcessManager::JobInsert(int pid, job_state state, int argc, char *argv[])
{
    try
    {
        unsigned int id = job_heap->extract();  // 从id池取出最小的id
        job_unit* newJob = new job_unit(id, pid, state, argc, argv);
        #ifdef _DEBUG_
        newJob->PrintJob();
        #endif
        jobs.emplace(*newJob);   // 加入集合
        return id;
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 0;
    }
}

void ProcessManager::JobRemove(job_unit * job)
{
    assert(job->id > 0);
    job_heap->insert(job->id);  // 将id放回id池中
    jobs.erase(*job);            // 移出集合
    // delete job;  // 因为在set里面存放的不是指针了，在erase set的时候已经完成了析构
    return;
}

void ProcessManager::JobRemove(std::set<job_unit>::iterator& job)
{
    job_heap->insert(job->id);  // 将id放回id池中
    jobs.erase(*job);            // 移出集合
    // delete job;  // 因为在set里面存放的不是指针了，在erase set的时候已经完成了析构
    return;
}

int ProcessManager::ForeGround(unsigned int jobid)
{
    for (auto job : jobs)
    {
        if (job.id == jobid)
        {
            Console::child_process_id = job.pid;
            setpgid(job.pid, getgid());

            // 将前端设置为子进程
            tcsetpgrp(STDIN_FILENO, job.pid);
            tcsetpgrp(STDOUT_FILENO, job.pid);
            tcsetpgrp(STDERR_FILENO, job.pid);

            job.state = Running;    // 继续运行

            kill(job.pid, SIGCONT); // 向子进程发送SIGCONT信号
            while(waitpid(Console::child_process_id, NULL, WNOHANG) == 0 && Console::child_process_id >= 0);
            
            Console::child_process_id = -1; // 等待子进程结束，并且将子进程状态清除
            JobRemove(&job);

            return jobid;
        }
    }

    return -1;  // 未找到该子进程
}


int ProcessManager::BackGround(unsigned int jobid)
{
    for (auto job : jobs)
    {
        if (job.id == jobid)
        {
            if (job.state == Running)
                return 0;
            
            job.state = Running;    // 修改子进程状态

            kill(job.pid, SIGCONT); // 发送继续执行信号SIGCONT
            
            return jobid;
        }
    }

    return -1;  // 未找到该子进程
}