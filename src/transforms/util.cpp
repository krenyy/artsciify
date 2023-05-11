#include "util.h"
#include "../color.h"
#include <string>

std::string rgb_to_fg_color_code(const Color c) {
  return "\033[38;2;" + std::to_string((int)c.r()) + ";" +
         std::to_string((int)c.g()) + ";" + std::to_string((int)c.b()) + "m";
}

std::string rgb_to_bg_color_code(const Color c) {
  return "\033[48;2;" + std::to_string((int)c.r()) + ";" +
         std::to_string((int)c.g()) + ";" + std::to_string((int)c.b()) + "m";
}
