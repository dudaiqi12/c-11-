#include "ManageThread.hpp"
using namespace std;

class ThreadPool
{
public:
    ThreadPool(int min_ = 10, int max_ = 50);
    ~ThreadPool();
    void addTask(Task task);
private:
    int min;
    int max;
    std::unordered_map<std::thread::id,WorkThread_ptr> workerThread_map;
    ManageThread_ptr manager;
    TaskQueue taskQueue;
    std::atomic_bool finish_;
};

ThreadPool::ThreadPool(int min_ = 10, int max_ = 50):min(min_),max(max_),finish_(false)
{
    //添加管理者线程
    manager = std::make_shared<ManageThread>(min_, max_,workerThread_map,taskQueue,finish_);

    //初始化工作线程
    for (int i = 0; i < min_; i++)
    {
        addThread();
    }
}

ThreadPool::~ThreadPool(){
}
