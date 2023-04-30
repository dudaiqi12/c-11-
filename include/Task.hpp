#include <iostream>
using callback = void(*)(void*);
class Task
{
public:
    callback func;
    void *arg;
    Task();
    Task(callback func_, void *args_);
    //执行任务
    void run();
};

//**************************实现****************************************


Task::Task()
{
    func = nullptr;
    arg = nullptr;
}
Task::Task(callback func_, void *args_)
{
    func = func_;
    arg = args_;
}

void Task::run(){
    if(func != nullptr){
        func(arg);
    }
}