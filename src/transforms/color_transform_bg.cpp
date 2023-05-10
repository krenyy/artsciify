#include "color_transform_bg.h"
#include "util.h"

BackgroundColorTransform::BackgroundColorTransform(Color c) : color(c) {}

void BackgroundColorTransform::apply(std::string &s, Color) const {
  s.insert(0, rgb_to_bg_color_code(color));
}
