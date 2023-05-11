#include "presentation.h"
#include "filters/scale.h"
#include "image.h"
#include "transforms/color_transform_bg_frompixel.h"
#include <iostream>

Presentation::Presentation(Config cfg,
                           std::vector<std::filesystem::path> image_paths)
    : config(cfg), paths(std::move(image_paths)), images(), previews(),
      thumbnails(), pipelines(), current_image(0),
      current_style(paths.size(), config.styles.begin()->first) {
  for (std::filesystem::path p : paths) {
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
  while (!images.empty()) {
    handle_input();
  }
}

void Presentation::handle_input() {
  std::string buf;
  for (;;) {
    buf.clear();
    std::cout << std::endl;
    std::cout << "current image: " << paths[current_image] << std::endl;
    std::cout << "image dimensions: " << images[current_image].width() << 'x'
              << images[current_image].height() << std::endl;
    if (images[current_image].width() != previews[current_image].width() ||
        images[current_image].height() != previews[current_image].height()) {
      std::cout << "preview dimensions: " << previews[current_image].width()
                << 'x' << previews[current_image].height() << std::endl;
    }
    std::cout << "current style: " << current_style[current_image] << std::endl;
    std::cout << std::endl;
    std::cout << "[p]rint image, [prev]/[next] image, select [s]tyle, add "
                 "[f]ilter pipeline, [q]uit: ";
    for (char c; (c = static_cast<char>(std::cin.get())) != '\n';) {
      buf += c;
    }
    std::cout << std::endl;
    if (buf == "p") {
      std::cout << config.styles.at(current_style[current_image])
                       .print(previews[current_image])
                << std::endl;
      return;
    }
    if (buf == "prev") {
      if (current_image == 0) {
        std::cout << "you're already on the first image" << std::endl;
        return;
      }
      --current_image;
      return;
    }
    if (buf == "next") {
      if (current_image == (images.size() - 1)) {
        std::cout << "you're already on the last image" << std::endl;
        return;
      }
      ++current_image;
      return;
    }
    if (buf == "s") {
      std::cout << "select a new style:\n";
      for (const auto &[name, _] : config.styles) {
        std::cout << name << '\n';
      }
      buf.clear();
      std::cout << ": ";
      for (char c; (c = static_cast<char>(std::cin.get())) != '\n';) {
        buf += c;
      }
      std::cout << std::endl;
      if (config.styles.count(buf) == 0) {
        std::cout << "unknown style" << std::endl;
        return;
      }
      current_style[current_image] = buf;
    }
    if (buf == "q") {
      throw std::runtime_error("quitting!");
    }
    std::cout << "invalid option: " << buf << std::endl;
  }
}