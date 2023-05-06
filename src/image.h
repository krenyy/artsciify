#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <png.h>
#include <string>
#include <vector>

struct Color {
  std::array<uint8_t, 3> rgb;
  uint8_t &r() { return rgb[0]; }
  uint8_t &g() { return rgb[1]; }
  uint8_t &b() { return rgb[2]; }
  uint8_t r() const { return rgb[0]; }
  uint8_t g() const { return rgb[1]; }
  uint8_t b() const { return rgb[2]; }
  uint8_t &operator[](const size_t i) { return rgb[i]; }
  uint8_t operator[](const size_t i) const { return rgb[i]; }
  std::array<uint8_t, 3>::const_iterator begin() const { return rgb.begin(); }
  std::array<uint8_t, 3>::const_iterator end() const { return rgb.end(); }
  std::array<uint8_t, 3>::iterator begin() { return rgb.begin(); }
  std::array<uint8_t, 3>::iterator end() { return rgb.end(); }
};

class Image {
public:
  using Row = std::vector<Color>;

  Image(size_t height, size_t width) : image(height, Row(width)) {}
  size_t height() const { return image.size(); }
  size_t width() const { return image.empty() ? 0 : image.size(); }
  Row &operator[](const size_t i) { return image[i]; }
  Row operator[](const size_t i) const { return image[i]; }
  std::vector<Row>::const_iterator begin() const { return image.begin(); }
  std::vector<Row>::const_iterator end() const { return image.end(); }
  std::vector<Row>::iterator begin() { return image.begin(); }
  std::vector<Row>::iterator end() { return image.end(); }

private:
  std::vector<Row> image;
};

class PngImage {
public:
  static std::optional<Image> read(const std::string &filename);
};
