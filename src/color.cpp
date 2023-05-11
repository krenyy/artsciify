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
