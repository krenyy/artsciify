#include "config.h"
#include "filters/scale.h"
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

int main(int argc, char **argv) {
  try {
    std::filesystem::path program_path =
        std::filesystem::weakly_canonical(argv[0]);
    std::filesystem::path config_path =
        program_path.parent_path() / "artsciify.conf";
    Config config(config_path);
    if (argc == 1) {
      throw std::runtime_error("No images provided!");
    }
    std::vector<std::filesystem::path> images;
    for (int i = 1; i < argc; ++i) {
      images.emplace_back(argv[i]);
    }
    for (const auto &p : images) {
      std::filesystem::path img_path(p);
      Image img = PngImage(img_path).read();
      std::cerr << img.width() << "x" << img.height() << std::endl;
      std::stringstream ss1, ss2;
      ss1 << config.styles.at("ascii_eddie_smith").print(img);
      ss2 << config.styles.at("block").print(img);
      for (size_t i = 0; i < img.height(); ++i) {
        std::string line;
        std::getline(ss1, line);
        std::cerr << line << " ";
        std::getline(ss2, line);
        std::cerr << line << std::endl;
      }
    }
    return EXIT_SUCCESS;
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}
