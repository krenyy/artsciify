#include "pipeline.h"
#include "brightness.h"
#include "grayscale.h"
#include "negative.h"

FilterPipeline::FilterPipeline(std::vector<std::shared_ptr<Filter>> f)
    : filters(std::move(f)) {}

void FilterPipeline::apply(Image &img) const {
  for (const std::shared_ptr<Filter> &f : filters) {
    f->apply(img);
  }
}

FilterPipeline FilterPipeline::read(
    ConfigReader &cr,
    std::map<std::string, std::unordered_set<std::string>> &names,
    const std::map<std::string, Luminance> &luminances,
    std::map<std::string, std::shared_ptr<FilterPipeline>> &pipelines) {
  std::vector<std::shared_ptr<Filter>> filters;
  for (;;) {
    auto filter_name_opt = cr.assert_word(
        {"FilterPipeline", "Grayscale", "Brightness", "Negative"});
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
    cr.next_line();
  }
  return {filters};
}
