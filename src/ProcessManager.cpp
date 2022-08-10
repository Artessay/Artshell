#include "BinaryHeap.h"
#include "ProcessManager.h"

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
