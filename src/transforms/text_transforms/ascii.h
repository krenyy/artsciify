#pragma once

#include "../../config_reader.h"
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
    static Map read(ConfigReader &cr);
  };

  AsciiTextTransform(Luminance luminance, Map map);
  void apply(std::string &s, const Color pixel) const override;

  static AsciiTextTransform
  read(ConfigReader &cr,
       std::map<std::string, std::unordered_set<std::string>> &names,
       const std::map<std::string, AsciiTextTransform::Map> &gradients,
       const std::map<std::string, Luminance> &luminances);

private:
  Luminance lum;
  Map m;
};
