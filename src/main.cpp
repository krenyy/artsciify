#include "image.h"
#include <iostream>
#include <optional>
#include <vector>


int main() {
  auto x = PngImage::read("stepech.png");
  return 0;
}
