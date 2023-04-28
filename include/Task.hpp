#include <iostream>
using callback = void(*)(void*);
struct Task
{
    callback func;
    void *arg;
    Task()
    {
        func = nullptr;
        arg = nullptr;
    };
    Task(callback func_, void *args_)
    {
        func = func_;
        this->arg = args_;
    };
};
