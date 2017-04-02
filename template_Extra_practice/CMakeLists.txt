cmake_minimum_required(VERSION 3.6)
project(template)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp template_for_num_type.cpp)
add_executable(template ${SOURCE_FILES})