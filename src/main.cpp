#include "filters.h"
#include "image.h"
#include "styles.h"
#include <iostream>
#include <memory>

int main(int argc, char **argv) {
  std::cerr << argc << std::endl;
  for (int i = 0; i < argc; ++i) {
    std::cerr << "  " << argv[i] << std::endl;
  }
  auto x = PngImage::read("stepech.png");
  auto img = x.value();
  // Negative().apply(img);
  // Grayscale(0.2126, 0.7152, 0.0722).apply(img);
  // Brightness(.125).apply(img);
  // std::cerr << ArtStyle(
  //                  std::make_shared<StringTextTransform>("🍆"),
  //                  {
  //                      std::make_shared<FromPixelBackgroundColorTransform>(),
  //                  })
  //                  .print(img);
  std::cerr << ArtStyle::ascii_eddie_smith_color().print(img);
  return 0;
}
