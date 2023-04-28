#include "Task.hpp"
#include <thread>
#include <vector>
#include <mutex>
#include <queue>
#include <unistd.h>
#include <list>
using namespace std;
void manage(void* arg);
void work(void* arg);


class ThreadPool{
public:
    ThreadPool(int min_,int max_);

    //操作任务队列;
    Task getTask();
    void addTask(Task& task);
    void addThread();
    int getBusyNum(){
        return busyNum;
    }
    int getLiveNum(){
        return liveNum;
    }
    int getMax(){
        return max;
    }
    int getMin(){
        return min;
    }
    int getTaskNum(){
        return taskQ.size();
    }
    mutex& getLock(){
        return pool_lock;
    }
    void popThread(int i){
        pool_lock.lock();
        exitNum = i;
        pool_lock.unlock();
    }

    //判断线程池是否关闭
    bool is_shutdown();

//访问任务队列 要加锁
private:
    int max;
    int min;
    int liveNum;
    int busyNum;
    int exitNum;
    bool shutdown = false;
    thread manager;//管理者线程
    list<thread> workers;//工作线程集合
    queue<Task> taskQ;//任务队列

    //锁
    mutex pool_lock;
};
void ThreadPool::addThread(int i){
    while (i > 0)
    {
        workers.push_back(thread(work,this));
        --i;
    }
    
   
}

bool ThreadPool::is_shutdown(){
    return shutdown;
}

void ThreadPool::addTask(Task& task){
    //添加任务先加锁
    pool_lock.lock();
    taskQ.push(task);
    pool_lock.unlock();   
}

Task ThreadPool::getTask(){
    Task temp;
    pool_lock.lock();
    temp = taskQ.front();//队头出队
    taskQ.pop();
    pool_lock.unlock();
    return temp;
}

void manage(void* arg){
    ThreadPool* pool = (ThreadPool*) arg;
    
    //如果线程池没有被关闭 就管理线程池 
    //主要是根据线程池当前任务个数和线程数量 决定
    //是否加入线程  或者 是否添加线程
    while(!pool->is_shutdown()){
        sleep(3);
        
        //是否添加线程
        //如果任务个数+2大于线程个数并且线程池没有被关闭
        //并且线程个数小于max
        //就添加两个线程
        if(pool->getTaskNum() + 2 >= pool->getLiveNum()
        && pool->getLiveNum() < pool->getMax()){
            pool->addThread(2);
        }

        // 如果忙线程个数的两倍小于存活线程，并且线程池没有关闭，那么
        // 就删除两个线程 不是说管理线程主动是删 是通过一个信号 让工作线程自己紫砂
        if(pool->getBusyNum()*2 < pool->getLiveNum()){
            pool->popThread(2);
        }
    }
}
void work(void* arg){
    
    ThreadPool* pool = (ThreadPool*) arg;
    while (!pool->is_shutdown())
    {
       // 不停访问任务队列


       //检查是否应该紫砂
        
        
    }
    
   

}

ThreadPool::ThreadPool(int min_,int max_){
    min = min_;
    max = max_;
    liveNum = min;
    busyNum = 0;
    exitNum = 0;
    manager = thread(manage,this);
   
    for(int i = 0;i < min_; i++){
        workers.push_back(thread(work,this));
    }
    
    cout<<workers.size()<<"  ";
    //hihihi
    
    
    
    
}