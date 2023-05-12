#pragma once

#include "filter.h"

class Rotate : public Filter {
public:
  void apply(Image &img) const override;
};
