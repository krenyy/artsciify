#ifndef ARTSCIIFY_STYLES_H
#define ARTSCIIFY_STYLES_H

#include "image.h"
#include <sstream>

std::string rgb_to_color_code(const int r, const int g, const int b);

class ArtStyle {
public:
  virtual std::string print(const Image &img) const = 0;
};

class AsciiArt : public ArtStyle {
public:
  std::string print(const Image &img) const override;
};

class CharacterArt : public ArtStyle {
public:
  CharacterArt(const unsigned char character);
  std::string print(const Image &img) const override;

private:
  unsigned char c;
};

class BlockArt : public ArtStyle {
public:
  std::string print(const Image &img) const override;
};

#endif
