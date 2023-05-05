#include "styles.h"
#include <fstream>
#include <iostream>
#include <map>
#include <optional>

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &v) {
  os << "[\n";
  for (const auto &x : v) {
    os << "  " << x << ",\n";
  }
  return os << "]";
}

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

class Config {
public:
  static std::optional<Config> load(const std::string &path) {
    using namespace std::string_literals;
    auto ro = ConfigReader::create(path);
    if (!ro.has_value()) {
      return std::nullopt;
    }
    ConfigReader cr = std::move(*ro);
    Config c;
    while (true) {
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
        if (c.gradients.count(name)) {
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
        c.gradients.emplace(name,
                            AsciiTextTransform::Map::build(gradient, weights));
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
        if (c.luminances.count(name)) {
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
        c.luminances.emplace(name, std::make_tuple(r, g, b));
      } else {
        throw std::logic_error("Unexpected symbol '" + buf + "'! (at line "s +
                               std::to_string(cr.get_current_line()) + ")");
      }
    }
    std::cerr << "*** WHAT HAS BEEN PARSED?" << std::endl;
    for (const auto &[x, y] : c.gradients) {
      std::cerr << "  " << '"' << x << '"' << ": " << std::endl;
      for (const auto &[z, w] : y) {
        std::cerr << "    " << z << ": " << w << std::endl;
      }
    }
    std::cerr << "***" << std::endl;
    return c;
  }

private:
  std::map<std::string, AsciiTextTransform::Map> gradients;
  std::map<std::string, std::tuple<double, double, double>> luminances;
};
