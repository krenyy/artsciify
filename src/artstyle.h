#pragma once

#include "image.h"
#include "transforms/color_transforms/color_transform.h"
#include "transforms/text_transforms/text_transform.h"
#include <memory>
#include <vector>

class ArtStyle {
public:
  ArtStyle(std::shared_ptr<TextTransform> text_transform,
           std::vector<std::shared_ptr<ColorTransform>> color_transforms);
  std::string print(const Image &img) const;

private:
  std::shared_ptr<TextTransform> text_transform;
  std::vector<std::shared_ptr<ColorTransform>> color_transforms;
};
