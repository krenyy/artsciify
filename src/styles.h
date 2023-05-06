#pragma once

#include "image.h"
#include <map>
#include <memory>
#include <optional>
#include <sstream>

std::string rgb_to_fg_color_code(const Color c);
std::string rgb_to_bg_color_code(const Color c);

class PixelTransform {
public:
  virtual ~PixelTransform() = default;
  virtual void transform(std::string &s, const Color pixel) const = 0;
};
class TextTransform : public PixelTransform {};
class ColorTransform : public PixelTransform {};

class StringTextTransform : public TextTransform {
public:
  StringTextTransform(std::string str);
  void transform(std::string &str, const Color pixel) const override;

private:
  std::string s;
};

struct Luminance {
  double r;
  double g;
  double b;
  Luminance(double brightness_r, double brightness_g, double brightness_b)
      : r(brightness_r), g(brightness_g), b(brightness_b) {}
};

class AsciiTextTransform : public TextTransform {
public:
  class Map : public std::map<double, unsigned char> {
  public:
    static std::optional<Map> build(std::string characters,
                                    std::vector<double> brightnesses);
  };

  AsciiTextTransform(Luminance luminance, Map map);
  void transform(std::string &s, const Color pixel) const;

private:
  Luminance lum;
  Map m;
};

class FromPixelForegroundColorTransform : public ColorTransform {
public:
  void transform(std::string &s, const Color pixel) const override;
};

class FromPixelBackgroundColorTransform : public ColorTransform {
public:
  void transform(std::string &s, const Color pixel) const override;
};

class ForegroundColorTransform : public ColorTransform {
public:
  ForegroundColorTransform(const Color c);
  void transform(std::string &s, const Color pixel) const override;

private:
  Color color;
};

class BackgroundColorTransform : public ColorTransform {
public:
  BackgroundColorTransform(const Color c);
  void transform(std::string &s, const Color pixel) const override;

private:
  Color color;
};

class ArtStyle {
public:
  ArtStyle(std::shared_ptr<TextTransform> text_transform,
           std::vector<std::shared_ptr<ColorTransform>> color_transforms);
  std::string print(const Image &img) const;

private:
  std::shared_ptr<TextTransform> text_transform;
  std::vector<std::shared_ptr<ColorTransform>> color_transforms;
};
