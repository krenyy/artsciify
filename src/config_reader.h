#pragma once

#include <filesystem>
#include <optional>
#include <unordered_set>
#include <vector>

/// A helper class for parsing a configuration file.
///
/// Implements a bunch of useful methods for parsing various option types.
class ConfigReader {
public:
  /// Constructs a ConfigReader object from the given path.
  ///
  /// \param p The path to read from
  /// \throws std::runtime_error if failed to open the file, or the files is too
  /// large
  ConfigReader(std::filesystem::path p);

  /// Skips to the next line in the file.
  ///
  /// \throws std::runtime_error if the current line isn't depleted yet, if EOF
  /// is reached, or the line contains weird characters
  void next_line();
  /// Skips all newlines starting from the current line.
  void skip_newlines();
  /// Reads the current line.
  ///
  /// \returns std::nullopt if at EOF or not at the start of a line, the line
  /// std::string otherwise
  std::optional<std::string> read_line();

  /// Reads a character.
  ///
  /// \returns std::nullopt if no character left in the current line, otherwise
  /// a char
  /// \throws std::runtime_error if EOF reached
  std::optional<char> read_char();
  /// Asserts that the read character is from a specified list.
  ///
  /// \param s Set containing the wanted characters
  /// \returns std::nullopt if character couldn't be read, otherwise char
  /// \throws std::runtime_error if the read character isn't in the set
  std::optional<char> assert_char(const std::unordered_set<char> &s);

  /// Reads a word.
  ///
  /// \returns std::nullopt if the read word is empty, otherwise a word
  /// \throws std::runtime_error if EOF reached
  std::optional<std::string> read_word();
  /// Asserts a word is contained in a provided list.
  ///
  /// \param s Set containing the allowed words
  /// \returns std::nullopt if a word couldn't be read
  /// \throws std::runtime_error if word doesn't match any word in the set
  std::optional<std::string>
  assert_word(const std::unordered_set<std::string> &s);
  /// Asserts a word is **not** contained in a provided list.
  ///
  /// \param s Set containing the disallowed words
  /// \returns std::nullopt if a word couldn't be read
  /// \throws std::runtime_error if word matches any word in the set
  std::optional<std::string>
  assert_not_word(const std::unordered_set<std::string> &s);

  /// Reads an integer.
  ///
  /// \returns the integer, or std::nullopt if an integer couldn't be read or
  /// converted
  /// \throws if an invalid integer is specified, for example "2," or "2a"
  std::optional<long> read_integer();
  /// Reads a unsigned 8bit integer.
  ///
  /// \returns the integer, or std::nullopt if an integer couldn't be read or
  /// converted
  /// \throws std::runtime_error if the integer is out of the valid range
  std::optional<uint8_t> read_uint8();
  /// Reads a double.
  ///
  /// \returns the double, or std::nullopt if the double couldn't be read or
  /// converted.
  /// \throws if an invalid double is specific, for example "2,1"
  /// instead of "2.1"
  std::optional<double> read_double();

  bool eof() const;

  size_t get_col() const;
  size_t get_row() const;
  size_t get_current_line_length() const;

  /// A convenience method for generating an error message in a specific format
  ///
  /// \returns std::runtime_error object containing the final message
  std::runtime_error except(const std::string msg) const;

private:
  std::filesystem::path path;
  std::vector<std::string> lines;
  size_t row;
  size_t col;
};
