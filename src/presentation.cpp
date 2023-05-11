#include "presentation.h"
#include "filters/scale.h"
#include "image.h"
#include <iostream>

Presentation::Presentation(Config cfg,
                           std::vector<std::filesystem::path> image_paths)
    : config(cfg), images(), previews(), thumbnails(), pipelines(),
      current_image(0) {
  for (std::filesystem::path p : std::move(image_paths)) {
    images.push_back(PngImage(p).read());
  }
  for (const Image &img : images) {
    Image preview_img = img;
    while (preview_img.height() > 64 || preview_img.width() > 64) {
      Scale().apply(preview_img);
    }
    previews.push_back(std::move(preview_img));
    Image thumbnail_img = img;
    while (thumbnail_img.height() > 20 || thumbnail_img.width() > 20) {
      Scale().apply(thumbnail_img);
    }
    thumbnails.push_back(std::move(thumbnail_img));
  }
}

void Presentation::start() {
  std::string buf;
  while (!images.empty()) {
    std::cout << config.styles.at("block_blue_x").print(previews[current_image])
              << std::endl;
    std::cout << buf << std::endl;
    buf.clear();
    std::cin >> buf;
  }
}
