#pragma once

#include "color_transform.h"

class FromPixelForegroundColorTransform : public ColorTransform {
public:
  void transform(std::string &s, const Color pixel) const override;
};
