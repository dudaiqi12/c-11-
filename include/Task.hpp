#include <iostream>
#include <functional>
#include <cstdarg>
#include <memory>
// using callback = void(*)(void*);
class Task
{
private:
    std::function<void()> func;

public:
    Task(std::function<void()> func_):func(func_){};
    // 执行任务
    void run()
    {
        func();
    }
};
using Task_ptr = std::shared_ptr<Task>;