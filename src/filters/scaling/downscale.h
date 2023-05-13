#pragma once

#include "scaling_filter.h"

/// Simple downscale filter.
class Downscale : public ScalingFilter {
public:
  /// Applies this Downscale filter.
  ///
  /// \param img image to apply the filter to
  virtual void apply(Image &img) const override;
  /// Get final dimensions of the image after applying this Downscale filter.
  ///
  /// \param width current width of the image
  /// \param height current height of the image
  /// \param max_width maximum width the image ever reaches
  /// \param max_height maximum height the image ever reaches
  virtual void get_final_dimensions(size_t &width, size_t &height,
                                    size_t &max_width,
                                    size_t &max_height) const override;
};
