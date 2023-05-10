#include "negative.h"

void Negative::apply(Image &img) const {
  for (Image::Row &row : img) {
    for (Color &pixel : row) {
      for (uint8_t &c : pixel) {
        c = 255 - c;
      }
    }
  }
}
