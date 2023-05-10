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
      // apply twice to make it proportionally
      // closer to the source image as fonts
      // usually have height larger than width
      text_transform->apply(s, pixel);
      text_transform->apply(s, pixel);
      for (const auto &t : color_transforms) {
        t->apply(s, pixel);
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
