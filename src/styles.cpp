#include "styles.h"
#include "image.h"
#include <sstream>

std::string rgb_to_color_code(const int r, const int g, const int b) {
  std::ostringstream oss;
  oss << "\033[48;2;" << r << ";" << g << ";" << b << "m";
  return oss.str();
}

std::string AsciiArt::print(const Image &img) const {
  (void)img;
  std::ostringstream oss;
  return oss.str();
}

std::string BlockArt::print(const Image &img) const {
  std::ostringstream oss;
  size_t height = img[0].size(), width = img[0][0].size();
  for (size_t i = 0; i < height; ++i) {
    for (size_t j = 0; j < width; ++j) {
      oss << rgb_to_color_code(img[0][i][j], img[1][i][j], img[2][i][j]) << " "
          << "\033[0;0m";
    }
    oss << std::endl;
  }
  return oss.str();
}
