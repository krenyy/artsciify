#include "presentation.h"
#include "filters/scaling/downscale.h"
#include "png.h"
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>

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

void Presentation::print_status() const {
  std::cerr << std::endl;
  std::cerr << "current image: " << paths[current_image] << " ("
            << current_image + 1 << '/' << images.size() << ")" << std::endl;
  std::cerr << "image dimensions: " << images[current_image].width() << 'x'
            << images[current_image].height() << std::endl;
  Image preview_img = previews[current_image];
  size_t final_width = images[current_image].width();
  size_t final_height = images[current_image].height();
  size_t max_width = final_width;
  size_t max_height = final_height;
  for (const auto &[_, pipeline] : pipelines[current_image]) {
    pipeline->apply_without_scaling(preview_img);
    pipeline->get_final_dimensions(final_width, final_height, max_width,
                                   max_height);
  }
  if (images[current_image].width() != preview_img.width() ||
      images[current_image].height() != preview_img.height()) {
    std::cerr << "preview dimensions: " << preview_img.width() << 'x'
              << preview_img.height() << std::endl;
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
}

void Presentation::start() {
  while (!images.empty()) {
    handle_input();
  }
  std::cerr << "done!" << std::endl;
}

void Presentation::handle_input() {
  static const std::unordered_map<std::string, std::function<void()>>
      choice_map = {
          {"p", std::bind(&Presentation::handle_print, this)},
          {"prev", std::bind(&Presentation::handle_previous, this)},
          {"next", std::bind(&Presentation::handle_next, this)},
          {"s", std::bind(&Presentation::handle_style, this)},
          {"f", std::bind(&Presentation::handle_filter, this)},
          {"w", std::bind(&Presentation::handle_write, this)},
          {"q", std::bind(&Presentation::handle_quit, this)},
      };

  print_status();

  std::string input = read_input();
  std::cerr << std::endl;

  if (choice_map.count(input) == 0) {
    std::cerr << "invalid option: " << input << std::endl;
    return;
  }
  choice_map.at(input)();
}

void Presentation::handle_print() const {
  Image tmp_preview = previews[current_image];
  for (const auto &[_, pipeline] : pipelines[current_image]) {
    pipeline->apply_without_scaling(tmp_preview);
  }
  std::cerr
      << config.styles.at(current_style[current_image]).print(tmp_preview);
}

void Presentation::handle_previous() {
  if (current_image == 0) {
    std::cerr << "you're already on the first image" << std::endl;
    return;
  }
  --current_image;
}

void Presentation::handle_next() {
  if (current_image == (images.size() - 1)) {
    std::cerr << "you're already on the last image" << std::endl;
    return;
  }
  ++current_image;
}

void Presentation::handle_style() {
  std::cerr << "select a new style:\n";
  std::vector<std::string> style_names(config.styles.size());
  size_t i = 0;
  for (const auto &[name, _] : config.styles) {
    std::cerr << "  " << i << ". " << name << '\n';
    style_names[i++] = name;
  }
  std::cerr << ": ";

  auto selected_opt = read_integer();
  if (!selected_opt.has_value()) {
    std::cerr << "invalid input" << std::endl;
    return;
  }
  size_t selected = static_cast<size_t>(std::move(*selected_opt));
  std::cerr << std::endl;

  if (selected >= style_names.size()) {
    std::cerr << "invalid index" << std::endl;
    return;
  }

  current_style[current_image] = style_names[selected];
}

void Presentation::handle_filter() {
  static const std::unordered_map<
      std::string,
      std::function<void(std::vector<std::pair<
                             std::string, std::shared_ptr<FilterPipeline>>> &)>>
      choice_map = {
          {"a", std::bind(&Presentation::handle_filter_add, this,
                          std::placeholders::_1)},
          {"d", std::bind(&Presentation::handle_filter_del, this,
                          std::placeholders::_1)},
      };

  std::vector<std::pair<std::string, std::shared_ptr<FilterPipeline>>>
      &img_pipeline = pipelines.at(current_image);

  std::cerr << "filter pipelines:\n";
  for (const auto &[name, _] : img_pipeline) {
    std::cerr << "  " << name << '\n';
  }
  std::cerr << std::endl;

  std::cerr << "[a]dd";
  if (!img_pipeline.empty()) {
    std::cerr << ", [d]elete";
  }
  std::cerr << ": ";

  std::string input = read_input();
  std::cerr << std::endl;

  if (choice_map.count(input) == 0) {
    std::cerr << "invalid input" << std::endl;
    return;
  }
  choice_map.at(input)(img_pipeline);
}

void Presentation::handle_filter_add(
    std::vector<std::pair<std::string, std::shared_ptr<FilterPipeline>>>
        &img_pipeline) {
  std::cerr << "select a filter pipeline:\n";

  std::vector<std::string> pipeline_names(config.pipelines.size());
  size_t i = 0;
  for (const auto &[name, _] : config.pipelines) {
    std::cerr << "  " << i << ". " << name << '\n';
    pipeline_names[i++] = name;
  }
  std::cerr << ": ";

  auto selected_opt = read_integer();
  if (!selected_opt.has_value()) {
    std::cerr << "invalid input" << std::endl;
    return;
  }
  size_t selected = static_cast<size_t>(std::move(*selected_opt));

  if (selected >= pipeline_names.size()) {
    std::cerr << "invalid index" << std::endl;
  }

  std::string pipeline_name = pipeline_names[selected];
  std::shared_ptr<FilterPipeline> pipeline = config.pipelines.at(pipeline_name);

  if (img_pipeline.empty()) {
    img_pipeline.emplace_back(pipeline_name, pipeline);
    std::cerr << "added filter " << pipeline_name << '\n';
    return;
  }

  for (;;) {
    std::cerr << "select a position to insert the pipeline at\n";
    i = 0;
    for (const auto &[name, _] : img_pipeline) {
      std::cerr << "  " << i << ". " << name << '\n';
      ++i;
    }
    std::cerr << "  [" << i << ".] \n";
    std::cerr << ": ";

    selected_opt = read_integer();
    if (!selected_opt.has_value()) {
      std::cerr << "invalid input" << std::endl;
      return;
    }
    selected = static_cast<size_t>(std::move(*selected_opt));

    if (selected > img_pipeline.size()) {
      continue;
    }
    img_pipeline.insert(img_pipeline.begin() + static_cast<ssize_t>(selected),
                        make_pair(pipeline_name, pipeline));
    std::cerr << "added filter " << pipeline_name << std::endl;
    return;
  }
  return;
}

void Presentation::handle_filter_del(
    std::vector<std::pair<std::string, std::shared_ptr<FilterPipeline>>>
        &img_pipeline) {
  if (img_pipeline.empty()) {
    return;
  }
  for (;;) {
    std::cerr << "select a pipeline to delete\n";
    size_t i = 0;
    for (const auto &[name, _] : img_pipeline) {
      std::cerr << "  " << i << ". " << name << '\n';
      ++i;
    }
    std::cerr << ": ";

    auto selected_opt = read_integer();
    if (!selected_opt.has_value()) {
      std::cerr << "invalid input" << std::endl;
      return;
    }
    size_t selected = static_cast<size_t>(std::move(*selected_opt));

    if (selected >= img_pipeline.size()) {
      continue;
    }
    std::string pipeline_name = img_pipeline.at(selected).first;
    img_pipeline.erase(img_pipeline.begin() + static_cast<ssize_t>(selected));
    std::cerr << "deleted pipeline " << pipeline_name << std::endl;
    return;
  }
}

void Presentation::handle_write() {
  static const std::unordered_map<std::string, std::function<void()>>
      choice_map = {
          {"o", [this]() { handle_write_to_stream(std::cout); }},
          {"f", std::bind(&Presentation::handle_write_file, this)},
      };

  std::cerr << "select write destination:\n";
  std::cerr << "  std[o]ut\n";
  std::cerr << "  [f]ile\n";
  std::cerr << ": ";

  std::string input = read_input();
  std::cerr << std::endl;

  if (choice_map.count(input) == 0) {
    std::cerr << "wrong option: " << input << std::endl;
    return;
  }
  choice_map.at(input)();
}

void Presentation::handle_write_file() {
  std::filesystem::path dst(paths.at(current_image));
  dst.replace_extension(".txt");

  std::cerr << "type in a file path (default is " << dst << "): ";

  std::string input = read_input();
  std::cerr << std::endl;

  if (input != "") {
    dst = input;
  }
  if (std::filesystem::exists(dst) && !std::filesystem::is_regular_file(dst)) {
    std::cerr << "path already exists and is not a regular file!" << std::endl;
    return;
  }
  if (std::filesystem::exists(dst) && std::filesystem::is_regular_file(dst)) {
    for (;;) {
      std::cerr << "file already exists, do you want to overwrite it? [y/n]: ";

      input = read_input();

      if (input == "y") {
        break;
      }
      if (input == "n") {
        std::cerr << "aborted!" << std::endl;
        return;
      }
    }
  }

  std::ofstream ofs(dst);
  if (!ofs.good()) {
    std::cerr
        << "couldn't open file (inaccessible directory / permission denied!)"
        << std::endl;
    return;
  }
  ofs.exceptions(std::ios::badbit | std::ios::failbit);

  try {
    handle_write_to_stream(ofs);
  } catch (const std::ios::failure &) {
    std::cerr << "write failed!" << std::endl;
  }
}

void Presentation::handle_write_to_stream(std::ostream &os) {
  for (const auto &[_, pipeline] : pipelines.at(current_image)) {
    pipeline->apply(images[current_image]);
  }
  os << config.styles.at(current_style[current_image])
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
}

void Presentation::handle_quit() const {
  throw std::runtime_error("quitting!");
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

std::optional<long> Presentation::read_integer() const {
  std::string input = read_input();
  size_t input_size = input.size();
  long n;
  size_t pos;
  try {
    n = std::stol(input, &pos);
  } catch (const std::exception &) {
    return std::nullopt;
  }
  if (pos != input_size) {
    return std::nullopt;
  }
  return n;
}
