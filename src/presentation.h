#pragma once

#include "artstyle.h"
#include "config.h"
#include "filters/pipeline.h"
#include "image.h"
#include <filesystem>
#include <queue>
#include <vector>

class Presentation {
public:
  Presentation(Config cfg, std::vector<std::filesystem::path> image_paths);
  void start();
  void handle_input();
  std::string read_input() const;
  std::optional<long> read_integer() const;

private:
  Config config;

  std::vector<std::filesystem::path> paths;
  std::vector<Image> images;
  std::vector<Image> previews;
  std::vector<
      std::vector<std::pair<std::string, std::shared_ptr<FilterPipeline>>>>
      pipelines;
  size_t current_image;
  std::vector<std::string> current_style;
};
