#pragma once

#include "../luminance.h"
#include "text_transform.h"
#include <map>

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
