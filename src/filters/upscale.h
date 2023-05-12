#pragma once

#include "filter.h"

class Upscale : public Filter {
public:
  void apply(Image &img) const override;
};
