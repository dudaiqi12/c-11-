# 基于C++11的线程池


代码拉下来  然后 linux下  执行./shell.bat就可以开跑



**include文件里的各个类**

**Task**：是任务类 , 构造一个任务通过传入回调函数和一个参数

**TaskQueue**: 是任务队列 用来管理任务

**WorkerThread**: 工作线程类，用于管理工作线程的生命周期

**ThreadPool**: 线程池  里面包括TaskQueue 、WorkerThread

以及管理者线程manager    线程池最大容量max  最小容量min

用于管理线程的哈希表 workerThread_map; 对外提供addTask()接口用于添加任务、提供run()用来运行线程池


src/main.cpp 里面有使用例子

