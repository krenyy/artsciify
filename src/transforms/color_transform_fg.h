#pragma once

#include "color_transform.h"

class ForegroundColorTransform : public ColorTransform {
public:
  ForegroundColorTransform(const Color c);
  void transform(std::string &s, const Color pixel) const override;

private:
  Color color;
};
