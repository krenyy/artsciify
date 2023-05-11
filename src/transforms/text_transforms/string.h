#pragma once

#include "text_transform.h"

class StringTextTransform : public TextTransform {
public:
  StringTextTransform(std::string str);
  void apply(std::string &str, const Color pixel) const override;

private:
  std::string s;
};
