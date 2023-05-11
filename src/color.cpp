#include "color.h"

Color::Color() : rgb({0, 0, 0}) {}
Color::Color(uint8_t r, uint8_t g, uint8_t b) : rgb({r, g, b}) {}
uint8_t &Color::r() { return rgb[0]; }
uint8_t &Color::g() { return rgb[1]; }
uint8_t &Color::b() { return rgb[2]; }
uint8_t Color::r() const { return rgb[0]; }
uint8_t Color::g() const { return rgb[1]; }
uint8_t Color::b() const { return rgb[2]; }
uint8_t &Color::operator[](const std::size_t i) { return rgb[i]; }
uint8_t Color::operator[](const std::size_t i) const { return rgb[i]; }
std::array<uint8_t, 3>::const_iterator Color::begin() const {
  return rgb.begin();
}
std::array<uint8_t, 3>::const_iterator Color::end() const { return rgb.end(); }
std::array<uint8_t, 3>::iterator Color::begin() { return rgb.begin(); }
std::array<uint8_t, 3>::iterator Color::end() { return rgb.end(); }

Color Color::read(ConfigReader &cr) {
  auto r_opt = cr.read_uint8();
  if (!r_opt.has_value()) {
    throw cr.except("Missing color value for red!");
  }
  cr.assert_char({' '});
  uint8_t r = std::move(*r_opt);
  auto g_opt = cr.read_uint8();
  if (!g_opt.has_value()) {
    throw cr.except("Missing color value for green!");
  }
  cr.assert_char({' '});
  uint8_t g = std::move(*g_opt);
  auto b_opt = cr.read_uint8();
  if (!b_opt.has_value()) {
    throw cr.except("Missing color value for blue!");
  }
  uint8_t b = std::move(*b_opt);
  return {r, g, b};
}
