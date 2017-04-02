g++ -std=gnu++11  -Wl,--stack=256000000 -DFINAL_TESTING -O0 -I. big_integer_testing.cpp big_integer.cpp optimized_vector.cpp gtest/gtest-all.cc gtest/gtest_main.cc 2>errors || echo ERRORS =(  
  
