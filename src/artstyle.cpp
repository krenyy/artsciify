#include "artstyle.h"
#include <sstream>

ArtStyle::ArtStyle(std::shared_ptr<TextTransform> transform,
                   std::vector<std::shared_ptr<ColorTransform>> transforms)
    : text_transform(transform), color_transforms(transforms) {}
std::string ArtStyle::print(const Image &img) const {
  std::ostringstream oss;
  for (auto &row : img) {
    for (auto &pixel : row) {
      std::string s;
      text_transform->transform(s, pixel);
      for (const auto &t : color_transforms) {
        t->transform(s, pixel);
      }
      oss << s;
    }
    if (!color_transforms.empty()) {
      oss << "\033[0;0m";
    }
    oss << '\n';
  }
  return oss.str();
}
