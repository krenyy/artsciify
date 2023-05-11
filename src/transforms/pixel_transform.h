#pragma once

#include "../color.h"
#include <string>

class PixelTransform {
public:
  virtual ~PixelTransform() = default;
  virtual void apply(std::string &s, const Color pixel) const = 0;
};
