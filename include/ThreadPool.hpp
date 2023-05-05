
#include <map>
#include "WorkerThread.hpp"
#include <unistd.h>
#include <thread>
#include <future>
class ThreadPool
{
public:
    ThreadPool(int min_, int max_);
    ~ThreadPool();

    template<typename T,typename... Args>
    auto addTask(T&& f,Args &&...args) -> std::future<decltype(f(args...))>;

    void addThread();
    void removeThread();
    bool isEmpty();
    void run();
    
private:
    size_t min;
    size_t max;
    std::map<std::thread::id,WorkThread_ptr> workerThread_map;
    std::thread manager;
    TaskQueue taskQueue;
    bool finish_;
};

ThreadPool::ThreadPool(int min_, int max_ ) : min(min_),max(max_)
{
    finish_ = false;
    std::cout<<"\n";
}

//**************************实现****************************************

ThreadPool::~ThreadPool(){
    finish_ = true;
    workerThread_map.clear();
    sleep(4);

    taskQueue.clear();
    manager.detach();
}


inline void ThreadPool::addThread()
{
    auto t = std::make_shared<WorkThread>(taskQueue);
    workerThread_map[t->getID()] = t;
    std::cout<<"添加线程: "<< t->getID() <<" 任务:"<<taskQueue.getSize()<<" 剩下线程"<<workerThread_map.size()<<"个\n";
}



template<typename T,typename... Args>
auto ThreadPool::addTask(T&& f,Args &&...args) -> std::future<decltype(f(args...))>{
    //构造一个可调用对象  decltype(f(args...))是推导返回值类型  ()是参数列表 
    std::function<decltype(f(args...))()> func
        = std::bind(std::forward<T>(f),std::forward<Args>(args)...);
    //packaged_task封装了一个可调用对象
    auto temp = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);
    //包装回调函数
    Task_ptr task = std::make_shared<Task>([temp](){(*temp)();});
    taskQueue.addTask(task);
    tv.notify_one();
    
    return temp->get_future();
}




inline void ThreadPool::removeThread()
{   
    for(auto& temp : workerThread_map){
        if(temp.second->getState() == WorkThread::STATE_WAIT){           
            workerThread_map.erase(temp.first);
            std::cout<<"移除线程: " <<temp.first<<" 剩下"<<workerThread_map.size()<<"个\n";
            break;
        }
    }
}

inline bool ThreadPool::isEmpty()
{
    return taskQueue.isEmpty();
}

inline void ThreadPool::run()
{
    size_t num = taskQueue.getSize();
    std::cout<<"初始任务个数："<<num<<"\n";
    //初始化工作线程
    for (size_t i = 0; i < min; i++)
    {
        addThread();
    }
    //初始化管理线程
    manager = std::thread([this]{
        while(!finish_){
            //每两秒管理一次
            sleep(2);
            std::cout<<"当前任务："<<taskQueue.getSize()<<"  当前线程："<<workerThread_map.size()<<"\n";
            //添加线程 
            
            if(taskQueue.getSize() > workerThread_map.size() && workerThread_map.size() < max){
                //如果任务多于线程数就唤醒所有 然后再添加线程
                addThread();
            }
            
            //移除线程
            else if(taskQueue.getSize()*1.2 + 1  <= workerThread_map.size() 
            && workerThread_map.size() > min){
                removeThread();
            }
        }
        // 先唤醒所有线程
        tv.notify_all();
        std::cout<<"管理者线程退出\n";
    });
}
