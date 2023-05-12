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
    new_image[i].swap(new_image[img.height() - i - 1]);
  }
  img = new_image;
}
