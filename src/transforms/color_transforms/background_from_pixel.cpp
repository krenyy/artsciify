#include "background_from_pixel.h"

void FromPixelBackgroundColorTransform::apply(std::string &s,
                                              const Color pixel) const {
  s.insert(0, rgb_to_bg_color_code(pixel));
}
