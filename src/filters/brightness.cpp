#include "brightness.h"

Brightness::Brightness(double multiplier) : m(multiplier) {}
void Brightness::apply(Image &img) const {
  for (Image::Row &row : img) {
    for (Color &pixel : row) {
      for (uint8_t &c : pixel) {
        double tmp = c * m;
        c = tmp > 255 ? 255 : static_cast<uint8_t>(tmp);
      }
    }
  }
}
