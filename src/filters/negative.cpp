#include "negative.h"

void Negative::apply(Image &img) const {
  for (auto &row : img) {
    for (auto &pixel : row) {
      for (auto &c : pixel) {
        c = 255 - c;
      }
    }
  }
}
