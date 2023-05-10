#include "color_transform_fg_frompixel.h"
#include "util.h"

void FromPixelForegroundColorTransform::apply(std::string &s,
                                              const Color pixel) const {
  s.insert(0, rgb_to_fg_color_code(pixel));
}
