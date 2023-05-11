#include "foreground.h"

ForegroundColorTransform::ForegroundColorTransform(Color c) : color(c) {}

void ForegroundColorTransform::apply(std::string &s, Color) const {
  s.insert(0, rgb_to_fg_color_code(color));
}
