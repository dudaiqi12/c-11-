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
	ThreadPool* pool = new ThreadPool(1,50);

	pool->addTask(Task([](void*){while(1){sleep(1);std::cout<<"1\n";}},nullptr));
	pool->addTask(Task([](void*){std::cout<<pthread_self()<<"hhh1231\n";},nullptr));
	
	pool->run();



	sleep(50);
}



int main()
{
	
	test1();

	return 0;
}
