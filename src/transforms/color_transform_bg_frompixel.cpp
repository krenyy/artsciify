#include "color_transform_bg_frompixel.h"
#include "util.h"

void FromPixelBackgroundColorTransform::apply(std::string &s,
                                              const Color pixel) const {
  s.insert(0, rgb_to_bg_color_code(pixel));
}
