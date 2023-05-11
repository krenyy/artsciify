#pragma once

#include "color.h"
#include <array>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <png.h>
#include <string>
#include <vector>

class Image {
public:
  using Row = std::vector<Color>;

  Image(size_t height, size_t width) : image(height, Row(width)) {}
  size_t height() const { return image.size(); }
  size_t width() const { return image.empty() ? 0 : image[0].size(); }
  void set_height(size_t height) { image.resize(height); }
  void set_width(size_t width) {
    if (image.empty()) {
      return;
    }
    for (Row &row : image) {
      row.resize(width);
    }
  }
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
  PngImage(const std::filesystem::path &path);
  std::runtime_error except(const std::string &s) const;
  Image read() const;

private:
  std::filesystem::path path;
};
