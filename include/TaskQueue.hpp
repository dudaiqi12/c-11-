#include <queue>
#include <iostream>
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
    void addTask(Task& task);
    void addTask(Task&& task);
    Task getTask();
    bool isEmpty();
    int getSize();
};



TaskQueue::TaskQueue()
{
}

TaskQueue::~TaskQueue()
{
}

inline void TaskQueue::addTask(Task task)
{
    std::lock_guard<std::mutex> lock(taskQueue_lock);
    taskQueue.push(task);
}

inline Task TaskQueue::getTask()
{
    std::lock_guard<std::mutex>  lock(taskQueue_lock);
    Task task = taskQueue.front();
    taskQueue.pop();
    return task;
}

inline bool TaskQueue::isEmpty()
{
    return taskQueue.empty();
}

inline int TaskQueue::getSize()
{
    return taskQueue.size();
}
