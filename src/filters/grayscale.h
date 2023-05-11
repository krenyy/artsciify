#pragma once

#include "../config_reader.h"
#include "../luminance.h"
#include "filter.h"
#include <map>

class Grayscale : public Filter {
public:
  Grayscale(Luminance luminance);
  void apply(Image &img) const override;

  static Grayscale
  read(ConfigReader &cr,
       std::map<std::string, std::unordered_set<std::string>> &names,
       const std::map<std::string, Luminance> &luminances);

private:
  Luminance lum;
};
