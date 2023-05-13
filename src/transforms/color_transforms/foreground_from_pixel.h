#pragma once

#include "color_transform.h"

/// A foreground color transform using a pixel reference
class FromPixelForegroundColorTransform : public ColorTransform {
public:
  /// Applies the FromPixelForegroundColorTransform.
  ///
  /// \param s a string to apply the transform to
  /// \param pixel reference pixel
  void apply(std::string &s, const Color pixel) const override;
};
