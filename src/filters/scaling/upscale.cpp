#include "upscale.h"
#include <iostream>

void Upscale::apply(Image &img) const {
  size_t old_height = img.height();
  size_t old_width = img.width();
  img.set_height(old_height * 2);
  img.set_width(old_width * 2);
  for (size_t i = 0; i < old_height; ++i) {
    for (size_t j = 0; j < img.width(); ++j) {
      img[i][img.width() - j - 1] = img[i][(img.width() - j - 1) / 2];
    }
  }
  for (size_t i = 0; i < img.height(); ++i) {
    img[img.height() - i - 1] = img[(img.height() - i - 1) / 2];
  }
}
void Upscale::get_final_dimensions(size_t &width, size_t &height,
                                   size_t &max_width,
                                   size_t &max_height) const {
  width *= 2;
  height *= 2;
  max_width = width > max_width ? width : max_width;
  max_height = height > max_height ? height : max_height;
}
