#pragma once

#include "color.h"
#include <vector>

class Image {
public:
  using Row = std::vector<Color>;

  Image(std::size_t height, std::size_t width);

  std::size_t height() const;
  std::size_t width() const;

  void set_height(std::size_t height);
  void set_width(std::size_t width);

  Row &operator[](const std::size_t i);
  Row operator[](const std::size_t i) const;

  std::vector<Row>::const_iterator begin() const;
  std::vector<Row>::const_iterator end() const;

  std::vector<Row>::iterator begin();
  std::vector<Row>::iterator end();

private:
  std::vector<Row> image;
};
