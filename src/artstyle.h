#pragma once

#include "config_reader.h"
#include "image.h"
#include "transforms/color_transforms/color_transform.h"
#include "transforms/text_transforms/ascii.h"
#include "transforms/text_transforms/text_transform.h"
#include <map>

/// Represents an artstyle having exactly one text transform and zero or more
/// color transforms
class ArtStyle {
public:
  ArtStyle(std::shared_ptr<TextTransform> text_transform,
           std::vector<std::shared_ptr<ColorTransform>> color_transforms);
  /// Creates a string containing an Image, printed using this ArtStyle.
  ///
  /// \param img an image to print
  /// \returns the printed image
  std::string print(const Image &img) const;

  /// Reads an ArtStyle using a ConfigReader instance and other parameters.
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
