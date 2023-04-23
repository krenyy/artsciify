#include "filters.h"
#include "image.h"
#include "styles.h"
#include <iostream>

int main() {
  auto x = PngImage::read("stepech.png");
  auto img = x.value();
  Negative().apply(img);
  std::cerr << ColorBlockArt().print(img);
  return 0;
}
