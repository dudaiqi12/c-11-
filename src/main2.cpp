#include <iostream>
#include "pool.hpp"
#include <unistd.h> // sleep(1);
using namespace std;
void taskFunc(void* arg)
{
    int num = *(int*)arg;
    printf("thread %ld is working, number = %d\n",
        pthread_self(), num);
    sleep(1);
}



void test(void* arg){
	cout<<"哈哈哈\n";
}

int main()
{
    ThreadPool* pool = new ThreadPool(3,10);
	pool->addTask(Task(test,NULL));

	sleep(30);
	std::cout << std::endl;
	return 0;
}