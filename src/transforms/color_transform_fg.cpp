#include "color_transform_fg.h"
#include "util.h"

ForegroundColorTransform::ForegroundColorTransform(Color c) : color(c) {}

void ForegroundColorTransform::apply(std::string &s, Color) const {
  s.insert(0, rgb_to_fg_color_code(color));
}
