#include <iostream>
#include <thread>
#include <functional>

template <typename F, typename... Args>
// 该函数的返回值将从std::future<decltype(f(args...))>中自动推导得出。
// ->语法可以直接指定返回类型，而不需要在函数名后面添加返回类型。这使得代码更加紧凑，同时也更加易于阅读和理解。
// 就跟lambda表达式一样 []()->int{};
// decltype关键字在这里用于推导函数返回类型
auto submit(F &&f, Args &&...args/*万能引用*/) -> std::future<decltype(f(args...))>
{
    // forward是一个用于完美转发的关键字。它可以将参数以原始的形式转发给另一个函数，同时保留参数的值类别和引用类型。
    // std::bind是一个函数适配器，用于将函数和参数绑定在一起，生成一个新的可调用对象。
    std::function<decltype(f(args...))()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
    
    // 连接函数和参数定义，特殊函数类型，避免左右值错误
    auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);


    std::function<void()> warpper_func = [task_ptr](){ //构造回调函数
        (*task_ptr)();
    };

    // 队列通用安全封包函数，并压入安全队列
    m_queue.enqueue(warpper_func);
    // 唤醒一个等待中的线程
    m_conditional_lock.notify_one();

    // 返回先前注册的任务指针
    return task_ptr->get_future();
}