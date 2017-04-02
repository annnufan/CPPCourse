g++ -std=gnu++11  -Wl,--stack=256000000 -DFINAL_TESTING -O2 -I. big_integer_testing.cpp big_integer.cpp gtest/gtest-all.cc gtest/gtest_main.cc 1>good 2>errors || echo ERRORS =(  
  
