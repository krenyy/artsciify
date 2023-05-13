#pragma once

#include "filter.h"

/// A negative filter
class Negative : public Filter {
public:
  /// Applies this Negative filter.
  ///
  /// \param img image to apply the filter to
  void apply(Image &img) const override;
};
