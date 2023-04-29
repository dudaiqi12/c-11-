#include <iostream>
#include "ThreadPool.hpp"
#include <unistd.h> // sleep(1);
#include <vector>
#include <thread>
using namespace std;

void test(void* arg){
	
	cout<<"\n "<<pthread_self()<<" 哈哈哈哈或或"<<"\n";
}


void test1(){
	ThreadPool* pool = new ThreadPool(3,10);
 
	Task k(test,nullptr);
	cout<<"等待3秒\n";
	while(1){
		sleep(3);
		cout<<"添加任务\n";
		pool->addTask(k);	
	}

	// testclass k;
	
	std::cout << std::endl;
	sleep(30);
}

void test2(){
	vector<thread> k;
	Task k2(test,NULL);
	k2.run();

}

int main()
{
	test1();
	sleep(2);
	return 0;
}
