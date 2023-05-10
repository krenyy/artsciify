#pragma once

#include "config.h"
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
};
