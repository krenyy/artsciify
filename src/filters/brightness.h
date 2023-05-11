#pragma once

#include "../config_reader.h"
#include "filter.h"

class Brightness : public Filter {
public:
  Brightness(double multiplier);
  void apply(Image &img) const override;

  static Brightness read(ConfigReader &cr);

private:
  double m;
};
