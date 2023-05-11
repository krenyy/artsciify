#pragma once

#include <array>
#include <cstdint>

struct Color {
  Color() : rgb({0, 0, 0}) {}
  Color(uint8_t r, uint8_t g, uint8_t b) : rgb({r, g, b}) {}
  std::array<uint8_t, 3> rgb;
  uint8_t &r() { return rgb[0]; }
  uint8_t &g() { return rgb[1]; }
  uint8_t &b() { return rgb[2]; }
  uint8_t r() const { return rgb[0]; }
  uint8_t g() const { return rgb[1]; }
  uint8_t b() const { return rgb[2]; }
  uint8_t &operator[](const std::size_t i) { return rgb[i]; }
  uint8_t operator[](const std::size_t i) const { return rgb[i]; }
  std::array<uint8_t, 3>::const_iterator begin() const { return rgb.begin(); }
  std::array<uint8_t, 3>::const_iterator end() const { return rgb.end(); }
  std::array<uint8_t, 3>::iterator begin() { return rgb.begin(); }
  std::array<uint8_t, 3>::iterator end() { return rgb.end(); }
};
