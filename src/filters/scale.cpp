#include "scale.h"
#include <iostream>

void Scale::apply(Image &img) const {
  for (size_t i = 0; i < (img.height() + 1) / 2; ++i) {
    img[i] = img[i * 2];
    for (size_t j = 0; j < (img.width() + 1) / 2; ++j) {
      img[i][j] = img[i][j * 2];
    }
  }
  img.set_height((img.height() + 1) / 2);
  img.set_width((img.width() + 1) / 2);
}
