#pragma once

#include "filter.h"

class Negative : public Filter {
public:
  void apply(Image &img) const override;
};
