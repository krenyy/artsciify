#pragma once

#include "artstyle.h"
#include "transforms/color_transform_bg.h"
#include "transforms/color_transform_bg_frompixel.h"
#include "transforms/color_transform_fg.h"
#include "transforms/color_transform_fg_frompixel.h"
#include "transforms/text_transform_ascii.h"
#include "transforms/text_transform_string.h"
#include "transforms/util.h"
#include <filesystem>
#include <fstream>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

class ConfigReader {
public:
  ConfigReader(std::filesystem::path p) : path(p), lines(), row(0), col(0) {
    if (!std::filesystem::exists(path)) {
      throw except("file doesn't exist!");
    }
    if (!std::filesystem::is_regular_file(path)) {
      throw except("not a regular file!");
    }
    std::ifstream is(path);
    if (!is.good()) {
      throw except("failed to open file!");
    }
    while (!is.eof()) {
      std::string line;
      std::getline(is, line);
      if (line.size() >= 2 && line.substr(0, 2) == "//") {
        lines.emplace_back();
        continue;
      }
      lines.push_back(line);
    }
  }
  std::optional<std::string> read_line() {
    if (row >= lines.size() || col != 0) {
      return std::nullopt;
    }
    col = lines[row].size();
    return lines[row];
  }
  void next_line() {
    if (lines[row].size() > col) {
      throw except("Garbage at the end of line: \"" +
                   lines[row].substr(col, lines[row].size()) + "\"!");
    }
    if (row == lines.size()) {
      throw except("Unexpected end of file!");
    }
    const std::string &line = lines[row];
    col = 0;
    size_t i = 0;
    for (const char c : line) {
      if (!std::isprint(c) || std::iscntrl(c)) {
        col = i;
        throw except("A line must contain only sane characters!");
      }
      ++i;
    }
    ++row;
  }
  std::optional<char> read_char() {
    if (col >= lines[row].size()) {
      return std::nullopt;
    }
    return lines[row][col++];
  }
  std::optional<char> assert_char(const std::unordered_set<char> &s) {
    auto c_opt = read_char();
    if (!c_opt.has_value()) {
      return std::nullopt;
    }
    char c = std::move(*c_opt);
    if (s.count(c) != 0) {
      return c;
    }
    std::ostringstream oss;
    oss << std::hex << "Expected one of {";
    bool first = true;
    for (const auto &x : s) {
      if (!first) {
        oss << ", ";
      }
      oss << '"' << x << "\" "
          << "(0x" << (int)x << ')';
      first = false;
    }
    oss << "}, got " << '"' << c << "\" "
        << "(0x" << (int)c << ')' << '!';
    col -= 1;
    throw except(oss.str());
  }
  std::optional<std::string> read_word() {
    const std::string &line = lines[row];
    std::string cur_to_end = line.substr(col, line.size());
    auto rel_end_space = cur_to_end.find_first_of(' ');
    auto rel_end_tab = cur_to_end.find_first_of('\t');
    auto rel_end = std::min(rel_end_space, rel_end_tab);
    rel_end = rel_end == std::string::npos ? cur_to_end.size() : rel_end;
    std::string word = cur_to_end.substr(0, rel_end);
    col += rel_end;
    if (word.size() == 0) {
      return std::nullopt;
    }
    return word;
  }
  std::optional<std::string>
  assert_word(const std::unordered_set<std::string> &s) {
    auto word_opt = read_word();
    if (!word_opt.has_value()) {
      return std::nullopt;
    }
    std::string word = std::move(*word_opt);
    if (s.count(word) != 0) {
      return word;
    }
    std::ostringstream oss;
    oss << std::hex << "Expected one of {";
    bool first = true;
    for (const auto &w : s) {
      if (!first) {
        oss << ", ";
      }
      oss << '"' << w << '"';
      first = false;
    }
    oss << "}, got " << '"';
    for (size_t i = 0; i < 10 && i < word.size(); ++i) {
      oss << word[i];
    }
    if (word.size() > 10) {
      oss << "...";
    }
    oss << "\" (0x";
    for (size_t i = 0; i < 10 && i < word.size(); ++i) {
      oss << (int)word[i];
    }
    if (word.size() > 10) {
      oss << "...";
    }
    oss << ")!";
    col -= word.size();
    throw except(oss.str());
  }
  std::optional<std::string>
  assert_not_word(const std::unordered_set<std::string> &s) {
    auto word_opt = read_word();
    if (!word_opt.has_value()) {
      return std::nullopt;
    }
    std::string word = std::move(*word_opt);
    if (s.count(word) == 0) {
      return word;
    }
    std::ostringstream oss;
    oss << "Expected anything else than {";
    bool first = true;
    for (const auto &x : s) {
      if (!first) {
        oss << ", ";
      }
      oss << '"' << x << '"';
      first = false;
    }
    oss << "}, got " << '"' << word << "\"!";
    col -= word.size();
    throw except(oss.str());
  }
  std::optional<double> read_double() {
    auto word_opt = read_word();
    if (!word_opt.has_value()) {
      return std::nullopt;
    }
    std::string word = std::move(*word_opt);
    size_t word_size = word.size();
    size_t pos;
    double d;
    try {
      d = std::stod(std::move(word), &pos);
    } catch (...) {
      return std::nullopt;
    }
    if (pos != word_size) {
      col -= word_size;
      throw except("Invalid double!");
    }
    return d;
  }
  std::optional<uint8_t> read_uint8() {
    auto integer_opt = read_integer();
    if (!integer_opt.has_value()) {
      return std::nullopt;
    }
    long integer = std::move(*integer_opt);
    if (integer < 0 || 255 < integer) {
      throw except("Value " + std::to_string(integer) +
                   " is out of range for type `uint8_t`!");
    }
    return integer;
  }
  std::optional<long> read_integer() {
    auto word_opt = read_word();
    if (!word_opt.has_value()) {
      return std::nullopt;
    }
    try {
      return std::stol(std::move(*word_opt));
    } catch (...) {
      return std::nullopt;
    }
  }
  void skip_newlines() {
    for (; lines[row].size() == 0;) {
      ++row;
    }
  }
  size_t get_col() const { return col; }
  size_t get_row() const { return row; }
  size_t get_current_line_length() const { return lines[row].size(); }
  bool eof() const { return row >= lines.size(); }
  std::runtime_error except(const std::string msg) const {
    std::ostringstream oss;
    oss << "Error reading config file "
        << std::filesystem::weakly_canonical(path) << ": "
        << std::to_string(row + 1) << ':' << std::to_string(col + 1) << ": "
        << msg;
    return std::runtime_error(oss.str());
  }

private:
  std::filesystem::path path;
  std::vector<std::string> lines;
  size_t row;
  size_t col;
};

struct Config {
  Config(std::filesystem::path path) : color_present(false), styles() {
    ConfigReader cr(path);
    cr.skip_newlines();
    if (!cr.assert_word({"ansi_color_present"}).has_value()) {
      throw cr.except("Missing ansi_color_present!");
    }
    cr.assert_char({' '});
    auto ansi_color_present_opt = cr.assert_word({"yes", "no"});
    if (!ansi_color_present_opt.has_value()) {
      throw cr.except("Missing ansi_color_present value!");
    }
    auto ansi_color_present = std::move(*ansi_color_present_opt);
    color_present = ansi_color_present == "yes" ? true : false;
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
        styles.emplace(name, ArtStyle(*text_transform_opt, color_transforms));
      }
      cr.next_line();
      cr.skip_newlines();
    }
  }

  bool color_present;
  std::map<std::string, ArtStyle> styles;
};
