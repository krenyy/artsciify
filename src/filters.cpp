#include "filters.h"
#include "image.h"

Grayscale::Grayscale(double brightness_r, double brightness_g,
                     double brightness_b)
    : br(brightness_r), bg(brightness_g), bb(brightness_b) {}
void Grayscale::apply(Image &img) const {
  for (auto &row : img) {
    for (auto &pixel : row) {
      pixel.r() = pixel.g() = pixel.b() =
          br * pixel.r() + bg * pixel.g() + bb * pixel.b();
    }
  }
}

void Negative::apply(Image &img) const {
  for (auto &row : img) {
    for (auto &pixel : row) {
      for (auto &c : pixel) {
        c = 255 - c;
      }
    }
  }
}

Brightness::Brightness(double multiplier) : m(multiplier) {}
void Brightness::apply(Image &img) const {
  for (auto &row : img) {
    for (auto &pixel : row) {
      for (auto &c : pixel) {
        double tmp = c * m;
        c = tmp > 255 ? 255 : tmp;
      }
    }
  }
}
