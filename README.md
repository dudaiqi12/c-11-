# 基于C++11的线程池

**快速上手**
代码拉下来  然后 linux下 cd到shell.bat所在目录 执行./shell.bat就可以开跑
测试用例在src/main.cpp中


**特点**
1. 提交的任务可以有任意参数，任意返回值
2. 支持主线程异步接收addTask返回值,就是说主线程添加任务，和获取任务结果可以异步进行, 不会阻塞主线程。
主线程把食材和食谱给厨师做菜. 自己去玩一小时后再回来问厨师要菜。
3. 当任务队列为空的时候 应该用条件变量阻塞线程 通过管理者唤醒的方式来让线程执行任务


**include文件里的各个类**

**Task**：是任务类 , 用来包装回调函数

**TaskQueue**: 是任务队列 用来管理任务

**WorkerThread**: 工作线程类，用于管理工作线程的生命周期

**ThreadPool**: 线程池  
    TaskQueue: 任务队列 
    WorkerThread: 工作线程
    manager: 管理者线程    
    max: 线程池最大容量  
    min 最小容量
    线程集合: workerThread_map
    添加任务: addTask()
    启动线程池: run()

