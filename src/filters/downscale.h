#pragma once

#include "filter.h"

class Downscale : public Filter {
public:
  void apply(Image &img) const override;
};
