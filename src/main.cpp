#include "filters.h"
#include "image.h"
#include "styles.h"
#include <iostream>

int main() {
  auto x = PngImage::read("pepa.png");
  auto img = x.value();
  // Negative().apply(img);
  // Grayscale(0.2126, 0.7152, 0.0722).apply(img);
  // Brightness(.125).apply(img);
  std::cerr << AsciiArt(0.2126, 0.7152, 0.0722, AsciiArt::Map::eddie_smith())
                   .print(img);
  return 0;
}
