#pragma once

#include "filter.h"

/// A rotate filter
class Rotate : public Filter {
public:
  /// Applies this Rotate filter.
  ///
  /// \param img image to apply the filter to
  void apply(Image &img) const override;
};
