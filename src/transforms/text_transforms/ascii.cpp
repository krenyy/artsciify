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
AsciiTextTransform::Map AsciiTextTransform::Map::read(ConfigReader &cr) {
  auto gradient_opt = cr.read_line();
  if (!gradient_opt.has_value()) {
    throw cr.except("Couldn't read gradient!");
  }
  std::string gradient = std::move(*gradient_opt);
  cr.next_line();
  std::vector<double> weights;
  for (size_t i = 0; i < gradient.size(); ++i) {
    auto weight_opt = cr.read_double();
    if (weight_opt.has_value()) {
      weights.push_back(std::move(*weight_opt));
      if (i != gradient.size() - 1) {
        cr.assert_char({' '});
      }
      continue;
    }
    if (weights.size() == 0) {
      break;
    }
    throw cr.except("Couldn't read weight!");
  }
  auto map_opt = AsciiTextTransform::Map::build(gradient, weights);
  if (!map_opt.has_value()) {
    throw cr.except("Number of gradient characters and weights doesn't match!");
  }
  return std::move(*map_opt);
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
AsciiTextTransform AsciiTextTransform::read(
    ConfigReader &cr,
    std::map<std::string, std::unordered_set<std::string>> &names,
    const std::map<std::string, AsciiTextTransform::Map> &gradients,
    const std::map<std::string, Luminance> &luminances) {
  auto lum_name_opt = cr.assert_word(names["luminance"]);
  if (!lum_name_opt.has_value()) {
    throw cr.except("Missing luminance name!");
  }
  std::string lum_name = std::move(*lum_name_opt);
  if (!luminances.count(lum_name)) {
    throw cr.except("Unknown luminance: '" + lum_name + "'!");
  }
  cr.assert_char({' '});
  auto gradient_name_opt = cr.assert_word(names["gradient"]);
  if (!gradient_name_opt.has_value()) {
    throw cr.except("Missing gradient name!");
  }
  std::string gradient_name = std::move(*gradient_name_opt);
  if (!gradients.count(gradient_name)) {
    throw cr.except("Unknown gradient: '" + gradient_name + "'!");
  }
  return {luminances.at(lum_name), gradients.at(gradient_name)};
}
