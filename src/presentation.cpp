#include "presentation.h"
#include "filters/scale.h"
#include "png.h"
#include <filesystem>
#include <fstream>
#include <iostream>

Presentation::Presentation(Config cfg,
                           std::vector<std::filesystem::path> image_paths)
    : config(cfg), paths(std::move(image_paths)), images(), previews(),
      pipelines(), current_image(0),
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
  }
}

void Presentation::start() {
  while (!images.empty()) {
    handle_input();
  }
  std::cerr << "done!" << std::endl;
}

void Presentation::handle_input() {
  std::string buf;
  for (;;) {
    buf.clear();
    std::cerr << std::endl;
    std::cerr << "current image: " << paths[current_image] << " ("
              << current_image + 1 << '/' << images.size() << ")" << std::endl;
    std::cerr << "image dimensions: " << images[current_image].width() << 'x'
              << images[current_image].height() << std::endl;
    if (images[current_image].width() != previews[current_image].width() ||
        images[current_image].height() != previews[current_image].height()) {
      std::cerr << "preview dimensions: " << previews[current_image].width()
                << 'x' << previews[current_image].height() << std::endl;
    }
    std::cerr << "current style: " << current_style[current_image] << std::endl;
    std::cerr << std::endl;
    std::cerr << "[p]rint image, [prev]/[next] image, select [s]tyle, add "
                 "[f]ilter pipeline, [w]rite image, [q]uit: ";
    for (char c; (c = static_cast<char>(std::cin.get())) != '\n';) {
      buf += c;
    }
    std::cerr << std::endl;
    if (buf == "p") {
      std::cerr << config.styles.at(current_style[current_image])
                       .print(previews[current_image]);
      return;
    }
    if (buf == "prev") {
      if (current_image == 0) {
        std::cerr << "you're already on the first image" << std::endl;
        return;
      }
      --current_image;
      return;
    }
    if (buf == "next") {
      if (current_image == (images.size() - 1)) {
        std::cerr << "you're already on the last image" << std::endl;
        return;
      }
      ++current_image;
      return;
    }
    if (buf == "s") {
      buf.clear();
      std::cerr << "select a new style:\n";
      for (const auto &[name, _] : config.styles) {
        std::cerr << "  " << name << '\n';
      }
      std::cerr << ": ";
      for (char c; (c = static_cast<char>(std::cin.get())) != '\n';) {
        buf += c;
      }
      std::cerr << std::endl;
      if (config.styles.count(buf) == 0) {
        std::cerr << "unknown style" << std::endl;
        return;
      }
      current_style[current_image] = buf;
      return;
    }
    if (buf == "w") {
      buf.clear();
      std::cerr << "select write destination:\n";
      std::cerr << "  stdout\n";
      std::cerr << "  file\n";
      std::cerr << ": ";
      for (char c; (c = static_cast<char>(std::cin.get())) != '\n';) {
        buf += c;
      }
      std::cerr << std::endl;
      if (buf == "stdout") {
        std::cout << config.styles.at(current_style[current_image])
                         .print(images[current_image])
                  << std::endl;
        images.erase(images.begin() + static_cast<ssize_t>(current_image));
        previews.erase(previews.begin() + static_cast<ssize_t>(current_image));
        current_style.erase(current_style.begin() +
                            static_cast<ssize_t>(current_image));
        std::cerr << "written successfully!" << std::endl;
        return;
      }
      if (buf == "file") {
        buf.clear();
        std::filesystem::path dst(paths.at(current_image));
        dst.replace_extension(".txt");
        std::cerr << "type in a file path (default is " << dst << "): ";
        for (char c; (c = static_cast<char>(std::cin.get())) != '\n';) {
          buf += c;
        }
        if (buf != "") {
          dst = buf;
        }
        if (std::filesystem::exists(dst) &&
            !std::filesystem::is_regular_file(dst)) {
          std::cerr << "path already exists and is not a regular file!"
                    << std::endl;
          return;
        }
        if (std::filesystem::exists(dst) &&
            std::filesystem::is_regular_file(dst)) {
          for (;;) {
            buf.clear();
            std::cerr
                << "file already exists, do you want to overwrite it? [y/n]: ";
            for (char c; (c = static_cast<char>(std::cin.get())) != '\n';) {
              buf += c;
            }
            if (buf == "y") {
              break;
            }
            if (buf == "n") {
              std::cerr << "\naborted!" << std::endl;
              return;
            }
          }
        }

        std::ofstream ofs(dst);
        ofs << config.styles.at(current_style[current_image])
                   .print(images[current_image])
            << std::endl;
        images.erase(images.begin() + static_cast<ssize_t>(current_image));
        previews.erase(previews.begin() + static_cast<ssize_t>(current_image));
        current_style.erase(current_style.begin() +
                            static_cast<ssize_t>(current_image));
        std::cerr << '\n' << dst << " written successfully!" << std::endl;
        return;
      }
      std::cerr << "wrong option!" << std::endl;
      return;
    }
    if (buf == "q") {
      throw std::runtime_error("quitting!");
    }
    std::cerr << "invalid option: " << buf << std::endl;
  }
}
