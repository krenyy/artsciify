#ifndef ARTSCIIFY_IMAGE_H
#define ARTSCIIFY_IMAGE_H

#include <optional>
#include <png.h>
#include <vector>

using ImagePixel = png_byte;
using ImageRow = std::vector<ImagePixel>;
using ImageChannel = std::vector<ImageRow>;
using Image = std::vector<ImageChannel>;

class PngImage {
public:
  static std::optional<Image> read(const char *filename);
};

#endif
