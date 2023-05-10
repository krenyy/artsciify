#include "text_transform_string.h"

StringTextTransform::StringTextTransform(std::string str) : s(str) {}

void StringTextTransform::apply(std::string &str, const Color) const {
  str += s;
}
