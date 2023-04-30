#include <queue>
#include <mutex>
#include "Task.hpp"

//用来管理任务的队列
class TaskQueue
{
private:
    std::queue<Task> taskQueue; 
    std::mutex taskQueue_lock;
public:
    TaskQueue();
    ~TaskQueue();
    void addTask(Task task);

    Task getTask();
    bool isEmpty();
    int getSize();
};

//**************************实现****************************************

TaskQueue::TaskQueue()
{
}

TaskQueue::~TaskQueue()
{
}

//添加任务
inline void TaskQueue::addTask(Task task)
{
    std::lock_guard<std::mutex> lock(taskQueue_lock);
    taskQueue.push(task);
}


//取一个任务
inline Task TaskQueue::getTask()
{
    std::lock_guard<std::mutex>  lock(taskQueue_lock);
    if(!taskQueue.empty()){
        Task task = taskQueue.front();
        taskQueue.pop();
        return task;
    }
}

// 判断任务队列是否为空
inline bool TaskQueue::isEmpty()
{
    std::lock_guard<std::mutex>  lock(taskQueue_lock);
    return taskQueue.empty();
}
// 得到任务个数
inline int TaskQueue::getSize()
{
    std::lock_guard<std::mutex>  lock(taskQueue_lock);
    return taskQueue.size();
}
