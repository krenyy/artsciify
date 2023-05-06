#pragma once

#include "../styles.h"
#include "filter.h"

class Grayscale : public Filter {
public:
  Grayscale(Luminance luminance);
  void apply(Image &img) const override;

private:
  Luminance lum;
};
