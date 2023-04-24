#include "image.h"

class Filter {
public:
  virtual void apply(Image &img) const = 0;
};

class Grayscale : public Filter {
public:
  Grayscale(double brightness_r, double brightness_g, double brightness_b);
  void apply(Image &img) const override;

private:
  double r;
  double g;
  double b;
};

class Negative : public Filter {
public:
  void apply(Image &img) const override;
};

class Brightness : public Filter {
public:
  Brightness(double multiplier);
  void apply(Image &img) const override;

private:
  double m;
};
