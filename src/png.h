#pragma once

#include "image.h"
#include <filesystem>

class PngImage {
public:
  PngImage(const std::filesystem::path &path);
  std::runtime_error except(const std::string &s) const;
  Image read() const;

private:
  std::filesystem::path path;
};
