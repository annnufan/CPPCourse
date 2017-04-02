cmake_minimum_required(VERSION 3.6)
project(big_int_with_vector)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES big_integer.cpp;optimized_vector.cpp)

add_executable(big_int_with_vector ${SOURCE_FILES})