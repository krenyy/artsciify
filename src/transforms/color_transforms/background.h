#pragma once

#include "../../config_reader.h"
#include "color_transform.h"
#include <map>

class BackgroundColorTransform : public ColorTransform {
public:
  BackgroundColorTransform(const Color c);
  void apply(std::string &s, const Color pixel) const override;

  static BackgroundColorTransform
  read(ConfigReader &cr,
       std::map<std::string, std::unordered_set<std::string>> &names,
       const std::map<std::string, Color> &colors);

private:
  Color color;
};
