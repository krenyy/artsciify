#include "config.h"
#include "presentation.h"
#include <iostream>

int main(int argc, char **argv) {
  try {
    std::filesystem::path program_path(argv[0]);
    std::filesystem::path config_path =
        program_path.parent_path() / "artsciify.conf";
    Config config(std::move(config_path));
    if (argc == 1) {
      throw std::runtime_error("usage: " + program_path.string() +
                               " <image> [<image> ...]");
    }
    std::vector<std::filesystem::path> image_paths;
    for (int i = 1; i < argc; ++i) {
      image_paths.emplace_back(argv[i]);
    }
    Presentation p(std::move(config), std::move(image_paths));
    p.start();
    return EXIT_SUCCESS;
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}
