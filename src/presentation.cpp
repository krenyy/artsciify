#include "presentation.h"
#include "image.h"
#include <iostream>

Presentation::Presentation(Config cfg,
                           std::vector<std::filesystem::path> image_paths)
    : config(cfg), images() {
  for (std::filesystem::path p : std::move(image_paths)) {
    images.push_back(PngImage(p).read());
  }
}

void Presentation::start() {
  for (Image &img : images) {
    std::cerr << img.width() << "x" << img.height() << std::endl;
    std::stringstream ss1, ss2;
    ss1 << config.styles.at("ascii_eddie_smith").print(img);
    ss2 << config.styles.at("block").print(img);
    for (size_t i = 0; i < img.height(); ++i) {
      std::string line;
      std::getline(ss1, line);
      std::cout << line << " ";
      std::getline(ss2, line);
      std::cout << line << std::endl;
    }
  }
}
