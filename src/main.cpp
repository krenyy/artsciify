#include "some_file.h"
#include <iostream>

int main() {
  auto x = SomeStruct();
  auto y = x.some_method();
  std::cerr << y << std::endl;

  return 0;
}
