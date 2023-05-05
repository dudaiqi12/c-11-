#include <queue>
#include <mutex>
#include "Task.hpp"
#include <iostream>
#include <thread>

//用来管理任务的队列
class TaskQueue
{
private:
    std::queue<Task_ptr> taskQueue; 
    std::mutex taskQueue_lock;
public:
    TaskQueue();
    ~TaskQueue();
    void addTask(Task_ptr task);

    Task_ptr getTask();
    bool isEmpty();
    size_t getSize();
};

//**************************实现****************************************

TaskQueue::TaskQueue()
{
}

TaskQueue::~TaskQueue()
{
    while(!isEmpty()){
        Task_ptr task = taskQueue.front();
        task = nullptr;
        taskQueue.pop();
    }
}

//添加任务
inline void TaskQueue::addTask(Task_ptr task)
{
    std::lock_guard<std::mutex> lock(taskQueue_lock);
    
    taskQueue.push(task);
}


//取一个任务
inline Task_ptr TaskQueue::getTask()
{
    std::lock_guard<std::mutex> lock(taskQueue_lock);
    if(!taskQueue.empty()){
        Task_ptr task = taskQueue.front();
        taskQueue.pop();
        std::cout<<"线程： "<<std::this_thread::get_id()<<" 拿任务\n";
        return task;
    }
    
}

// 判断任务队列是否为空
inline bool TaskQueue::isEmpty()
{
    std::lock_guard<std::mutex> lock(taskQueue_lock);
    return taskQueue.empty();
}

// 得到任务个数
inline size_t TaskQueue::getSize()
{
    std::lock_guard<std::mutex> lock(taskQueue_lock);
    return taskQueue.size();
}

