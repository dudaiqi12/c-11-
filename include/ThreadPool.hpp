
#include <unordered_map>
#include "WorkerThread.hpp"
#include <unistd.h>
class ThreadPool
{
public:
    ThreadPool(int min_, int max_);
    ~ThreadPool();
    void addTask(Task task);
    void addThread();
    void removeThread();
    void run();
private:
    int min;
    int max;
    std::unordered_map<std::thread::id,WorkThread_ptr> workerThread_map;
    std::thread manager;
    TaskQueue taskQueue;
    std::atomic_bool finish_;
};

ThreadPool::ThreadPool(int min_, int max_ ) : min(min_),max(max_)
{
    finish_ = false;
    std::cout<<"\n";
}

//**************************实现****************************************

ThreadPool::~ThreadPool(){
    finish_ = true;
    if(manager.joinable()){
        manager.join();
    }
}

inline void ThreadPool::addThread()
{
    auto t = std::make_shared<WorkThread>(taskQueue);
    workerThread_map[t->getID()] = t;
    std::cout<<"添加线程: 剩下"<<workerThread_map.size()<<"个\n";
}

void ThreadPool::addTask(Task task){
    taskQueue.addTask(task);
}

inline void ThreadPool::removeThread()
{
    for(auto& temp : workerThread_map){
        if(temp.second->getState() == WorkThread::STATE_WAIT){
            temp.second->finished_ = true;
            
            workerThread_map.erase(temp.first);
            std::cout<<"移除线程: 剩下"<<workerThread_map.size()<<"个\n";
            break;
        }
    }
    
}

inline void ThreadPool::run()
{
    int num = taskQueue.getSize();
    std::cout<<"初始任务个数："<<num<<"\n";
    //初始化工作线程
    num = num > max?max:num;
    for (int i = 0; i < num; i++)
    {
        addThread();
    }
    //初始化管理线程
    manager = std::thread([this]{
        while(!finish_){
            sleep(1);
            //添加线程
            
            if(taskQueue.getSize() > workerThread_map.size() && workerThread_map.size() < max){
                addThread();
            }
            //让线程紫砂
            if(taskQueue.getSize()*1.2 + 1  < workerThread_map.size() 
            && workerThread_map.size() > 0){
                removeThread();
            }

        }
    });
}
