#include <unordered_map>
#include "TaskQueue.hpp"
#include "WorkerThread.hpp"
#include <unistd.h>
#include <atomic>
class ManageThread
{
    int& min;
    int& max;
    std::unordered_map<std::thread::id, WorkThread_ptr>& workerThread_map;
    TaskQueue& taskQueue;
    std::atomic_bool& finish_;
public:
    ManageThread(int& min_,
    int& max_,
    std::unordered_map<std::thread::id, WorkThread_ptr>& workerThread_map_,
    TaskQueue& taskQueue_,
    std::atomic_bool& finish__);
    ~ManageThread();
    void addThread();
    void removeThread();
};

ManageThread::ManageThread(int& min_,
    int& max_,
    std::unordered_map<std::thread::id, WorkThread_ptr>& workerThread_map_,
    TaskQueue& taskQueue_,
    std::atomic_bool& finish__)
    {
    min = min_;
    max = max_;
    workerThread_map = workerThread_map_;
    taskQueue = taskQueue_;
    finish_ = finish__;

    while(!finish_){
        sleep(2);
        //看看是否添加线程
        if(taskQueue.getSize() > workerThread_map.size() && workerThread_map.size() < max){
            addThread();
        }

        //是否删除线程
        if(taskQueue.getSize() + 5 < workerThread_map.size() && workerThread_map.size() > 0){
            removeThread();
        }
    }
};

ManageThread::~ManageThread(){};

void ManageThread::addThread(){
    auto workThread_ptr = std::make_shared<WorkThread>(taskQueue);
    workerThread_map[workThread_ptr->getID()] = workThread_ptr;
    
}

//删除空闲的线程
void ManageThread::removeThread(){
    for(auto&& t : workerThread_map){
        if(t.second->getState() == WorkThread::STATE_WAIT){
            t.second->finished_ = true;//通知线程紫砂
            workerThread_map.erase(t.first);
            break;
        }
    }
}

using ManageThread_ptr = std::shared_ptr<ManageThread>;