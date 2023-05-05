#include "styles.h"
#include "image.h"
#include <algorithm>
#include <memory>
#include <sstream>

std::string rgb_to_fg_color_code(const Color c) {
  std::ostringstream oss;
  oss << "\033[38;2;" << (int)c.r() << ";" << (int)c.g() << ";" << (int)c.b()
      << "m";
  return oss.str();
}

std::string rgb_to_bg_color_code(const Color c) {
  std::ostringstream oss;
  oss << "\033[48;2;" << (int)c.r() << ";" << (int)c.g() << ";" << (int)c.b()
      << "m";
  return oss.str();
}

StringTextTransform::StringTextTransform(std::string str) : s(str) {}
void StringTextTransform::transform(std::string &str, Color) const { str += s; }

AsciiTextTransform::AsciiTextTransform(const double brightness_r,
                                       const double brightness_g,
                                       const double brightness_b,
                                       AsciiTextTransform::Map map)
    : br(brightness_r), bg(brightness_g), bb(brightness_b), m(map) {}
AsciiTextTransform::Map
AsciiTextTransform::Map::build(std::string characters,
                               std::vector<double> brightnesses = {}) {
  Map m;
  if (brightnesses.empty()) {
    for (size_t i = 0; i < characters.size(); ++i) {
      brightnesses.push_back(static_cast<double>(i) / characters.size());
    }
  }
  if (characters.size() != brightnesses.size()) {
    throw 0;
  }
  for (size_t i = 0; i < characters.size(); ++i) {
    m.emplace(brightnesses[i], characters[i]);
  }
  return m;
}
void AsciiTextTransform::transform(std::string &s, const Color pixel) const {
  double px_brightness =
      (br * pixel.r() + bg * pixel.g() + bb * pixel.b()) / 255;
  auto [begin, end] = m.equal_range(px_brightness);
  // returns a character which has the closest
  // brightness value to the current pixel
  s += (begin == m.end() || end == m.end())
           ? (--m.end())->second
           : (((begin->first - px_brightness < end->first - px_brightness)
                   ? begin->second
                   : end->second));
}

void FromPixelForegroundColorTransform::transform(std::string &s,
                                                  const Color pixel) const {
  s.insert(0, rgb_to_fg_color_code(pixel));
}
void FromPixelBackgroundColorTransform::transform(std::string &s,
                                                  const Color pixel) const {
  s.insert(0, rgb_to_bg_color_code(pixel));
}

ForegroundColorTransform::ForegroundColorTransform(const Color pixel)
    : pixel(pixel) {}
void ForegroundColorTransform::transform(std::string &s, Color) const {
  s.insert(0, rgb_to_fg_color_code(pixel));
}

BackgroundColorTransform::BackgroundColorTransform(const Color pixel)
    : pixel(pixel) {}
void BackgroundColorTransform::transform(std::string &s, Color) const {
  s.insert(0, rgb_to_bg_color_code(pixel));
}

ArtStyle::ArtStyle(
    std::shared_ptr<TextTransform> text_transform,
    std::vector<std::shared_ptr<ColorTransform>> color_transforms)
    : text_transform(text_transform), color_transforms(color_transforms) {}
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
