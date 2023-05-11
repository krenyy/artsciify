#pragma once

#include "config_reader.h"

struct Luminance {
  double r;
  double g;
  double b;
  Luminance(double brightness_r, double brightness_g, double brightness_b);

  static Luminance read(ConfigReader &cr);
};
