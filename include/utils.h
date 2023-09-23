#include <iostream>

#ifdef DEBUG
#define debug(str) std::cout << str << std::endl;
#else
#define debug(str)
#endif

#define info(str) std::cout << str << std::endl;

#define error(str)                                                             \
  std::cout << str << std::endl;                                               \
  std::exit(-1);
