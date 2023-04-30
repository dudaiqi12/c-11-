#include <iostream>
#include "ThreadPool.hpp"
#include <unistd.h> // sleep(1);
#include <vector>
#include <thread>
#include <string>
#include <chrono>
using namespace std;


// 测试用例
void test1(){
	//创建线程池
	ThreadPool* pool = new ThreadPool(1,50);

	//填入任务
	pool->addTask(Task([](void*){while(1){sleep(1);std::cout<<"1\n";}},nullptr));
	pool->addTask(Task([](void*){while(1){sleep(1);std::cout<<"2\n";}},nullptr));
	pool->addTask(Task([](void*){std::cout<<pthread_self()<<"hhh1231\n";},nullptr));
	
	//启动线程池
	pool->run();
	sleep(1);
	//当然 启动以后也是可以填入任务的
	pool->addTask(Task([](void*){while(1){sleep(1);std::cout<<"3\n";}},nullptr));
	
	sleep(50);
}



int main()
{
	
	test1();

	return 0;
}
