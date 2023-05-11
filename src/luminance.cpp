#include "luminance.h"

Luminance::Luminance(double brightness_r, double brightness_g,
                     double brightness_b)
    : r(brightness_r), g(brightness_g), b(brightness_b) {}

Luminance Luminance::read(ConfigReader &cr) {
  auto r_opt = cr.read_double();
  if (!r_opt.has_value()) {
    throw cr.except("Missing luminance value for red!");
  }
  cr.assert_char({' '});
  double r = std::move(*r_opt);
  auto g_opt = cr.read_double();
  if (!g_opt.has_value()) {
    throw cr.except("Missing luminance value for green!");
  }
  cr.assert_char({' '});
  double g = std::move(*g_opt);
  auto b_opt = cr.read_double();
  if (!b_opt.has_value()) {
    throw cr.except("Missing luminance value for blue!");
  }
  double b = std::move(*b_opt);
  return {r, g, b};
}
