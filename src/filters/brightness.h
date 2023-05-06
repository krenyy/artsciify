#pragma once

#include "filter.h"

class Brightness : public Filter {
public:
  Brightness(double multiplier);
  void apply(Image &img) const override;

private:
  double m;
};
