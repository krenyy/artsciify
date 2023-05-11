#pragma once

#include "color_transform.h"
#include <map>

class ForegroundColorTransform : public ColorTransform {
public:
  ForegroundColorTransform(const Color c);
  void apply(std::string &s, const Color pixel) const override;

  static ForegroundColorTransform
  read(ConfigReader &cr,
       std::map<std::string, std::unordered_set<std::string>> &names,
       const std::map<std::string, Color> &colors);

private:
  Color color;
};
