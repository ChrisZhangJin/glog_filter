cmake_minimum_required(VERSION 3.9)
project(glog_filter)

add_definitions(-g)
#
#set(CMAKE_CXX_STANDARD 11)

#include_directories(/usr/local/include)

add_executable(glog_filter main.c)
#target_link_libraries(glog_filter /usr/local/lib/libpcre2-8.a)