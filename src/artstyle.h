#pragma once

#include "config_reader.h"
#include "image.h"
#include "transforms/color_transforms/color_transform.h"
#include "transforms/text_transforms/ascii.h"
#include "transforms/text_transforms/text_transform.h"
#include <map>

class ArtStyle {
public:
  ArtStyle(std::shared_ptr<TextTransform> text_transform,
           std::vector<std::shared_ptr<ColorTransform>> color_transforms);
  std::string print(const Image &img) const;

  static void
  read(ConfigReader &cr, std::string name, const bool color_present,
       std::map<std::string, std::unordered_set<std::string>> &names,
       const std::map<std::string, AsciiTextTransform::Map> &gradients,
       const std::map<std::string, Luminance> &luminances,
       const std::map<std::string, Color> &colors,
       std::map<std::string, ArtStyle> &styles);

private:
  std::shared_ptr<TextTransform> text_transform;
  std::vector<std::shared_ptr<ColorTransform>> color_transforms;
};
