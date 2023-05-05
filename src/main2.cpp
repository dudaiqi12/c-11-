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


// 测试用例
void test1(){
	//创建线程池
	ThreadPool* pool = new ThreadPool(1,50);

	//填入任务
	pool->addTask([](void*){while(1){sleep(1);std::cout<<"1\n";}},nullptr);
	pool->addTask([](void*){while(1){sleep(1);std::cout<<"2\n";}},nullptr);
	pool->addTask([](void*){std::cout<<pthread_self()<<"hhh1231\n";},nullptr);
	
	//启动线程池
	pool->run();
	sleep(1);
	//当然 启动以后也是可以填入任务的
	pool->addTask([](void*){while(1){sleep(1);std::cout<<"3\n";}},nullptr);
	
	sleep(50);
}


static int sum = 0;
static const int num_tasks = 220000;//任务数
int ii = 1;
void work(void* arg){
	
	for(int j=0;j < 10000;j++){
		sum +=j;
	}
	// while (ii)
	// {
	// 	sleep(1);
	// 	std::cout<<"*************\n";
	// }
}

//测试线程池
void test2(){
    ThreadPool* pool = new ThreadPool(4,5);
	sleep(1);
	pool->run();
    auto start_time = std::chrono::high_resolution_clock::now();
	
	for (int i = 0; i < num_tasks; ++i)
    {
		pool->addTask(work,nullptr);
    }
	// sleep(8);
	// ii = 0;
	// sleep(40);
	// ii = 1;
	// sleep(40);
	// ii = 0;
	sleep(10);
	for (int i = 0; i < num_tasks; ++i)
    {
		pool->addTask(work,nullptr);
    }
	sleep(20);
	// while(sum != 49999500000000){}	
    auto end_time = std::chrono::high_resolution_clock::now();
	
	

    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    std::cout << "Elapsed time: " << elapsed_time << " ms" << std::endl;
}
//测试不用线程池
void test3(){

    auto start_time = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < num_tasks; ++i)
    {
		new std::thread(work,nullptr);
    }
	// while(sum != 49999500000000){}	
    auto end_time = std::chrono::high_resolution_clock::now();
	

    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    std::cout << "Elapsed time: " << elapsed_time << " ms" << std::endl;
}

//不用线程
void test4(){

	auto start_time = std::chrono::high_resolution_clock::now();
	for(int k = 0;k<num_tasks;k++){
		for(int j=0;j < 100000;j++){
			sum +=j;
		}
	}
    auto end_time = std::chrono::high_resolution_clock::now();


    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    std::cout << "Elapsed time: " << elapsed_time << " ms" << std::endl;
}

void test8(){
	ThreadPool* pool = new ThreadPool(2,10);
	pool->run();
	int task_num = 10; 
	std::array<std::future<int>,10> result;
	
	for(int i = 0;i<task_num;i++){
		result[i] = pool->addTask([](int a,int b)->int{
			sleep(8);
			return a+b;
		},i,i);
	}
	
	cout<<"主线程干自己的事情\n";
	// sleep(2);
	
	// delete pool;
	for (int i = 0; i < task_num; i++)
	{
		result[i].wait();
		// pool->addTask([](){});
		// cout<<"\n";
	}
	
	for (int i = 0; i < task_num; i++)
	{
		cout<<"result"<<i<<": "<<result[i].get()<<"\n";
	}
	
	// sleep(10);
}



int main()
{
	// std::cout<<std::thread::hardware_concurrency();//当前系统支持的最大线程数
	test8();
	// cout<<sum<<"\n";
	return 0;
}
