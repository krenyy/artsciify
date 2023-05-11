#include "foreground.h"

ForegroundColorTransform::ForegroundColorTransform(Color c) : color(c) {}

void ForegroundColorTransform::apply(std::string &s, Color) const {
  s.insert(0, rgb_to_fg_color_code(color));
}

ForegroundColorTransform ForegroundColorTransform::read(
    ConfigReader &cr,
    std::map<std::string, std::unordered_set<std::string>> &names,
    const std::map<std::string, Color> &colors) {
  cr.assert_char({' '});
  auto color_name_opt = cr.assert_word(names["color"]);
  if (!color_name_opt.has_value()) {
    throw cr.except("Missing color!");
  }
  std::string color_name = std::move(*color_name_opt);
  return {colors.at(color_name)};
}
