#pragma once

#include "config.h"
#include "filters/pipeline.h"
#include "image.h"
#include <filesystem>
#include <vector>

class Presentation {
public:
  Presentation(Config cfg, std::vector<std::filesystem::path> image_paths);
  void start();

private:
  Config config;
  std::vector<Image> images;
  std::vector<Image> previews;
  std::vector<Image> thumbnails;
  std::vector<FilterPipeline> pipelines;
  size_t current_image;
};
