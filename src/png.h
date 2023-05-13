#pragma once

#include "image.h"
#include <filesystem>

/// A container for parsing PNG files.
class PngImage {
public:
  /// Constructs a PngImage object from the given path.
  ///
  /// \param path png file path
  PngImage(const std::filesystem::path &path);
  /// Utility method for generating an error message.
  ///
  /// \param s string to use for generating the message
  /// \returns std::runtime_error containing the generated message
  std::runtime_error except(const std::string &s) const;
  /// Reads the PNG file from the stored path.
  ///
  /// Most of the code is heavily inspired by
  /// https://web.archive.org/web/20230421230936/https://gist.github.com/niw/5963798
  ///
  /// \returns an Image object representing the parsed image
  /// \throws std::runtime_error if the file doesn't exist, couldn't be opened,
  /// or some other error happened
  Image read() const;

private:
  std::filesystem::path path;
};
