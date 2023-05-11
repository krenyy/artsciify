#include "config.h"
#include "filters/pipeline.h"
#include "transforms/color_transforms/background.h"
#include "transforms/color_transforms/background_from_pixel.h"
#include "transforms/color_transforms/foreground.h"
#include "transforms/color_transforms/foreground_from_pixel.h"
#include "transforms/text_transforms/ascii.h"
#include "transforms/text_transforms/string.h"
#include <fstream>

Config::Config(std::filesystem::path path)
    : preview_side_limit(), styles(), pipelines() {
  ConfigReader cr(path);
  cr.skip_newlines();
  if (!cr.assert_word({"preview_side_limit"}).has_value()) {
    throw cr.except("Missing preview_side_limit!");
  }
  cr.assert_char({' '});
  auto preview_side_limit_opt = cr.read_integer();
  if (!preview_side_limit_opt.has_value()) {
    throw cr.except("Missing preview_side_limit value!");
  }
  long preview_side_limit_long = std::move(*preview_side_limit_opt);
  preview_side_limit = static_cast<size_t>(std::move(preview_side_limit_long));
  cr.next_line();
  if (!cr.assert_word({"ansi_color_present"}).has_value()) {
    throw cr.except("Missing ansi_color_present!");
  }
  cr.assert_char({' '});
  auto ansi_color_present_opt = cr.assert_word({"yes", "no"});
  if (!ansi_color_present_opt.has_value()) {
    throw cr.except("Missing ansi_color_present value!");
  }
  auto ansi_color_present = std::move(*ansi_color_present_opt);
  bool color_present = ansi_color_present == "yes" ? true : false;
  cr.next_line();
  cr.skip_newlines();

  std::map<std::string, std::unordered_set<std::string>> names;
  std::map<std::string, AsciiTextTransform::Map> gradients;
  std::map<std::string, Luminance> luminances;
  std::map<std::string, Color> colors;
  while (!cr.eof()) {
    auto section_opt =
        cr.assert_word({"gradient", "luminance", "color", "style", "pipeline"});
    if (!section_opt.has_value()) {
      throw cr.except("Missing section ID!");
    }
    std::string section = std::move(*section_opt);
    cr.assert_char({' '});
    auto name_opt = cr.assert_not_word(names[section]);
    if (!name_opt.has_value()) {
      throw cr.except("Expected " + section + " name!");
    }
    std::string name = std::move(*name_opt);
    names[section].insert(name);
    cr.next_line();
    if (section == "gradient") {
      gradients.emplace(std::move(name), AsciiTextTransform::Map::read(cr));
    }
    if (section == "color") {
      colors.emplace(std::move(name), Color::read(cr));
    }
    if (section == "luminance") {
      luminances.emplace(std::move(name), Luminance::read(cr));
    }
    if (section == "style") {
      ArtStyle::read(cr, std::move(name), color_present, names, gradients,
                     luminances, colors, styles);
    }
    if (section == "pipeline") {
      pipelines.emplace(std::move(name),
                        std::make_shared<FilterPipeline>(FilterPipeline::read(
                            cr, names, luminances, pipelines)));
    }
    cr.next_line();
    cr.skip_newlines();
  }
}
