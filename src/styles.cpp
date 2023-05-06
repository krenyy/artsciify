#include "styles.h"
#include "image.h"
#include <algorithm>
#include <memory>
#include <sstream>

std::string rgb_to_fg_color_code(const Color c) {
  return "\033[38;2;" + std::to_string((int)c.r()) + ";" +
         std::to_string((int)c.g()) + ";" + std::to_string((int)c.b()) + "m";
}

std::string rgb_to_bg_color_code(const Color c) {
  return "\033[48;2;" + std::to_string((int)c.r()) + ";" +
         std::to_string((int)c.g()) + ";" + std::to_string((int)c.b()) + "m";
}

StringTextTransform::StringTextTransform(std::string str) : s(str) {}
void StringTextTransform::transform(std::string &str, Color) const { str += s; }

AsciiTextTransform::AsciiTextTransform(Luminance luminance,
                                       AsciiTextTransform::Map map)
    : lum(std::move(luminance)), m(std::move(map)) {}
std::optional<AsciiTextTransform::Map>
AsciiTextTransform::Map::build(std::string characters,
                               std::vector<double> brightnesses = {}) {
  Map m;
  if (brightnesses.empty()) {
    for (size_t i = 0; i < characters.size(); ++i) {
      brightnesses.push_back(static_cast<double>(i) /
                             static_cast<double>(characters.size()));
    }
  }
  if (characters.size() != brightnesses.size()) {
    return std::nullopt;
  }
  for (size_t i = 0; i < characters.size(); ++i) {
    m.emplace(brightnesses[i], characters[i]);
  }
  return m;
}
void AsciiTextTransform::transform(std::string &s, const Color pixel) const {
  double px_brightness =
      (lum.r * pixel.r() + lum.g * pixel.g() + lum.b * pixel.b()) / 255;
  auto [begin, end] = m.equal_range(px_brightness);
  // returns a character which has the closest
  // brightness value to the current pixel
  s += static_cast<char>(
      (begin == m.end() || end == m.end())
          ? (--m.end())->second
          : (((begin->first - px_brightness < end->first - px_brightness)
                  ? begin->second
                  : end->second)));
}

void FromPixelForegroundColorTransform::transform(std::string &s,
                                                  const Color pixel) const {
  s.insert(0, rgb_to_fg_color_code(pixel));
}
void FromPixelBackgroundColorTransform::transform(std::string &s,
                                                  const Color pixel) const {
  s.insert(0, rgb_to_bg_color_code(pixel));
}

ForegroundColorTransform::ForegroundColorTransform(Color c) : color(c) {}
void ForegroundColorTransform::transform(std::string &s, Color) const {
  s.insert(0, rgb_to_fg_color_code(color));
}

BackgroundColorTransform::BackgroundColorTransform(Color c) : color(c) {}
void BackgroundColorTransform::transform(std::string &s, Color) const {
  s.insert(0, rgb_to_bg_color_code(color));
}

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
