#pragma once

#include "../image.h"

class Filter {
public:
  virtual ~Filter() = default;
  virtual void apply(Image &img) const = 0;
  virtual void apply_without_scaling(Image &img) const { apply(img); }
  virtual void get_final_dimensions(size_t &, size_t &, size_t &,
                                    size_t &) const {}
};
