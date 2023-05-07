#pragma once

#include "artstyle.h"
#include "transforms/color_transform_bg_frompixel.h"
#include "transforms/color_transform_fg_frompixel.h"
#include "transforms/text_transform_ascii.h"
#include "transforms/text_transform_string.h"
#include "transforms/util.h"
#include <fstream>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

class ConfigReader {
public:
  static std::optional<ConfigReader> create(const std::string &path) {
    ConfigReader r(path);
    if (!r.is.good()) {
      return std::nullopt;
    }
    return r;
  }
  std::optional<std::string> read_line() {
    std::string line;
    bool first = true;
    while (first || line.substr(0, 2) == "//") {
      first = false;
      std::getline(is, line);
      ++current_line;
      if (is.eof()) {
        return std::nullopt;
      }
    }
    return line;
  }
  size_t get_current_line() const { return current_line; }

private:
  ConfigReader(const std::string &path) : is(path), current_line(0) {}
  std::ifstream is;
  size_t current_line;
};

struct Config {
  static std::optional<Config> load(const std::string &path) {
    using namespace std::string_literals;
    auto ro = ConfigReader::create(path);
    if (!ro.has_value()) {
      return std::nullopt;
    }
    ConfigReader cr = std::move(*ro);
    Config conf{};
    for (;;) {
      auto line_opt = cr.read_line();
      if (!line_opt.has_value()) {
        break;
      }
      std::string line = *line_opt;
      std::istringstream iss(std::move(line));
      std::string buf;
      iss >> buf;
      if (buf.size() == 0) {
        continue;
      }
      if (buf == "gradient") {
        std::string name;
        iss >> name;
        if (name.size() == 0) {
          throw std::logic_error("Missing gradient name! (at line "s +
                                 std::to_string(cr.get_current_line()) + ")");
        }
        for (const char c : name) {
          if (!std::isprint(c)) {
            throw std::logic_error(
                "A gradient name must contain only printable characters! (at line "s +
                std::to_string(cr.get_current_line()) + ")");
          }
        }
        if (conf.gradients.count(name)) {
          throw std::logic_error("A gradient with the name '" + name +
                                 "' already exists! (at line "s +
                                 std::to_string(cr.get_current_line()) + ")");
        }
        buf.clear();
        iss >> buf;
        if (buf.size()) {
          throw std::logic_error("Unexpected symbol '" + buf + "'! (at line "s +
                                 std::to_string(cr.get_current_line()) + ")");
        }
        auto gradient_opt = cr.read_line();
        if (!gradient_opt.has_value()) {
          throw std::logic_error("Couldn't parse gradient! (at line "s +
                                 std::to_string(cr.get_current_line()) + ")");
        }
        std::string gradient = *gradient_opt;
        for (const char c : gradient) {
          if (!std::isprint(c)) {
            throw std::logic_error(
                "Gradient must contain only printable characters! (at line "s +
                std::to_string(cr.get_current_line()) + ")");
          }
        }
        line_opt = cr.read_line();
        if (!line_opt.has_value()) {
          throw std::logic_error("Unexpected EOF! (at line "s +
                                 std::to_string(cr.get_current_line()) + ")");
        }
        line = *line_opt;
        std::vector<double> weights;
        if (line.size() != 0) {
          iss = std::istringstream(std::move(line));
          double weight;
          while (iss >> weight) {
            weights.push_back(weight);
          }
        }
        auto map_opt = AsciiTextTransform::Map::build(gradient, weights);
        if (!map_opt.has_value()) {
          throw std::logic_error(
              "Number of gradient characters and weights doesn't match! (at line "s +
              std::to_string(cr.get_current_line()) + ")");
        }
        conf.gradients.emplace(std::move(name), std::move(*map_opt));
      } else if (buf == "luminance") {
        std::string name;
        iss >> name;
        if (name.size() == 0) {
          throw std::logic_error("Missing luminance name! (at line "s +
                                 std::to_string(cr.get_current_line()) + ")");
        }
        for (const char c : name) {
          if (!std::isprint(c)) {
            throw std::logic_error(
                "A luminance name must contain only printable characters! (at line "s +
                std::to_string(cr.get_current_line()) + ")");
          }
        }
        if (conf.luminances.count(name)) {
          throw std::logic_error("A luminance with the name '" + name +
                                 "' already exists! (at line "s +
                                 std::to_string(cr.get_current_line()) + ")");
        }
        buf.clear();
        iss >> buf;
        if (buf.size()) {
          throw std::logic_error("Unexpected symbol '" + buf + "'! (at line "s +
                                 std::to_string(cr.get_current_line()) + ")");
        }
        line_opt = cr.read_line();
        if (!line_opt.has_value()) {
          throw std::logic_error("Unexpected EOF! (at line "s +
                                 std::to_string(cr.get_current_line()) + ")");
        }
        line = *line_opt;
        iss = std::istringstream(std::move(line));
        double r, g, b;
        if (!(iss >> r)) {
          throw std::logic_error("Missing luminance value for red! (at line "s +
                                 std::to_string(cr.get_current_line()) + ")");
        }
        if (!(iss >> g)) {
          throw std::logic_error(
              "Missing luminance value for green! (at line "s +
              std::to_string(cr.get_current_line()) + ")");
        }
        if (!(iss >> b)) {
          throw std::logic_error(
              "Missing luminance value for blue! (at line "s +
              std::to_string(cr.get_current_line()) + ")");
        }
        conf.luminances.emplace(name, Luminance(r, g, b));
      } else if (buf == "style") {
        std::string name;
        iss >> name;
        if (name.size() == 0) {
          throw std::logic_error("Missing style name! (at line "s +
                                 std::to_string(cr.get_current_line()) + ")");
        }
        for (const char c : name) {
          if (!std::isprint(c)) {
            throw std::logic_error(
                "A style name must contain only printable characters! (at line "s +
                std::to_string(cr.get_current_line()) + ")");
          }
        }
        if (conf.styles.count(name)) {
          throw std::logic_error("A style with the name '" + name +
                                 "' already exists! (at line "s +
                                 std::to_string(cr.get_current_line()) + ")");
        }
        buf.clear();
        iss >> buf;
        if (buf.size()) {
          throw std::logic_error("Unexpected symbol '" + buf + "'! (at line "s +
                                 std::to_string(cr.get_current_line()) + ")");
        }
        line_opt = cr.read_line();
        if (!line_opt.has_value()) {
          throw std::logic_error("Unexpected EOF! (at line "s +
                                 std::to_string(cr.get_current_line()) + ")");
        }
        line = *line_opt;
        iss = std::istringstream(std::move(line));
        buf.clear();
        iss >> buf;
        std::optional<std::shared_ptr<TextTransform>> text_transform_opt;
        if (buf == "AsciiTextTransform") {
          buf.clear();
          iss >> buf;
          if (!conf.luminances.count(buf)) {
            throw std::logic_error("Unknown luminance: '" + buf +
                                   "'! (at line "s +
                                   std::to_string(cr.get_current_line()) + ")");
          }
          const auto &luminance = conf.luminances.at(buf);
          buf.clear();
          iss >> buf;
          if (!conf.gradients.count(buf)) {
            throw std::logic_error("Unknown gradient: '" + buf +
                                   "'! (at line "s +
                                   std::to_string(cr.get_current_line()) + ")");
          }
          const auto &gradient = conf.gradients.at(buf);
          text_transform_opt =
              std::make_shared<AsciiTextTransform>(luminance, gradient);
        } else if (buf == "StringTextTransform") {
          if (iss.eof() ||
              (buf = iss.str().substr(static_cast<size_t>(iss.tellg()) + 1,
                                      iss.str().size()),
               !buf.size())) {
            throw std::logic_error(
                "Argument expected for 'StringTextTransform'! (at line "s +
                std::to_string(cr.get_current_line()) + ")");
          }
          if (buf[0] != '"' || buf[buf.size() - 1] != '"') {
            throw std::logic_error(
                "Argument for 'StringTextTransform' must be enclosed in "
                "quotation marks (\"<arg>\")! (at line "s +
                std::to_string(cr.get_current_line()) + ")");
          }
          buf = buf.substr(1, buf.size() - 2);
          if (!buf.size()) {
            throw std::logic_error(
                "Argument for 'StringTextTransform' must not be empty! (at line "s +
                std::to_string(cr.get_current_line()) + ")");
          }
          text_transform_opt = std::make_shared<StringTextTransform>(buf);
        } else {
          throw std::logic_error("Unknown text transform '" + buf +
                                 "'! (at line "s +
                                 std::to_string(cr.get_current_line()) + ")");
        }
        std::vector<std::shared_ptr<ColorTransform>> color_transforms;
        for (;;) {
          line_opt = cr.read_line();
          if (!line_opt.has_value() ||
              (line = std::move(*line_opt), line.size() == 0)) {
            break;
          }
          iss = std::istringstream(line);
          buf.clear();
          iss >> buf;
          if (buf == "FromPixelForegroundColorTransform") {
            buf.clear();
            iss >> buf;
            if (buf.size()) {
              throw std::logic_error(
                  "Unexpected symbol '" + buf + "'! (at line "s +
                  std::to_string(cr.get_current_line()) + ")");
            }
            color_transforms.push_back(
                std::make_shared<FromPixelForegroundColorTransform>());
          } else if (buf == "FromPixelBackgroundColorTransform") {
            buf.clear();
            iss >> buf;
            if (buf.size()) {
              throw std::logic_error(
                  "Unexpected symbol '" + buf + "'! (at line "s +
                  std::to_string(cr.get_current_line()) + ")");
            }
            color_transforms.push_back(
                std::make_shared<FromPixelBackgroundColorTransform>());
          } else {
            throw std::logic_error("Unknown color transform '" + buf +
                                   "'! (at line "s +
                                   std::to_string(cr.get_current_line()) + ")");
          }
        }
        conf.styles.emplace(name,
                            ArtStyle(*text_transform_opt, color_transforms));
      } else {
        throw std::logic_error("Unexpected symbol '" + buf + "'! (at line "s +
                               std::to_string(cr.get_current_line()) + ")");
      }
    }
    return conf;
  }

  std::map<std::string, AsciiTextTransform::Map> gradients;
  std::map<std::string, Luminance> luminances;
  std::map<std::string, ArtStyle> styles;
};
