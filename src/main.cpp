#include "config.h"
#include <iostream>
#include <string>

int main(int, char **argv) {
  std::string relative_program_path = argv[0];
  std::string relative_program_parent_dir =
      relative_program_path.substr(0, relative_program_path.find_last_of('/'));
  std::string config_path = relative_program_parent_dir + "/artsciify.conf";
  auto config_opt = Config::load(config_path);
  if (!config_opt.has_value()) {
    throw std::logic_error("Couldn't find config file! (" + config_path + ")");
  }
  Config config = std::move(*config_opt);
  std::string img_path = "./stepech.png";
  auto img_opt = PngImage::read(img_path);
  if (!img_opt.has_value()) {
    throw std::logic_error("Couldn't find config file! (" + config_path + ")");
  }
  Image img = std::move(*img_opt);
  std::stringstream ss1, ss2;
  ss1 << config.styles.at("ascii_eddie_smith_color").print(img);
  ss2 << config.styles.at("block").print(img);
  for (size_t i = 0; i < img.height(); ++i) {
    std::string line;
    std::getline(ss1, line);
    std::cerr << line << " ";
    std::getline(ss2, line);
    std::cerr << line << std::endl;
  }
  return 0;
}
