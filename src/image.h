#ifndef ARTSCIIFY_IMAGE_H
#define ARTSCIIFY_IMAGE_H

#include <array>
#include <optional>
#include <png.h>
#include <vector>

struct Pixel {
  std::array<unsigned char, 3> rgb;
  unsigned char &r() { return rgb[0]; }
  unsigned char &g() { return rgb[1]; }
  unsigned char &b() { return rgb[2]; }
  unsigned char r() const { return rgb[0]; }
  unsigned char g() const { return rgb[1]; }
  unsigned char b() const { return rgb[2]; }
  unsigned char &operator[](const size_t i) { return rgb[i]; }
  unsigned char operator[](const size_t i) const { return rgb[i]; }
};

using ImagePixel = png_byte;
using ImageRow = std::vector<ImagePixel>;
using ImageChannel = std::vector<ImageRow>;
using Image = std::vector<ImageChannel>;

class PngImage {
public:
  static std::optional<Image> read(const char *filename);
};

#endif
