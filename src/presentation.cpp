#include "presentation.h"
#include "filters/downscale.h"
#include "filters/upscale.h"
#include "png.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>

Presentation::Presentation(Config cfg,
                           std::vector<std::filesystem::path> image_paths)
    : config(cfg), paths(std::move(image_paths)), images(), previews(),
      pipelines(paths.size()), current_image(0),
      current_style(paths.size(), config.styles.begin()->first) {
  for (std::filesystem::path p : paths) {
    images.push_back(PngImage(p).read());
  }
  for (const Image &img : images) {
    Image preview_img = img;
    while (preview_img.height() > config.preview_side_limit ||
           preview_img.width() > config.preview_side_limit) {
      Downscale().apply(preview_img);
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
  for (;;) {
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
    size_t final_width = images[current_image].width();
    size_t final_height = images[current_image].height();
    size_t max_width = final_width;
    size_t max_height = final_height;
    for (const auto &[_, pipeline] : pipelines[current_image]) {
      pipeline->get_final_dimensions(final_width, final_height, max_width,
                                     max_height);
    }
    std::cerr << "maximum dimensions during the pipeline application: "
              << max_width << 'x' << max_height << std::endl;
    if (images[current_image].width() != final_width ||
        images[current_image].height() != final_height) {
      std::cerr << "final dimensions: " << final_width << 'x' << final_height
                << std::endl;
    }
    if (final_width < max_width || final_height < max_height) {
      std::cerr
          << "WARNING: the final image may be smaller than intended\n         "
             "as it is larger during the pipeline application.\n         are "
             "you sure you are applying scaling filter correctly?"
          << std::endl;
    }
    if (final_width >= config.warning_side_limit ||
        final_height >= config.warning_side_limit) {
      std::cerr << "WARNING: the final image dimensions are too big\n         "
                   "and could "
                   "result in a slow write or even a crash!"
                << std::endl;
    }
    std::cerr << "current style: " << current_style[current_image] << std::endl;
    std::cerr << std::endl;
    std::cerr << "[p]rint image, [prev]/[next] image, select [s]tyle, edit "
                 "[f]ilter pipeline, [w]rite image, [q]uit: ";

    std::string input = read_input();
    std::cerr << std::endl;

    if (input == "p") {
      Image tmp_preview = previews[current_image];
      for (const auto &[_, pipeline] : pipelines[current_image]) {
        pipeline->apply_without_scaling(tmp_preview);
      }
      std::cerr
          << config.styles.at(current_style[current_image]).print(tmp_preview);
      return;
    }
    if (input == "prev") {
      if (current_image == 0) {
        std::cerr << "you're already on the first image" << std::endl;
        return;
      }
      --current_image;
      return;
    }
    if (input == "next") {
      if (current_image == (images.size() - 1)) {
        std::cerr << "you're already on the last image" << std::endl;
        return;
      }
      ++current_image;
      return;
    }
    if (input == "s") {
      std::cerr << "select a new style:\n";
      for (const auto &[name, _] : config.styles) {
        std::cerr << "  " << name << '\n';
      }
      std::cerr << ": ";

      input = read_input();
      std::cerr << std::endl;

      if (config.styles.count(input) == 0) {
        std::cerr << "unknown style" << std::endl;
        return;
      }
      current_style[current_image] = input;
      return;
    }
    if (input == "f") {
      std::vector<std::pair<std::string, std::shared_ptr<FilterPipeline>>>
          &img_pipeline = pipelines.at(current_image);

      std::cerr << "filter pipelines:\n";
      for (const auto &[name, _] : img_pipeline) {
        std::cerr << "  " << name << '\n';
      }
      std::cerr << std::endl;

      for (;;) {
        std::cerr << "[a]dd, [d]elete: ";

        input = read_input();
        std::cerr << std::endl;

        if (input == "a") {
          std::cerr << "select a filter pipeline:\n";
          for (const auto &[name, _] : config.pipelines) {
            std::cerr << "  " << name << '\n';
          }
          std::cerr << ": ";

          input = read_input();
          std::cerr << std::endl;

          if (config.pipelines.count(input) == 0) {
            std::cerr << "unknown filter pipeline" << std::endl;
            return;
          }

          std::string pipeline_name = input;
          std::shared_ptr<FilterPipeline> pipeline =
              config.pipelines.at(pipeline_name);

          if (img_pipeline.empty()) {
            img_pipeline.emplace_back(pipeline_name, pipeline);
            std::cerr << "added filter " << pipeline_name << '\n';
            return;
          }

          for (;;) {
            std::cerr << "select a position to insert the pipeline at\n";
            size_t i = 0;
            for (const auto &[name, _] : img_pipeline) {
              std::cerr << "  " << i << ". " << name << '\n';
              ++i;
            }
            std::cerr << "  [" << i << ".] \n";
            std::cerr << ": ";

            input = read_input();
            size_t selected;
            try {
              selected = std::stoul(input);
            } catch (const std::exception &) {
              std::cerr << "\ninvalid index\n";
              return;
            }
            if (selected > img_pipeline.size()) {
              continue;
            }
            img_pipeline.insert(img_pipeline.begin() +
                                    static_cast<ssize_t>(selected),
                                make_pair(pipeline_name, pipeline));
            std::cerr << "\nadded filter " << pipeline_name << '\n';
            return;
          }
          return;
        }
        if (input == "d") {
          for (;;) {
            std::cerr << "select a pipeline to delete\n";
            size_t i = 0;
            for (const auto &[name, _] : img_pipeline) {
              std::cerr << "  " << i << ". " << name << '\n';
              ++i;
            }
            std::cerr << ": ";

            input = read_input();
            size_t input_size = input.size();
            size_t selected;
            size_t pos;
            try {
              selected = std::stoul(input, &pos);
            } catch (const std::exception &) {
              std::cerr << "\ninvalid index" << std::endl;
              return;
            }
            if (pos != input_size) {
              std::cerr << "\ninvalid input" << std::endl;
              return;
            }
            if (selected >= img_pipeline.size()) {
              continue;
            }
            std::string pipeline_name = img_pipeline.at(selected).first;
            img_pipeline.erase(img_pipeline.begin() +
                               static_cast<ssize_t>(selected));
            std::cerr << "\ndeleted pipeline " << pipeline_name << '\n';
            return;
          }
        }
        return;
      }
    }
    if (input == "w") {
      std::cerr << "select write destination:\n";
      std::cerr << "  std[o]ut\n";
      std::cerr << "  [f]ile\n";
      std::cerr << ": ";

      input = read_input();
      std::cerr << std::endl;

      if (input == "o") {
        for (const auto &[_, pipeline] : pipelines.at(current_image)) {
          pipeline->apply(images[current_image]);
        }
        std::cout << config.styles.at(current_style[current_image])
                         .print(images[current_image])
                  << std::endl;
        ssize_t cur_img = static_cast<ssize_t>(current_image);
        paths.erase(paths.begin() + cur_img);
        images.erase(images.begin() + cur_img);
        previews.erase(previews.begin() + cur_img);
        pipelines.erase(pipelines.begin() + cur_img);
        current_style.erase(current_style.begin() + cur_img);
        std::cerr << "written successfully!" << std::endl;
        if (current_image == images.size()) {
          --current_image;
        }
        return;
      }
      if (input == "f") {
        std::filesystem::path dst(paths.at(current_image));
        dst.replace_extension(".txt");

        std::cerr << "type in a file path (default is " << dst << "): ";

        input = read_input();
        std::cerr << std::endl;

        if (input != "") {
          dst = input;
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
            std::cerr
                << "file already exists, do you want to overwrite it? [y/n]: ";

            input = read_input();

            if (input == "y") {
              break;
            }
            if (input == "n") {
              std::cerr << "\naborted!" << std::endl;
              return;
            }
          }
        }

        for (const auto &[_, pipeline] : pipelines.at(current_image)) {
          pipeline->apply(images[current_image]);
        }
        std::ofstream ofs(dst);
        if (!ofs.good()) {
          std::cerr << "\ncouldn't open file (either inaccessible directory, "
                       "or permission denied!)"
                    << std::endl;
          return;
        }
        ofs << config.styles.at(current_style[current_image])
                   .print(images[current_image])
            << std::endl;
        ssize_t cur_img = static_cast<ssize_t>(current_image);
        paths.erase(paths.begin() + cur_img);
        images.erase(images.begin() + cur_img);
        previews.erase(previews.begin() + cur_img);
        pipelines.erase(pipelines.begin() + cur_img);
        current_style.erase(current_style.begin() + cur_img);
        std::cerr << dst << " written successfully!" << std::endl;
        if (current_image == images.size()) {
          --current_image;
        }
        return;
      }
      std::cerr << "wrong option!" << std::endl;
      return;
    }
    if (input == "q") {
      throw std::runtime_error("quitting!");
    }
    std::cerr << "invalid option: " << input << std::endl;
  }
}

std::string Presentation::read_input() const {
  std::string buf;
  for (char c; (c = static_cast<char>(std::cin.get())) != '\n';) {
    if (c == EOF) {
      throw std::runtime_error("unexpected end of input!");
    }
    if (std::iscntrl(c)) {
      continue;
    }
    buf += c;
  }
  return buf;
}
