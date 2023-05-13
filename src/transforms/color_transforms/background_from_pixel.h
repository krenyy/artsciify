#pragma once

#include "color_transform.h"

/// A background color transform using a pixel reference
class FromPixelBackgroundColorTransform : public ColorTransform {
public:
  /// Applies the FromPixelBackgroundColorTransform.
  ///
  /// \param s a string to apply the transform to
  /// \param pixel reference pixel
  void apply(std::string &s, const Color pixel) const override;
};
