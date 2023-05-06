#include "grayscale.h"

Grayscale::Grayscale(Luminance luminance) : lum(std::move(luminance)) {}
void Grayscale::apply(Image &img) const {
  for (auto &row : img) {
    for (auto &pixel : row) {
      pixel.r() = pixel.g() = pixel.b() = static_cast<uint8_t>(
          lum.r * pixel.r() + lum.g * pixel.g() + lum.b * pixel.b());
    }
  }
}
