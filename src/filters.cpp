#include "filters.h"
#include "image.h"

Grayscale::Grayscale(double brightness_r, double brightness_g,
                     double brightness_b)
    : br(brightness_r), bg(brightness_g), bb(brightness_b) {}
void Grayscale::apply(Image &img) const {
  size_t height = img[0].size(), width = img[0][0].size();
  for (size_t i = 0; i < height; ++i) {
    for (size_t j = 0; j < width; ++j) {
      ImagePixel &r = img[0][i][j];
      ImagePixel &g = img[1][i][j];
      ImagePixel &b = img[2][i][j];
      r = g = b = br * r + bg * g + bb * b;
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

Brightness::Brightness(double multiplier) : m(multiplier) {}
void Brightness::apply(Image &img) const {
  for (auto &channel : img) {
    for (auto &row : channel) {
      for (auto &px : row) {
        double tmp = px * m;
        px = tmp > 255 ? 255 : tmp;
      }
    }
  }
}
