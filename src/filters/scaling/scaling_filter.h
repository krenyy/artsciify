#pragma once

#include "../filter.h"

/// Generic scaling filter.
class ScalingFilter : public Filter {
public:
  /// Applies this ScalingFilter.
  ///
  /// \param img image to apply the filter to
  virtual void apply(Image &img) const override = 0;
  /// Applies this ScalingFilter without scaling.
  ///
  /// \param img image to apply the filter to
  virtual void apply_without_scaling(Image &) const override {}
};
