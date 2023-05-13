#pragma once

#include "../config_reader.h"
#include "filter.h"

/// A threshold filter.
///
/// Any pixel value less than the threshold will be set to zero.
///
/// Any pixel value greater than or equal to the threshold will be set to 255.
class Threshold : public Filter {
public:
  /// Constructs a Threshold instance.
  ///
  /// \param threshold a threshold
  Threshold(uint8_t threshold);
  /// Applies this Threshold filter.
  ///
  /// \param img image to apply the filter to
  void apply(Image &img) const override;

  /// Reads a Threshold using a ConfigReader instance and other params.
  static Threshold read(ConfigReader &cr);

private:
  uint8_t t;
};
