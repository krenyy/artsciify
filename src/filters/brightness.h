#pragma once

#include "../config_reader.h"
#include "filter.h"

/// A brightness filter
class Brightness : public Filter {
public:
  /// Constructs a Brightness instance.
  ///
  /// \param multiplier a brightness multiplier
  Brightness(double multiplier);
  /// Applies this Brightness filter.
  ///
  /// \param img image to apply the filter to
  void apply(Image &img) const override;

  /// Reads a Brightness using a ConfigReader instance.
  static Brightness read(ConfigReader &cr);

private:
  double m;
};
