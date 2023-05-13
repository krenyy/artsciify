#pragma once

#include "../config_reader.h"
#include "../luminance.h"
#include "filter.h"
#include <map>

/// A grayscale filter.
class Grayscale : public Filter {
public:
  /// Constructs a Grayscale instance.
  ///
  /// \param luminance a luminance
  Grayscale(Luminance luminance);
  /// Applies this Grayscale filter.
  ///
  /// \param img image to apply the filter to
  void apply(Image &img) const override;

  /// Reads a Grayscale using a ConfigReader instance and other params.
  static Grayscale
  read(ConfigReader &cr,
       std::map<std::string, std::unordered_set<std::string>> &names,
       const std::map<std::string, Luminance> &luminances);

private:
  Luminance lum;
};
