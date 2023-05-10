#pragma once

#include "filter.h"

class Scale : public Filter {
public:
  void apply(Image &img) const override;
};
