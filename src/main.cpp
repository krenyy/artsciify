#include "filters.h"
#include "image.h"
#include "styles.h"
#include <iostream>

int main() {
  auto x = PngImage::read("stepech.png");
  auto img = x.value();
  Negative().apply(img);
  Grayscale().apply(img);
  std::cerr << BlockArt().print(img);
  return 0;
}
