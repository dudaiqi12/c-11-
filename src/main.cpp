#include <iostream>
#include "ThreadPool.hpp"
#include <unistd.h> // sleep(1);
using namespace std;



void test(void* arg){
	cout<<"哈哈哈\n";
}

int main()
{
	ThreadPool* pool = new ThreadPool(5,10);
	
	std::cout << std::endl;
	sleep(5);
	return 0;
}
