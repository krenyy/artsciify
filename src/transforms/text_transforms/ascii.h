#pragma once

#include "../../luminance.h"
#include "text_transform.h"
#include <map>
#include <optional>
#include <vector>

class AsciiTextTransform : public TextTransform {
public:
  class Map : public std::map<double, unsigned char> {
  public:
    static std::optional<Map> build(std::string characters,
                                    std::vector<double> brightnesses);
  };

  AsciiTextTransform(Luminance luminance, Map map);
  void apply(std::string &s, const Color pixel) const override;

private:
  Luminance lum;
  Map m;
};
