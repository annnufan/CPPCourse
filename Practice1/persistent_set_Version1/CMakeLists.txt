cmake_minimum_required(VERSION 3.6)
project(persistent_set)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES persistent_set.cpp)
add_executable(persistent_set ${SOURCE_FILES})