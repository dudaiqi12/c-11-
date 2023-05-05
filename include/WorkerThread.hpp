
#include "TaskQueue.hpp"
#include <atomic>
#include <thread>
#include <memory>
#include <condition_variable>
#include <unistd.h>



//条件变量 唤醒线程
static std::mutex work_lock;
static std::condition_variable tv; 
static std::unique_lock<std::mutex> lock(work_lock);

// 用来管理工作线程的生命周期
class WorkThread
{
private:
    std::thread thread_;
    // 既然是工作线程 让它看到任务队列 很合理
    TaskQueue &taskQueue_;
    int state_;

public:
    // 线程的状态
    constexpr static int STATE_WAIT = 1;
    constexpr static int STATE_WORK = 2;

    // 信号量  用于知道自己是否已经完了
    // 有管理者线程通知
    bool finished_;

    WorkThread(TaskQueue &taskQueue);
    ~WorkThread();
    std::thread::id getID();
    int getState();
};

//**************************实现****************************************

WorkThread::WorkThread(TaskQueue &taskQueue) : taskQueue_(taskQueue)
{
    state_ = WorkThread::STATE_WAIT;
    finished_ = false;
    // 创建线程  让它一直读任务队列  lambda捕获this taskQueue
    thread_ = std::thread([this,&taskQueue]{
        while(!finished_){
            if(taskQueue.isEmpty()){
                //如果任务队列为空 就阻塞
                //当工作线程那边notify时候才醒来
                // std::cout<<"线程"<<std::this_thread::get_id()<<"因任务队列为空而阻塞\n";
                state_ = WorkThread::STATE_WAIT;
                tv.wait_for(lock,std::chrono::seconds(4));
                // std::cout<<"线程"<<std::this_thread::get_id()<<"被唤醒\n";
            }else{
                Task_ptr task = taskQueue.getTask();
                state_ = WorkThread::STATE_WORK;
                task->run();
                //因为这是一个任务的智能指针
                //所以执行完这个任务以后要释放这块内存
                task = nullptr;
            }
            if(!finished_){
                break;
            }
        } 
        // std::cout<<"线程"<<std::this_thread::get_id()<<"结束------\n";
    });
}

//被析构之前 让c++运行时库管理 线程执行完
WorkThread::~WorkThread()
{
    finished_ = true;
    std::cout<<"线程:"<<thread_.get_id()<<"被移除\n";
    thread_.detach();
}

std::thread::id WorkThread::getID()
{
    return thread_.get_id();
}

int WorkThread::getState()
{
    return state_;
}

using WorkThread_ptr = std::shared_ptr<WorkThread>;