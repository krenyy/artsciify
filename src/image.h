#pragma once

#include "color.h"
#include <vector>

class Image {
public:
  using Row = std::vector<Color>;

  Image(std::size_t height, std::size_t width) : image(height, Row(width)) {}
  std::size_t height() const { return image.size(); }
  std::size_t width() const { return image.empty() ? 0 : image[0].size(); }
  void set_height(std::size_t height) { image.resize(height); }
  void set_width(std::size_t width) {
    if (image.empty()) {
      return;
    }
    for (Row &row : image) {
      row.resize(width);
    }
  }
  Row &operator[](const std::size_t i) { return image[i]; }
  Row operator[](const std::size_t i) const { return image[i]; }
  std::vector<Row>::const_iterator begin() const { return image.begin(); }
  std::vector<Row>::const_iterator end() const { return image.end(); }
  std::vector<Row>::iterator begin() { return image.begin(); }
  std::vector<Row>::iterator end() { return image.end(); }

private:
  std::vector<Row> image;
};
