#include "some_file.h"

const char *SomeStruct::some_method() {
  static auto x = "hello world";
  return x;
}
