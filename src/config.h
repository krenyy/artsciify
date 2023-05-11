#pragma once

#include "artstyle.h"
#include <cstdint>
#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

class ConfigReader {
public:
  ConfigReader(std::filesystem::path p);
  std::optional<std::string> read_line();
  void next_line();
  std::optional<char> read_char();
  std::optional<char> assert_char(const std::unordered_set<char> &s);
  std::optional<std::string> read_word();
  std::optional<std::string>
  assert_word(const std::unordered_set<std::string> &s);
  std::optional<std::string>
  assert_not_word(const std::unordered_set<std::string> &s);
  std::optional<double> read_double();
  std::optional<uint8_t> read_uint8();
  std::optional<long> read_integer();
  void skip_newlines();
  size_t get_col() const;
  size_t get_row() const;
  size_t get_current_line_length() const;
  bool eof() const;
  std::runtime_error except(const std::string msg) const;

private:
  std::filesystem::path path;
  std::vector<std::string> lines;
  size_t row;
  size_t col;
};

struct Config {
  Config(std::filesystem::path path);

  bool color_present;
  std::map<std::string, ArtStyle> styles;
};
