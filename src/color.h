#pragma once

#include "config_reader.h"
#include <array>
#include <cstdint>

struct Color {
  Color();
  Color(uint8_t r, uint8_t g, uint8_t b);
  uint8_t &r();
  uint8_t &g();
  uint8_t &b();
  uint8_t r() const;
  uint8_t g() const;
  uint8_t b() const;
  uint8_t &operator[](const std::size_t i);
  uint8_t operator[](const std::size_t i) const;
  std::array<uint8_t, 3>::const_iterator begin() const;
  std::array<uint8_t, 3>::const_iterator end() const;
  std::array<uint8_t, 3>::iterator begin();
  std::array<uint8_t, 3>::iterator end();

  static Color read(ConfigReader &cr);

  std::array<uint8_t, 3> rgb;
};
