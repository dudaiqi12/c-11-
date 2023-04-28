#include <pthread.h>
#include <queue>
#include <vector>
#include <string>
#include <unistd.h> // sleep(1);
using namespace std;
using callback = void *(void *);
void *manage(void *arg);
void *work(void *arg);

struct Task
{
    Task()
    {
        func = nullptr;
        args = nullptr;
    };
    Task(callback func_,void* args_){
        func = func_;
        this->args = args_;
    };
    callback func;
    void *args;
};

class ThreadPool
{
public:
    int max;     // 最大线程数
    int min;     // 最小线程数
    int liveNum; // 存活线程数
    int busyNum; // 繁忙的线程数
    bool shutdowm = false;
    pthread_t managerID;              // 管理者线程
    std::vector<pthread_t> workerIDs; // 工作线程id集合
    std::queue<Task> taskQueue;
    pthread_cond_t notEmpty;   // 任务队列是不是空了
    pthread_mutex_t pool_lock; // 线程池锁
    int exitNum;               // 要销毁的线程数
    ThreadPool() {}
    // 初始化线程池
    ThreadPool(int max_, int min_)
    {
        min = min_;
        max = max_;
        busyNum = 0;
        liveNum = min_;
        exitNum = 0;
        workerIDs.reserve(max);
        for (int i = 0; i < max; i++)
        {
            workerIDs[i] = 0; // -1表示没有线程
        }
        pthread_create(&managerID, NULL, manage, this);
        for (int i = 0; i < max; i++)
        {
            pthread_create(&workerIDs[i], NULL, work, this);
        }
        // 初始化锁和条件变量
        if (pthread_mutex_init(&pool_lock, NULL) != 0 &&
            pthread_cond_init(&notEmpty, NULL) != 0)
        {
            std::cout << "初始化锁和条件变量失败\n";
        }
    }

    ~ThreadPool()
    {
        shutdowm = true;
        // 阻塞回收管理者线程
        pthread_join(managerID, NULL); // NULL是接收线程的返回值
        // 唤醒阻塞的消费者线程
        for (int i = 0; i < liveNum; i++)
        {
            pthread_cond_signal(&notEmpty);
        }

        // 释放内存
        pthread_mutex_destroy(&pool_lock);
    }

    void addTask(Task task)
    {
        pthread_mutex_lock(&pool_lock);
        taskQueue.push(task);
        pthread_mutex_unlock(&pool_lock);
    }

    // 取出一个任务
    Task takeTask()
    {
        pthread_mutex_lock(&pool_lock);
        Task task;
        if (taskQueue.size() > 0)
        {
            task = taskQueue.front();
            taskQueue.pop();
        }

        pthread_mutex_unlock(&pool_lock);
        return task; // 浅拷贝 实际上task里面哪个指针还指向原来的地址
    }

    // 获取当前队列中任务个数
    inline int getTaskNum()
    {
        return taskQueue.size();
    }

    bool is_shutdown()
    {
        return this->shutdowm;
    }

    // 退出当前线程
    void threadExit()
    {
        pthread_t tid = pthread_self();
        for (int i = 0; i < max; i++)
        {
            if (workerIDs[i] == tid)
            {
                workerIDs[i] = 0;
                cout << "thread " << to_string(tid) << " exit\n";
                break;
            }
        }
        // 退出当前线程
        pthread_exit(NULL);
    }
};

// 管理者线程任务
void *manage(void *arg)
{
    ThreadPool *pool = (ThreadPool *)arg;
    while (!pool->is_shutdown())
    {
        sleep(3);
        // 增加线程
        pthread_mutex_lock(&pool->pool_lock);
        int busy = pool->busyNum;
        int live = pool->liveNum;
        int size = pool->getTaskNum();
        pthread_mutex_unlock(&pool->pool_lock);
        // 如果任务个数大于存活线程 && 存活线程<最大线程
        if (size > live && live < pool->max)
        {
            pthread_mutex_lock(&pool->pool_lock);
            int count = 0;
            for (int i = 0; i < pool->max && count < 2 && pool->liveNum < pool->max; i++)
            {
                // 说明这个位置没有线程
                if (pool->workerIDs[i] == 0)
                {
                    count++;
                    pthread_create(&pool->workerIDs[i], NULL, work, pool);
                }
            }
            pthread_mutex_unlock(&pool->pool_lock);
        }

        // 删除线程
        // 忙的线程*2 <存活线程 && 存活的线程 > 最小线程
        if (busy * 2 < live && live > pool->min)
        {
            pthread_mutex_lock(&pool->pool_lock);
            // 某个线程工作完以后会检查exitNum是否有
            // 如果大于0 自己就自杀
            pool->exitNum = 2;
            pthread_mutex_unlock(&pool->pool_lock);
            // 让工作线程自杀
            for (size_t i = 0; i < 2; i++)
            {
                // 设置notEmpty信号
                // 唤醒被阻塞的线程
                pthread_cond_signal(&pool->notEmpty);
            }
        }
    }
}

void *work(void *arg)
{
    ThreadPool *pool = (ThreadPool *)arg;
    
    while (!pool->is_shutdown())
    {
        // 访问任务队列
        pthread_mutex_lock(&pool->pool_lock);
        // 如果没关闭并且任务队列为空  线程无事可干 就 阻塞
        while (pool->getTaskNum() == 0 && !pool->is_shutdown())
        {
            cout << "\nthread " << to_string(pthread_self()) << " waiting..." << endl;
            // 阻塞线程
            pthread_cond_wait(&pool->notEmpty, &pool->pool_lock);
            // 解除阻塞之后, 判断是否要销毁线程
            if (pool->exitNum > 0)
            {
                pool->liveNum--;
                if (pool->liveNum > pool->min)
                {
                    pool->liveNum--;
                    pthread_mutex_unlock(&pool->pool_lock);
                    pool->threadExit();
                }
            }
        }

        /// 有任务  取出一个任务
        cout<<"\n\n\n";
        Task task = pool->takeTask();
        pool->busyNum++;
        pthread_mutex_unlock(&pool->pool_lock);
        cout << "thread " << to_string(pthread_self()) << " start working\n";

        // task.func(task.args);
        // delete task.args;

        // task.args = nullptr;
        cout << "thread " << to_string(pthread_self()) << " end working\n";
        pthread_mutex_lock(&pool->pool_lock);
        pool->busyNum--;
        pthread_mutex_unlock(&pool->pool_lock);
    }
}
