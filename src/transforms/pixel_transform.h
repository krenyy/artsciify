#pragma once

#include "../color.h"
#include <string>

/// Generic pixel transform.
class PixelTransform {
public:
  virtual ~PixelTransform() = default;
  /// Apply a pixel transform.
  ///
  /// \param s string to transform
  /// \param pixel a reference pixel
  virtual void apply(std::string &s, const Color pixel) const = 0;
};
