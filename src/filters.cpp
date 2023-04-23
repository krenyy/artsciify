#include "filters.h"
#include "image.h"

void Grayscale::apply(Image &img) const {
  size_t height = img[0].size(), width = img[0][0].size();
  for (size_t i = 0; i < height; ++i) {
    for (size_t j = 0; j < width; ++j) {
      double px_brightness =
          0.2126 * img[0][i][j] + 0.7152 * img[1][i][j] + 0.0722 * img[2][i][j];
      img[0][i][j] = img[1][i][j] = img[2][i][j] = px_brightness;
    }
  }
}

void Negative::apply(Image &img) const {
  for (auto &channel : img) {
    for (auto &row : channel) {
      for (auto &px : row) {
        px = 255 - px;
      }
    }
  }
}
