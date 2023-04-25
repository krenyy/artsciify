#ifndef ARTSCIIFY_STYLES_H
#define ARTSCIIFY_STYLES_H

#include "image.h"
#include <map>
#include <memory>
#include <queue>
#include <sstream>

std::string rgb_to_color_code(const int r, const int g, const int b);

class CharTransform {
public:
  virtual unsigned char transform(unsigned char r, unsigned char g,
                                  unsigned char b) const = 0;
};

class CharacterCharTransform : public CharTransform {
public:
  CharacterCharTransform(const unsigned char character);
  unsigned char transform(const unsigned char r, const unsigned char g,
                          const unsigned char b) const override;
};

class AsciiCharTransform : public CharTransform {
public:
  class Map : public std::map<double, unsigned char> {
  public:
    static Map build(std::string characters, std::vector<double> brightnesses);
    static Map standard();
    static Map eddie_smith();
  };

  AsciiCharTransform(const double brightness_r, const double brightness_g,
                     const double brightness_b, Map map);
  unsigned char transform(const unsigned char r, const unsigned char g,
                          const unsigned char b) const override;

private:
  double br;
  double bg;
  double bb;
  Map m;
};

class ColorTransform {
public:
  virtual std::string transform(unsigned char c, unsigned char r,
                                unsigned char g, unsigned char b) const = 0;
};

class ForegroundColorTransform : public ColorTransform {
public:
  std::string transform(unsigned char s, unsigned char r, unsigned char g,
                        unsigned char b) const override;
};

class BackgroundColorTransform : public ColorTransform {
public:
  std::string transform(unsigned char s, unsigned char r, unsigned char g,
                        unsigned char b) const override;
};

class ArtStyle {
public:
  ArtStyle(std::unique_ptr<CharTransform> text_transform,
           std::queue<std::unique_ptr<ColorTransform>> color_transforms);
  static ArtStyle ascii_standard();
  static ArtStyle ascii_standard_color();
  static ArtStyle ascii_eddie_smith();
  static ArtStyle ascii_eddie_smith_color();
  static ArtStyle character_art(const unsigned char c);
  static ArtStyle block_art();
  std::string print(const Image &img) const;

private:
  std::unique_ptr<CharTransform> text_transform;
  std::queue<std::unique_ptr<ColorTransform>> color_transforms;
};

#endif
