#pragma once

#include "artstyle.h"
#include <filesystem>
#include <map>

struct Config {
  Config(std::filesystem::path path);

  size_t preview_side_limit;
  std::map<std::string, ArtStyle> styles;
};
