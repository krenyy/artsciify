#include "config.h"
#include "filters.h"
#include "image.h"
#include "styles.h"
#include <iostream>
#include <memory>

int main(int, char **argv) {
  std::string relative_program_path = argv[0];
  std::string relative_program_parent_dir =
      relative_program_path.substr(0, relative_program_path.find_last_of('/'));
  std::string config_path = relative_program_parent_dir + "/artsciify.conf";
  auto config_opt = Config::load(config_path);
  if (!config_opt.has_value()) {
    throw std::logic_error("Couldn't find config file! (" + config_path + ")");
  }
  auto x = PngImage::read("pepa.png");
  auto img = x.value();
  // Negative().apply(img);
  // Grayscale(0.2126, 0.7152, 0.0722).apply(img);
  // Brightness(5).apply(img);
  // std::cerr << ArtStyle(
  //                  std::make_shared<StringTextTransform>("🍆"),
  //                  {
  //                      std::make_shared<FromPixelBackgroundColorTransform>(),
  //                  })
  //                  .print(img);
  // std::cerr << ArtStyle::ascii_eddie_smith().print(img);
  // std::cerr << ArtStyle::ascii_eddie_smith_color().print(img);
  return 0;
}
