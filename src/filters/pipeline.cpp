#include "pipeline.h"
#include "brightness.h"
#include "downscale.h"
#include "grayscale.h"
#include "negative.h"
#include "rotate.h"
#include "threshold.h"
#include "upscale.h"
#include <memory>

FilterPipeline::FilterPipeline(std::vector<std::shared_ptr<Filter>> f)
    : filters(std::move(f)) {}

void FilterPipeline::apply(Image &img) const {
  for (const std::shared_ptr<Filter> &f : filters) {
    f->apply(img);
  }
}
void FilterPipeline::apply_without_scaling(Image &img) const {
  for (const std::shared_ptr<Filter> &f : filters) {
    if (!std::dynamic_pointer_cast<Upscale>(f) &&
        !std::dynamic_pointer_cast<Downscale>(f)) {
      f->apply(img);
    }
  }
}

FilterPipeline FilterPipeline::read(
    ConfigReader &cr,
    std::map<std::string, std::unordered_set<std::string>> &names,
    const std::map<std::string, Luminance> &luminances,
    std::map<std::string, std::shared_ptr<FilterPipeline>> &pipelines) {
  std::vector<std::shared_ptr<Filter>> filters;
  for (;;) {
    auto filter_name_opt =
        cr.assert_word({"FilterPipeline", "Grayscale", "Brightness", "Negative",
                        "Threshold", "Upscale", "Downscale", "Rotate"});
    if (!filter_name_opt.has_value()) {
      break;
    }
    std::string filter_name = std::move(*filter_name_opt);
    if (filter_name == "FilterPipeline") {
      cr.assert_char({' '});
      auto pipeline_name_opt = cr.assert_word(names["pipeline"]);
      if (!pipeline_name_opt.has_value()) {
        throw cr.except("Missing FilterPipeline argument!");
      }
      std::string pipeline_name = std::move(*pipeline_name_opt);
      filters.push_back(pipelines.at(pipeline_name));
    }
    if (filter_name == "Grayscale") {
      filters.push_back(
          std::make_shared<Grayscale>(Grayscale::read(cr, names, luminances)));
    }
    if (filter_name == "Brightness") {
      filters.push_back(std::make_shared<Brightness>(Brightness::read(cr)));
    }
    if (filter_name == "Negative") {
      filters.push_back(std::make_shared<Negative>());
    }
    if (filter_name == "Threshold") {
      filters.push_back(std::make_shared<Threshold>(Threshold::read(cr)));
    }
    if (filter_name == "Upscale") {
      filters.push_back(std::make_shared<Upscale>());
    }
    if (filter_name == "Downscale") {
      filters.push_back(std::make_shared<Downscale>());
    }
    if (filter_name == "Rotate") {
      filters.push_back(std::make_shared<Rotate>());
    }
    cr.next_line();
  }
  return {filters};
}

void FilterPipeline::get_final_dimensions(size_t &width, size_t &height,
                                          size_t &max_width,
                                          size_t &max_height) const {
  for (const auto &f : filters) {
    if (const auto pipeline = std::dynamic_pointer_cast<FilterPipeline>(f)) {
      pipeline->get_final_dimensions(width, height, max_width, max_height);
    }
    if (std::dynamic_pointer_cast<Upscale>(f)) {
      width *= 2;
      height *= 2;
    }
    max_width = width > max_width ? width : max_width;
    max_height = height > max_height ? height : max_height;
    if (std::dynamic_pointer_cast<Downscale>(f)) {
      width = (width + 1) / 2;
      height = (height + 1) / 2;
    }
  }
}
