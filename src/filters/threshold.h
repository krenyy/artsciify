#pragma once

#include "../config_reader.h"
#include "filter.h"

class Threshold : public Filter {
public:
  Threshold(uint8_t threshold);
  void apply(Image &img) const override;

  static Threshold read(ConfigReader &cr);

private:
  uint8_t t;
};
