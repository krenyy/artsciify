#include "rotate.h"
#include <iostream>

void Rotate::apply(Image &img) const {
  Image new_image = img;
  new_image.set_height(img.width());
  new_image.set_width(img.height());
  for (size_t i = 0; i < img.height(); ++i) {
    for (size_t j = 0; j < img.width(); ++j) {
      new_image[j][i] = img[i][j];
    }
  }
  for (size_t i = 0; i < (new_image.height() / 2); ++i) {
    new_image[i].swap(new_image[new_image.height() - i - 1]);
  }
  img = new_image;
}
void Rotate::get_final_dimensions(size_t &width, size_t &height,
                                  size_t &max_width, size_t &max_height) const {
  std::swap(width, height);
  std::swap(max_width, max_height);
}
