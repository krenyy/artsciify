#include "styles.h"
#include "image.h"
#include <algorithm>
#include <memory>
#include <sstream>

std::string rgb_to_fg_color_code(const int r, const int g, const int b) {
  std::ostringstream oss;
  oss << "\033[38;2;" << r << ";" << g << ";" << b << "m";
  return oss.str();
}

std::string rgb_to_bg_color_code(const int r, const int g, const int b) {
  std::ostringstream oss;
  oss << "\033[48;2;" << r << ";" << g << ";" << b << "m";
  return oss.str();
}

StringTextTransform::StringTextTransform(std::string str) : s(str) {}
void StringTextTransform::transform(std::string &str, const unsigned char,
                                    const unsigned char,
                                    const unsigned char) const {
  str += s;
}

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
// https://stackoverflow.com/a/67780964
AsciiTextTransform::Map AsciiTextTransform::Map::standard() {
  return build(".'`^\",:;Il!i><~+_-?][}{1)(|\\/"
               "tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$");
}
// https://stackoverflow.com/a/74186686
AsciiTextTransform::Map AsciiTextTransform::Map::eddie_smith() {
  return build(
      " `.-':_,^=;><+!rc*/"
      "z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]"
      "2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@",
      {0,      0.0751, 0.0829, 0.0848, 0.1227, 0.1403, 0.1559, 0.185,  0.2183,
       0.2417, 0.2571, 0.2852, 0.2902, 0.2919, 0.3099, 0.3192, 0.3232, 0.3294,
       0.3384, 0.3609, 0.3619, 0.3667, 0.3737, 0.3747, 0.3838, 0.3921, 0.396,
       0.3984, 0.3993, 0.4075, 0.4091, 0.4101, 0.42,   0.423,  0.4247, 0.4274,
       0.4293, 0.4328, 0.4382, 0.4385, 0.442,  0.4473, 0.4477, 0.4503, 0.4562,
       0.458,  0.461,  0.4638, 0.4667, 0.4686, 0.4693, 0.4703, 0.4833, 0.4881,
       0.4944, 0.4953, 0.4992, 0.5509, 0.5567, 0.5569, 0.5591, 0.5602, 0.5602,
       0.565,  0.5776, 0.5777, 0.5818, 0.587,  0.5972, 0.5999, 0.6043, 0.6049,
       0.6093, 0.6099, 0.6465, 0.6561, 0.6595, 0.6631, 0.6714, 0.6759, 0.6809,
       0.6816, 0.6925, 0.7039, 0.7086, 0.7235, 0.7302, 0.7332, 0.7602, 0.7834,
       0.8037, 0.9999}

  );
}
void AsciiTextTransform::transform(std::string &s, const unsigned char r,
                                   const unsigned char g,
                                   const unsigned char b) const {
  double px_brightness = (br * r + bg * g + bb * b) / 255;
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
                                                  unsigned char r,
                                                  unsigned char g,
                                                  unsigned char b) const {
  s.insert(0, rgb_to_fg_color_code(r, g, b));
}
void FromPixelBackgroundColorTransform::transform(std::string &s,
                                                  unsigned char r,
                                                  unsigned char g,
                                                  unsigned char b) const {
  s.insert(0, rgb_to_bg_color_code(r, g, b));
}

ForegroundColorTransform::ForegroundColorTransform(unsigned char r,
                                                   unsigned char g,
                                                   unsigned char b)
    : r(r), g(g), b(b) {}
void ForegroundColorTransform::transform(std::string &s, unsigned char,
                                         unsigned char, unsigned char) const {
  s.insert(0, rgb_to_fg_color_code(r, g, b));
}

BackgroundColorTransform::BackgroundColorTransform(unsigned char r,
                                                   unsigned char g,
                                                   unsigned char b)
    : r(r), g(g), b(b) {}
void BackgroundColorTransform::transform(std::string &s, unsigned char,
                                         unsigned char, unsigned char) const {
  s.insert(0, rgb_to_bg_color_code(r, g, b));
}

ArtStyle::ArtStyle(
    std::shared_ptr<TextTransform> text_transform,
    std::vector<std::shared_ptr<ColorTransform>> color_transforms)
    : text_transform(text_transform), color_transforms(color_transforms) {}
std::string ArtStyle::print(const Image &img) const {
  size_t height = img[0].size();
  size_t width = img[0][0].size();
  std::ostringstream oss;
  for (size_t i = 0; i < height; ++i) {
    for (size_t j = 0; j < width; ++j) {
      unsigned char r = img[0][i][j];
      unsigned char g = img[1][i][j];
      unsigned char b = img[2][i][j];
      std::string s;
      text_transform->transform(s, r, g, b);
      for (const auto &t : color_transforms) {
        t->transform(s, r, g, b);
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
ArtStyle ArtStyle::ascii_standard() {
  return ArtStyle(
      std::make_shared<AsciiTextTransform>(0.2126, 0.7152, 0.0722,
                                           AsciiTextTransform::Map::standard()),
      {});
}
ArtStyle ArtStyle::ascii_standard_color() {
  return ArtStyle(
      std::make_shared<AsciiTextTransform>(0.2126, 0.7152, 0.0722,
                                           AsciiTextTransform::Map::standard()),
      {std::make_shared<FromPixelForegroundColorTransform>()});
}
ArtStyle ArtStyle::ascii_eddie_smith() {
  return ArtStyle(
      std::make_shared<AsciiTextTransform>(
          0.2126, 0.7152, 0.0722, AsciiTextTransform::Map::eddie_smith()),
      {});
}
ArtStyle ArtStyle::ascii_eddie_smith_color() {
  return ArtStyle(
      std::make_shared<AsciiTextTransform>(
          0.2126, 0.7152, 0.0722, AsciiTextTransform::Map::eddie_smith()),
      {std::make_shared<FromPixelForegroundColorTransform>()});
}
ArtStyle ArtStyle::block() {
  return ArtStyle(std::make_shared<StringTextTransform>(" "),
                  {
                      std::make_shared<FromPixelBackgroundColorTransform>(),
                  });
}
