#include "styles.h"
#include "image.h"
#include <algorithm>
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

AsciiArt::AsciiArt(const double brightness_r, const double brightness_g,
                   const double brightness_b, AsciiArt::Map map)
    : br(brightness_r), bg(brightness_g), bb(brightness_b), m(map) {}
AsciiArt::Map AsciiArt::Map::build(std::string characters,
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
AsciiArt::Map AsciiArt::Map::standard() {
  return build(".'`^\",:;Il!i><~+_-?][}{1)(|\\/"
               "tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$");
}
// https://stackoverflow.com/a/74186686
AsciiArt::Map AsciiArt::Map::eddie_smith() {
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
std::string AsciiArt::print(const Image &img) const {
  std::ostringstream oss;
  size_t height = img[0].size(), width = img[0][0].size();
  for (size_t i = 0; i < height; ++i) {
    for (size_t j = 0; j < width; ++j) {
      ImagePixel r = img[0][i][j];
      ImagePixel g = img[1][i][j];
      ImagePixel b = img[2][i][j];
      double px_brightness = (br * r + bg * g + bb * b) / 255;
      auto [begin, end] = m.equal_range(px_brightness);
      if (begin == m.end() || end == m.end()) {
        oss << (--m.end())->second;
        continue;
      }
      oss << ((begin->first - px_brightness < end->first - px_brightness)
                  ? begin->second
                  : end->second);
    }
    oss << std::endl;
  }
  return oss.str();
}

CharacterArt::CharacterArt(const unsigned char character) : c(character) {}
std::string CharacterArt::print(const Image &img) const {
  std::ostringstream oss;
  size_t height = img[0].size(), width = img[0][0].size();
  for (size_t i = 0; i < height; ++i) {
    for (size_t j = 0; j < width; ++j) {
      oss << rgb_to_fg_color_code(img[0][i][j], img[1][i][j], img[2][i][j]) << c
          << "\033[0;0m";
    }
    oss << std::endl;
  }
  return oss.str();
}

std::string BlockArt::print(const Image &img) const {
  std::ostringstream oss;
  size_t height = img[0].size(), width = img[0][0].size();
  for (size_t i = 0; i < height; ++i) {
    for (size_t j = 0; j < width; ++j) {
      oss << rgb_to_bg_color_code(img[0][i][j], img[1][i][j], img[2][i][j])
          << " "
          << "\033[0;0m";
    }
    oss << std::endl;
  }
  return oss.str();
}
