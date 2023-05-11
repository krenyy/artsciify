#pragma once

#include "artstyle.h"
#include "config.h"
#include "filters/pipeline.h"
#include "image.h"
#include <filesystem>
#include <vector>

class Presentation {
public:
  Presentation(Config cfg, std::vector<std::filesystem::path> image_paths);
  void start();
  void handle_input();

private:
  Config config;
  std::vector<std::filesystem::path> paths;
  std::vector<Image> images;
  std::vector<Image> previews;
  std::vector<Image> thumbnails;
  std::vector<FilterPipeline> pipelines;
  size_t current_image;
  std::vector<std::string> current_style;
};
