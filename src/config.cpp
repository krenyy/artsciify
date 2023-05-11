#include "config.h"
#include "transforms/color_transforms/background.h"
#include "transforms/color_transforms/background_from_pixel.h"
#include "transforms/color_transforms/foreground.h"
#include "transforms/color_transforms/foreground_from_pixel.h"
#include "transforms/text_transforms/ascii.h"
#include "transforms/text_transforms/string.h"
#include <fstream>

Config::Config(std::filesystem::path path) : preview_side_limit(), styles() {
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
  for (; !cr.eof();) {
    auto section_opt =
        cr.assert_word({"gradient", "luminance", "color", "style"});
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
      auto gradient_opt = cr.read_line();
      if (!gradient_opt.has_value()) {
        throw cr.except("Couldn't read gradient!");
      }
      std::string gradient = std::move(*gradient_opt);
      cr.next_line();
      std::vector<double> weights;
      for (size_t i = 0; i < gradient.size(); ++i) {
        auto weight_opt = cr.read_double();
        if (weight_opt.has_value()) {
          weights.push_back(std::move(*weight_opt));
          if (i != gradient.size() - 1) {
            cr.assert_char({' '});
          }
          continue;
        }
        if (weights.size() == 0) {
          break;
        }
        throw cr.except("Couldn't read weight!");
      }
      auto map_opt = AsciiTextTransform::Map::build(gradient, weights);
      if (!map_opt.has_value()) {
        throw cr.except(
            "Number of gradient characters and weights doesn't match!");
      }
      gradients.emplace(std::move(name), std::move(*map_opt));
    }
    if (section == "color") {
      auto r_opt = cr.read_uint8();
      if (!r_opt.has_value()) {
        throw cr.except("Missing color value for red!");
      }
      cr.assert_char({' '});
      uint8_t r = std::move(*r_opt);
      auto g_opt = cr.read_uint8();
      if (!g_opt.has_value()) {
        throw cr.except("Missing color value for green!");
      }
      cr.assert_char({' '});
      uint8_t g = std::move(*g_opt);
      auto b_opt = cr.read_uint8();
      if (!b_opt.has_value()) {
        throw cr.except("Missing color value for blue!");
      }
      uint8_t b = std::move(*b_opt);
      colors.emplace(name, Color(r, g, b));
    }
    if (section == "luminance") {
      auto r_opt = cr.read_double();
      if (!r_opt.has_value()) {
        throw cr.except("Missing luminance value for red!");
      }
      cr.assert_char({' '});
      double r = std::move(*r_opt);
      auto g_opt = cr.read_double();
      if (!g_opt.has_value()) {
        throw cr.except("Missing luminance value for green!");
      }
      cr.assert_char({' '});
      double g = std::move(*g_opt);
      auto b_opt = cr.read_double();
      if (!b_opt.has_value()) {
        throw cr.except("Missing luminance value for blue!");
      }
      double b = std::move(*b_opt);
      luminances.emplace(name, Luminance(r, g, b));
    }
    if (section == "style") {
      auto tt_name_opt =
          cr.assert_word({"AsciiTextTransform", "StringTextTransform"});
      if (!tt_name_opt.has_value()) {
        throw cr.except("Missing text transform!");
      }
      std::string tt_name = std::move(*tt_name_opt);
      cr.assert_char({' '});
      std::optional<std::shared_ptr<TextTransform>> text_transform_opt;
      if (tt_name == "AsciiTextTransform") {
        auto lum_name_opt = cr.assert_word(names["luminance"]);
        if (!lum_name_opt.has_value()) {
          throw cr.except("Missing luminance name!");
        }
        std::string lum_name = std::move(*lum_name_opt);
        if (!luminances.count(lum_name)) {
          throw cr.except("Unknown luminance: '" + lum_name + "'!");
        }
        cr.assert_char({' '});
        auto gradient_name_opt = cr.assert_word(names["gradient"]);
        if (!gradient_name_opt.has_value()) {
          throw cr.except("Missing gradient name!");
        }
        std::string gradient_name = std::move(*gradient_name_opt);
        if (!gradients.count(gradient_name)) {
          throw cr.except("Unknown gradient: '" + gradient_name + "'!");
        }
        const auto &luminance = luminances.at(lum_name);
        const auto &gradient = gradients.at(gradient_name);
        text_transform_opt =
            std::make_shared<AsciiTextTransform>(luminance, gradient);
      } else if (tt_name == "StringTextTransform") {
        cr.assert_char({'"'});
        std::string val;
        while (cr.get_col() != cr.get_current_line_length() - 1) {
          val += std::move(*cr.read_char());
        }
        cr.assert_char({'"'});
        text_transform_opt = std::make_shared<StringTextTransform>(val);
      }
      std::vector<std::shared_ptr<ColorTransform>> color_transforms;
      for (;;) {
        cr.next_line();
        auto ct_name_opt = cr.assert_word({
            "BackgroundColorTransform",
            "ForegroundColorTransform",
            "FromPixelBackgroundColorTransform",
            "FromPixelForegroundColorTransform",
        });
        if (!ct_name_opt.has_value()) {
          break;
        }
        std::string ct_name = std::move(*ct_name_opt);
        if (ct_name == "FromPixelForegroundColo"
                       "rTransform") {
          color_transforms.push_back(
              std::make_shared<FromPixelForegroundColorTransform>());
        }
        if (ct_name == "FromPixelBackgroundColorTransform") {
          color_transforms.push_back(
              std::make_shared<FromPixelBackgroundColorTransform>());
        }
        if (ct_name == "ForegroundColorTransform") {
          cr.assert_char({' '});
          auto color_name_opt = cr.assert_word(names["color"]);
          if (!color_name_opt.has_value()) {
            throw cr.except("Missing color!");
          }
          std::string color_name = std::move(*color_name_opt);
          color_transforms.push_back(
              std::make_shared<ForegroundColorTransform>(colors[color_name]));
        }
        if (ct_name == "BackgroundColorTransform") {
          cr.assert_char({' '});
          auto color_name_opt = cr.assert_word(names["color"]);
          if (!color_name_opt.has_value()) {
            throw cr.except("Missing color!");
          }
          std::string color_name = std::move(*color_name_opt);
          color_transforms.push_back(
              std::make_shared<BackgroundColorTransform>(colors[color_name]));
        }
      }
      if (color_present || color_transforms.size() == 0) {
        styles.emplace(name, ArtStyle(*text_transform_opt, color_transforms));
      }
    }
    cr.next_line();
    cr.skip_newlines();
  }
}
