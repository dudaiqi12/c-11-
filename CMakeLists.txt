cmake_minimum_required(VERSION 2.6)

project(C_EPOLL_SERVER)

include_directories(${CMAKE_SOURCE_DIR}/include)
#############多线程
include (FindThreads)
find_package (Threads REQUIRED)
##############

add_compile_options("-Wall")

set(CMAKE_BUILD_TYPE "Debug")

# set(SRC
#  src/k.cpp)
 
add_executable(../main src/main.cpp)

##多线程
target_link_libraries (../main ${CMAKE_THREAD_LIBS_INIT})