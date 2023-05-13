#pragma once

#include "../../config_reader.h"
#include "text_transform.h"

/// A string text transform
class StringTextTransform : public TextTransform {
public:
  /// Creates a StringTextTransform object.
  ///
  /// \param str a string to use for transform
  StringTextTransform(std::string str);
  /// Applies the StringTextTransform.
  ///
  /// \param str string to apply the transform to
  /// \param pixel reference pixel (not used here)
  void apply(std::string &str, const Color pixel) const override;

  /// Reads a StringTextTransform using a ConfigReader instance.
  static StringTextTransform read(ConfigReader &cr);

private:
  std::string s;
};
