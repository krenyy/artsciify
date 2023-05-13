#pragma once

#include "scaling_filter.h"

class Upscale : public ScalingFilter {
public:
  virtual void apply(Image &img) const override;
  virtual void get_final_dimensions(size_t &width, size_t &height,
                                    size_t &max_width,
                                    size_t &max_height) const override;
};
