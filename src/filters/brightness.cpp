#include "brightness.h"

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
