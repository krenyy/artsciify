#pragma once

#include "config_reader.h"

/// A structure representing a luminance value.
struct Luminance {
  double r;
  double g;
  double b;
  Luminance(double brightness_r, double brightness_g, double brightness_b);

  /// Reads a Luminance using a ConfigReader instance.
  static Luminance read(ConfigReader &cr);
};
