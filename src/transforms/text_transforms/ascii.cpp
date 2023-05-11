#include "ascii.h"

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

AsciiTextTransform::AsciiTextTransform(Luminance luminance,
                                       AsciiTextTransform::Map map)
    : lum(std::move(luminance)), m(std::move(map)) {}

void AsciiTextTransform::apply(std::string &s, const Color pixel) const {
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
