#include "config_reader.h"
#include <fstream>

ConfigReader::ConfigReader(std::filesystem::path p)
    : path(p), lines(), row(0), col(0) {
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
    line.clear();
    for (char c; (c = static_cast<char>(is.get())) != '\n';) {
      if (is.bad()) {
        throw except("error during read!");
      }
      if (c == EOF) {
        break;
      }
      line.push_back(c);
      if (line.size() > 1000) {
        throw except("line is too long!");
      }
    }
    if (line.size() >= 2 && line.substr(0, 2) == "//") {
      lines.emplace_back();
      continue;
    }
    lines.push_back(line);
    if (lines.size() > 5000) {
      throw except("way too many lines!");
    }
  }
}

void ConfigReader::next_line() {
  if (lines.at(row).size() > col) {
    throw except("Garbage at the end of line: \"" +
                 lines.at(row).substr(col, lines.at(row).size()) + "\"!");
  }
  if (row >= lines.size()) {
    throw except("Unexpected end of file!");
  }
  const std::string &line = lines.at(row);
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
void ConfigReader::skip_newlines() {
  while (row < lines.size() && lines.at(row).size() == 0) {
    ++row;
  }
}
std::optional<std::string> ConfigReader::read_line() {
  if (row >= lines.size() || col != 0) {
    return std::nullopt;
  }
  col = lines.at(row).size();
  return lines.at(row);
}

std::optional<char> ConfigReader::read_char() {
  if (row >= lines.size()) {
    throw except("Unexpected end of file!");
  }
  if (col >= lines.at(row).size()) {
    return std::nullopt;
  }
  return lines.at(row).at(col++);
}
std::optional<char>
ConfigReader::assert_char(const std::unordered_set<char> &s) {
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

std::optional<std::string> ConfigReader::read_word() {
  if (row >= lines.size()) {
    throw except("Unexpected end of file!");
  }
  const std::string &line = lines.at(row);
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
ConfigReader::assert_word(const std::unordered_set<std::string> &s) {
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
ConfigReader::assert_not_word(const std::unordered_set<std::string> &s) {
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

std::optional<long> ConfigReader::read_integer() {
  auto word_opt = read_word();
  if (!word_opt.has_value()) {
    return std::nullopt;
  }
  std::string word = std::move(*word_opt);
  size_t word_size = word.size();
  size_t pos;
  long n;
  try {
    n = std::stol(std::move(word), &pos);
  } catch (const std::exception &) {
    return std::nullopt;
  }
  if (pos != word_size) {
    col -= word_size;
    throw except("Invalid integer!");
  }
  return n;
}
std::optional<uint8_t> ConfigReader::read_uint8() {
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
std::optional<double> ConfigReader::read_double() {
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
  } catch (const std::exception &) {
    return std::nullopt;
  }
  if (pos != word_size) {
    col -= word_size;
    throw except("Invalid double!");
  }
  return d;
}

bool ConfigReader::eof() const { return row >= lines.size(); }

size_t ConfigReader::get_col() const { return col; }
size_t ConfigReader::get_row() const { return row; }
size_t ConfigReader::get_current_line_length() const {
  return lines.at(row).size();
}

std::runtime_error ConfigReader::except(const std::string msg) const {
  std::ostringstream oss;
  oss << "Error reading config file " << std::filesystem::weakly_canonical(path)
      << ": " << std::to_string(row + 1) << ':' << std::to_string(col + 1)
      << ": " << msg;
  return std::runtime_error(oss.str());
}
