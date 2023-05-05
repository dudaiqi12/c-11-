#include <iostream>
#include "ThreadPool.hpp"
#include <unistd.h> // sleep(1);
#include <vector>
#include <thread>
#include <string>
#include <chrono>
#include <future>
#include <array>
using namespace std;

//测试用例
int main()
{
	// 构建线程池
	ThreadPool* pool = new ThreadPool(2,10);
	// 启动线程池
	pool->run();
	// 任务数量
	int task_num = 10; 
	// 存储各任务的返回期货
	std::array<std::future<int>,10> result;
	
	// 添加task_num个任务
	for(int i = 0;i<task_num;i++){
		result[i] = pool->addTask([](int a,int b)->int{
			sleep(8);
			return a+b;
		},i,i);
	}
	
	// 模拟主线程做别的事情
	cout<<"主线程干自己的事情\n";
	sleep(2);
	
	
	// 此时主线程希望拿到任务的返回值
	for (int i = 0; i < task_num; i++)
	{
		result[i].wait();
	}
	
	// 输出任务返回值
	for (int i = 0; i < task_num; i++)
	{
		cout<<"result"<<i<<": "<<result[i].get()<<"\n";
	}

	return 0;
}