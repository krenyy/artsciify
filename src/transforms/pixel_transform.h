#pragma once

#include "../image.h"
#include <string>

class PixelTransform {
public:
  virtual ~PixelTransform() = default;
  virtual void transform(std::string &s, const Color pixel) const = 0;
};
