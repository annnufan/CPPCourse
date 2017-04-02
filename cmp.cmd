g++ -std=gnu++11  -Wall -Werror -Wl,--stack=256000000 -O2 -I. big_integer.cpp optimized_vector.cpp  2>errors || echo ERRORS =(  
  
