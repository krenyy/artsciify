#pragma once

#include "../../config_reader.h"
#include "text_transform.h"

class StringTextTransform : public TextTransform {
public:
  StringTextTransform(std::string str);
  void apply(std::string &str, const Color pixel) const override;

  static StringTextTransform read(ConfigReader &cr);

private:
  std::string s;
};
