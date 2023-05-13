#pragma once

#include "../../config_reader.h"
#include "color_transform.h"
#include <map>

/// A background color transform.
class BackgroundColorTransform : public ColorTransform {
public:
  /// Constructs a BackgroundColorTransform.
  ///
  /// \param c color to use for transform
  BackgroundColorTransform(const Color c);
  /// Applies this BackgroundColorTransform.
  ///
  /// \param s a string to apply the transform to
  /// \param pixel reference pixel
  void apply(std::string &s, const Color pixel) const override;

  /// Reads a BackgroundColorTransform using a ConfigReader instance and other
  /// params.
  static BackgroundColorTransform
  read(ConfigReader &cr,
       std::map<std::string, std::unordered_set<std::string>> &names,
       const std::map<std::string, Color> &colors);

private:
  Color color;
};
