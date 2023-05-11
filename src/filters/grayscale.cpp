#include "grayscale.h"

Grayscale::Grayscale(Luminance luminance) : lum(std::move(luminance)) {}
void Grayscale::apply(Image &img) const {
  for (Image::Row &row : img) {
    for (Color &pixel : row) {
      pixel.r() = pixel.g() = pixel.b() = static_cast<uint8_t>(
          lum.r * pixel.r() + lum.g * pixel.g() + lum.b * pixel.b());
    }
  }
}

Grayscale
Grayscale::read(ConfigReader &cr,
                std::map<std::string, std::unordered_set<std::string>> &names,
                const std::map<std::string, Luminance> &luminances) {
  cr.assert_char({' '});
  auto lum_name_opt = cr.assert_word(names["luminance"]);
  if (!lum_name_opt.has_value()) {
    throw cr.except("Missing luminance value!");
  }
  return {luminances.at(std::move(*lum_name_opt))};
}
