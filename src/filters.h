#include "image.h"

class Filter {
public:
  virtual void apply(Image &img) const = 0;
};

class Negative : public Filter {
public:
  void apply(Image &img) const override;
};
