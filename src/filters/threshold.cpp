#include "threshold.h"

Threshold::Threshold(uint8_t threshold) : t(threshold) {}
void Threshold::apply(Image &img) const {
  for (Image::Row &row : img) {
    for (Color &pixel : row) {
      for (uint8_t &c : pixel) {
        c = c >= t ? 255 : 0;
      }
    }
  }
}

Threshold Threshold::read(ConfigReader &cr) {
  cr.assert_char({' '});
  auto t_opt = cr.read_uint8();
  if (!t_opt.has_value()) {
    throw cr.except("Missing threshold value!");
  }
  return {std::move(*t_opt)};
}
