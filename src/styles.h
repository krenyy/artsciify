#ifndef ARTSCIIFY_STYLES_H
#define ARTSCIIFY_STYLES_H

#include "image.h"
#include <sstream>

std::string rgb_to_color_code(const int r, const int g, const int b);

class PrintStyle {
public:
  virtual std::string print(const Image &img) const = 0;
};

class AsciiArt : public PrintStyle {
public:
  std::string print(const Image &img) const override;
};

class BlockArt : public PrintStyle {
public:
  std::string print(const Image &img) const override;
};

#endif
