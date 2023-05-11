#include "foreground_from_pixel.h"

void FromPixelForegroundColorTransform::apply(std::string &s,
                                              const Color pixel) const {
  s.insert(0, rgb_to_fg_color_code(pixel));
}
