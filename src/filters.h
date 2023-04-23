#include "image.h"

class Filter {
public:
  virtual void apply(Image &img) const = 0;
};

class Grayscale : public Filter {
public:
  void apply(Image &img) const override;
};

class Negative : public Filter {
public:
  void apply(Image &img) const override;
};
