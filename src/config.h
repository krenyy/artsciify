#pragma once

#include "artstyle.h"
#include "filters/pipeline.h"
#include <filesystem>
#include <map>

struct Config {
  Config(std::filesystem::path path);

  size_t preview_side_limit;
  size_t warning_side_limit;
  std::map<std::string, ArtStyle> styles;
  std::map<std::string, std::shared_ptr<FilterPipeline>> pipelines;
};
