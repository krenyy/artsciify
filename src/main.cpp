#include "image.h"
#include "styles.h"
#include <iostream>

int main() {
  auto x = PngImage::read("stepech.png");
  std::cerr << GrayscaleBlockArt().print(x.value());
  return 0;
}
