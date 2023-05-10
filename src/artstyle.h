#pragma once

#include "transforms/color_transform.h"
#include "transforms/text_transform.h"
#include <memory>

class ArtStyle {
public:
  ArtStyle(std::shared_ptr<TextTransform> text_transform,
           std::vector<std::shared_ptr<ColorTransform>> color_transforms);
  std::string print(const Image &img) const;

private:
  std::shared_ptr<TextTransform> text_transform;
  std::vector<std::shared_ptr<ColorTransform>> color_transforms;
};