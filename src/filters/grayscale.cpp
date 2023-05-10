#include "grayscale.h"

Grayscale::Grayscale(Luminance luminance) : lum(std::move(luminance)) {}
void Grayscale::apply(Image &img) const {
  for (Image::Row &row : img) {
    for (Color &pixel : row) {
      pixel.r() = pixel.g() = pixel.b() = static_cast<uint8_t>(
          lum.r * pixel.r() + lum.g * pixel.g() + lum.b * pixel.b());
    }
  }
}
