#pragma once

#include "color_transform.h"

class BackgroundColorTransform : public ColorTransform {
public:
  BackgroundColorTransform(const Color c);
  void apply(std::string &s, const Color pixel) const override;

private:
  Color color;
};
