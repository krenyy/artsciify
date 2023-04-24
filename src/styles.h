#ifndef ARTSCIIFY_STYLES_H
#define ARTSCIIFY_STYLES_H

#include "image.h"
#include <map>
#include <sstream>

std::string rgb_to_color_code(const int r, const int g, const int b);

class ArtStyle {
public:
  virtual std::string print(const Image &img) const = 0;
};

class AsciiArt : public ArtStyle {
public:
  class Map : public std::map<double, unsigned char> {
  public:
    static Map build(std::string characters, std::vector<double> brightnesses);
    static Map standard();
    static Map eddie_smith();
  };

  AsciiArt(const double brightness_r, const double brightness_g,
           const double brightness_b, Map map);
  std::string print(const Image &img) const override;

private:
  double br;
  double bg;
  double bb;
  Map m;
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
