#pragma once

#include "color_transform.h"
#include <map>

/// A foreground color transform.
class ForegroundColorTransform : public ColorTransform {
public:
  /// Constructs a ForegroundColorTransform.
  ///
  /// \param c color to use for transform
  ForegroundColorTransform(const Color c);
  /// Applies this ForegroundColorTransform.
  ///
  /// \param s a string to apply the transform to
  /// \param pixel reference pixel
  void apply(std::string &s, const Color pixel) const override;

  /// Reads a ForegroundColorTransform using a ConfigReader instance and other
  /// params.
  static ForegroundColorTransform
  read(ConfigReader &cr,
       std::map<std::string, std::unordered_set<std::string>> &names,
       const std::map<std::string, Color> &colors);

private:
  Color color;
};
