#pragma once

#include "../image.h"

/// Generic filter class.
class Filter {
public:
  virtual ~Filter() = default;
  /// Apply the filter.
  ///
  /// \param img image to apply the filter to
  virtual void apply(Image &img) const = 0;
  /// Apply the filter without scaling.
  ///
  /// \param img image to apply the filter to
  virtual void apply_without_scaling(Image &img) const { apply(img); }
  /// Get final dimensions of the image after applying this filter.
  ///
  /// \param width current width of the image
  /// \param height current height of the image
  /// \param max_width maximum width the image ever reaches
  /// \param max_height maximum height the image ever reaches
  virtual void get_final_dimensions(size_t &, size_t &, size_t &,
                                    size_t &) const {}
};
