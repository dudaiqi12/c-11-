#include <mutex>
#include <iostream>
#include <thread>
#include "TaskQueue.hpp"
#include <atomic>
#include <memory>
// 用来管理工作线程的生命周期
class WorkThread
{
private:
    std::thread thread_;
    //既然是工作线程 让它看到任务队列 很合理
    TaskQueue &taskQueue_;
    std::atomic_int state_;

    

public:
    //线程的状态 
    constexpr static int STATE_WAIT = 1;
    constexpr static int STATE_WORK = 2;
    constexpr static int STATE_EXIT = 3;
    
    // 信号量  用于知道自己是否已经完了
    // 有管理者线程通知
    std::atomic_bool finished_;

    WorkThread(TaskQueue &taskQueue);
    ~WorkThread();
    std::thread::id getID();
    int getState();
    
};

WorkThread::WorkThread(TaskQueue &taskQueue): taskQueue_(taskQueue),finished_(false),state_(WorkThread::STATE_WAIT)
{
    //一直读线程池
    while(!finished_){
        this->state_ = WorkThread::STATE_WAIT;
        if(taskQueue.isEmpty()){
            continue;
        }
        if(!finished_){
            this->state_ = WorkThread::STATE_EXIT;
            break;
        }
        this->state_ = WorkThread::STATE_WORK;
        Task task = taskQueue.getTask();
        task.run();
    }
}

WorkThread::~WorkThread()
{
    if(thread_.joinable()){
        //阻塞当前线程 直到thread_调用完成
        thread_.join();
    }
}


std::thread::id getID()
{
    return thread_.get_id();
}

int WorkThread::getState(){
    return state_;
}

using WorkThread_ptr = std::shared_ptr<WorkThread>;