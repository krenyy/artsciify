#include "string.h"

StringTextTransform::StringTextTransform(std::string str) : s(str) {}

void StringTextTransform::apply(std::string &str, const Color) const {
  str += s;
}

StringTextTransform StringTextTransform::read(ConfigReader &cr) {
  cr.assert_char({'"'});
  std::string val;
  while (cr.get_col() != cr.get_current_line_length() - 1) {
    val += std::move(*cr.read_char());
  }
  cr.assert_char({'"'});
  return {val};
}
