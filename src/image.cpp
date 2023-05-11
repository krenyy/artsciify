#include "image.h"
#include <stddef.h>

using Row = Image::Row;
using std::vector;

Image::Image(size_t height, size_t width) : image(height, Row(width)) {}

size_t Image::height() const { return image.size(); }
size_t Image::width() const { return image.empty() ? 0 : image[0].size(); }

void Image::set_height(size_t height) { image.resize(height); }
void Image::set_width(size_t width) {
  if (image.empty()) {
    return;
  }
  for (Row &row : image) {
    row.resize(width);
  }
}

Row &Image::operator[](const size_t i) { return image[i]; }
Row Image::operator[](const size_t i) const { return image[i]; }

vector<Row>::const_iterator Image::begin() const { return image.begin(); }
vector<Row>::const_iterator Image::end() const { return image.end(); }

vector<Row>::iterator Image::begin() { return image.begin(); }
vector<Row>::iterator Image::end() { return image.end(); }
