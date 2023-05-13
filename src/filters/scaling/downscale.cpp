#include "downscale.h"

void Downscale::apply(Image &img) const {
  size_t new_height = (img.height() + 1) / 2;
  size_t new_width = (img.width() + 1) / 2;
  for (size_t i = 0; i < new_height; ++i) {
    img[i] = img[i * 2];
    for (size_t j = 0; j < new_width; ++j) {
      img[i][j] = img[i][j * 2];
    }
  }
  img.set_height(new_height);
  img.set_width(new_width);
}
void Downscale::get_final_dimensions(size_t &width, size_t &height, size_t &,
                                     size_t &) const {
  width = (width + 1) / 2;
  height = (height + 1) / 2;
}
