#include "filters.h"
#include "image.h"

void Negative::apply(Image &img) const {
  for (auto &channel : img) {
    for (auto &row : channel) {
      for (auto &px : row) {
        px = 255 - px;
      }
    }
  }
}
