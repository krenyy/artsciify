#include "filters.h"
#include "image.h"
#include "styles.h"
#include <iostream>

int main() {
  auto x = PngImage::read("stepech.png");
  auto img = x.value();
  Negative().apply(img);
  Grayscale(0.2126, 0.7152, 0.0722).apply(img);
  Brightness(.125).apply(img);
  std::cerr << BlockArt().print(img);
  return 0;
}
