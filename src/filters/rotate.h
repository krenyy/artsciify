#pragma once

#include "filter.h"

/// A rotate filter
class Rotate : public Filter {
public:
  /// Applies this Rotate filter.
  ///
  /// \param img image to apply the filter to
  void apply(Image &img) const override;
  /// Get final dimensions of the image after applying this Upscale filter.
  ///
  /// \param width current width of the image
  /// \param height current height of the image
  /// \param max_width maximum width the image ever reaches
  /// \param max_height maximum height the image ever reaches
  virtual void get_final_dimensions(size_t &width, size_t &height,
                                    size_t &max_width,
                                    size_t &max_height) const override;
};
