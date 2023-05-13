#pragma once

#include "../filter.h"

class ScalingFilter : public Filter {
public:
  virtual void apply(Image &img) const override = 0;
  virtual void apply_without_scaling(Image &) const override {}
};
