#pragma once

#include "artstyle.h"
#include "config.h"
#include "filters/pipeline.h"
#include "image.h"
#include <filesystem>
#include <queue>
#include <vector>

/// Represents a presentation in the terminal.
///
/// Handles loading image files, storing the state, carrying
/// out various operations, and also writing.
class Presentation {
public:
  /// Initialize a new Presentation instance with the provided config and image
  /// paths.
  ///
  /// \param cfg Config object constructed by reading a config file
  /// \param image_paths Image paths to load and do operations on
  Presentation(Config cfg, std::vector<std::filesystem::path> image_paths);
  /// Initiates the presentation
  ///
  /// Uses stderr for output
  void start();

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

  void print_status() const;
  void handle_input();
  void handle_print() const;
  void handle_previous();
  void handle_next();
  void handle_style();
  void handle_filter();
  void handle_filter_add(
      std::vector<std::pair<std::string, std::shared_ptr<FilterPipeline>>>
          &img_pipeline);
  void handle_filter_del(
      std::vector<std::pair<std::string, std::shared_ptr<FilterPipeline>>>
          &img_pipeline);
  void handle_write();
  void handle_write_file();
  void handle_write_to_stream(std::ostream &os);
  void handle_quit() const;

  /// Reads input from stdin until a newline.
  ///
  /// Skips control characters.
  ///
  /// \throws std::runtime_error on EOF
  /// \returns std::string containing the retrieved input
  std::string read_input() const;
  /// Reads an integer from stdin using the read_input method
  ///
  /// \returns An integer. If the conversion failed, returns std::nullopt
  std::optional<long> read_integer() const;
};
