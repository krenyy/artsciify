#ifndef ARTSCIIFY_STYLES_H
#define ARTSCIIFY_STYLES_H

#include "image.h"
#include <map>
#include <memory>
#include <sstream>

std::string rgb_to_color_code(const int r, const int g, const int b);

class PixelTransform {
public:
  virtual ~PixelTransform() = default;
  virtual void transform(std::string &s, unsigned char r, unsigned char g,
                         unsigned char b) const = 0;
};
class TextTransform : public PixelTransform {};
class ColorTransform : public PixelTransform {};

class StringTextTransform : public TextTransform {
public:
  StringTextTransform(std::string str);
  void transform(std::string &str, const unsigned char r, const unsigned char g,
                 const unsigned char b) const override;

private:
  std::string s;
};

class AsciiTextTransform : public TextTransform {
public:
  class Map : public std::map<double, unsigned char> {
  public:
    static Map build(std::string characters, std::vector<double> brightnesses);
    static Map standard();
    static Map eddie_smith();
  };

  AsciiTextTransform(const double brightness_r, const double brightness_g,
                     const double brightness_b, Map map);
  void transform(std::string &s, const unsigned char r, const unsigned char g,
                 const unsigned char b) const override;

private:
  double br;
  double bg;
  double bb;
  Map m;
};

class FromPixelForegroundColorTransform : public ColorTransform {
public:
  void transform(std::string &s, unsigned char r, unsigned char g,
                 unsigned char b) const override;
};

class FromPixelBackgroundColorTransform : public ColorTransform {
public:
  void transform(std::string &s, unsigned char r, unsigned char g,
                 unsigned char b) const override;
};

class ForegroundColorTransform : public ColorTransform {
public:
  ForegroundColorTransform(unsigned char r, unsigned char g, unsigned char b);
  void transform(std::string &s, unsigned char r, unsigned char g,
                 unsigned char b) const override;

private:
  unsigned char r;
  unsigned char g;
  unsigned char b;
};

class BackgroundColorTransform : public ColorTransform {
public:
  BackgroundColorTransform(unsigned char r, unsigned char g, unsigned char b);
  void transform(std::string &s, unsigned char r, unsigned char g,
                 unsigned char b) const override;

private:
  unsigned char r;
  unsigned char g;
  unsigned char b;
};

class ArtStyle {
public:
  ArtStyle(std::shared_ptr<TextTransform> text_transform,
           std::vector<std::shared_ptr<ColorTransform>> color_transforms);
  static ArtStyle ascii_standard();
  static ArtStyle ascii_standard_color();
  static ArtStyle ascii_eddie_smith();
  static ArtStyle ascii_eddie_smith_color();
  static ArtStyle character(const unsigned char c);
  static ArtStyle block();
  std::string print(const Image &img) const;

private:
  std::shared_ptr<TextTransform> text_transform;
  std::vector<std::shared_ptr<ColorTransform>> color_transforms;
};

#endif
