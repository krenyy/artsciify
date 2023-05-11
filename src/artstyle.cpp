#include "artstyle.h"
#include "transforms/color_transforms/background.h"
#include "transforms/color_transforms/background_from_pixel.h"
#include "transforms/color_transforms/foreground.h"
#include "transforms/color_transforms/foreground_from_pixel.h"
#include "transforms/text_transforms/ascii.h"
#include "transforms/text_transforms/string.h"
#include <sstream>

ArtStyle::ArtStyle(std::shared_ptr<TextTransform> transform,
                   std::vector<std::shared_ptr<ColorTransform>> transforms)
    : text_transform(transform), color_transforms(transforms) {}
std::string ArtStyle::print(const Image &img) const {
  std::ostringstream oss;
  for (const Image::Row &row : img) {
    for (const Color &pixel : row) {
      std::string s;
      // apply twice to make it proportionally
      // closer to the source image as fonts
      // usually have height larger than width
      text_transform->apply(s, pixel);
      text_transform->apply(s, pixel);
      for (const std::shared_ptr<ColorTransform> &t : color_transforms) {
        t->apply(s, pixel);
      }
      oss << s;
    }
    if (!color_transforms.empty()) {
      oss << "\033[0;0m";
    }
    oss << '\n';
  }
  return oss.str();
}

void ArtStyle::read(
    ConfigReader &cr, std::string name, const bool color_present,
    std::map<std::string, std::unordered_set<std::string>> &names,
    const std::map<std::string, AsciiTextTransform::Map> &gradients,
    const std::map<std::string, Luminance> &luminances,
    const std::map<std::string, Color> &colors,
    std::map<std::string, ArtStyle> &styles) {
  auto tt_name_opt =
      cr.assert_word({"AsciiTextTransform", "StringTextTransform"});
  if (!tt_name_opt.has_value()) {
    throw cr.except("Missing text transform!");
  }
  std::string tt_name = std::move(*tt_name_opt);
  cr.assert_char({' '});

  std::optional<std::shared_ptr<TextTransform>> text_transform_opt;
  if (tt_name == "AsciiTextTransform") {
    text_transform_opt = std::make_shared<AsciiTextTransform>(
        AsciiTextTransform::read(cr, names, gradients, luminances));
  } else if (tt_name == "StringTextTransform") {
    text_transform_opt =
        std::make_shared<StringTextTransform>(StringTextTransform::read(cr));
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
      color_transforms.push_back(std::make_shared<ForegroundColorTransform>(
          ForegroundColorTransform::read(cr, names, colors)));
    }
    if (ct_name == "BackgroundColorTransform") {
      color_transforms.push_back(std::make_shared<BackgroundColorTransform>(
          BackgroundColorTransform::read(cr, names, colors)));
    }
  }
  if (color_present || color_transforms.size() == 0) {
    styles.emplace(std::move(name),
                   ArtStyle(*text_transform_opt, color_transforms));
  }
}
