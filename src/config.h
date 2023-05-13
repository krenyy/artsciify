#pragma once

#include "artstyle.h"
#include "filters/pipeline.h"
#include <filesystem>
#include <map>

/// A struct representing the parsed configuration.
///
/// Gets passed to a Presentation instance later.
struct Config {
  /// Constructs a Config instance.
  ///
  /// \param path The path to read the configuration from
  /// \throws std::runtime_error if the config couldn't be read properly
  Config(std::filesystem::path path);

  size_t preview_side_limit;
  size_t warning_side_limit;
  std::map<std::string, ArtStyle> styles;
  std::map<std::string, std::shared_ptr<FilterPipeline>> pipelines;
};
