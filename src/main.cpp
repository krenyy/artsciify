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
    {
      std::unordered_set<std::filesystem::path> image_paths_set;
      for (int i = 1; i < argc; ++i) {
        std::filesystem::path p(argv[i]);
        if (image_paths_set.count(p) != 0) {
          throw std::runtime_error('"' + p.string() +
                                   "\" already provided once!");
        }
        image_paths_set.insert(p);
        image_paths.push_back(std::move(p));
      }
    }
    Presentation p(std::move(config), std::move(image_paths));
    p.start();
    return EXIT_SUCCESS;
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}
