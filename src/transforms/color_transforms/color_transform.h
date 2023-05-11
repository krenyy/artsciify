#pragma once

#include "../pixel_transform.h"

std::string rgb_to_fg_color_code(const Color c);
std::string rgb_to_bg_color_code(const Color c);

class ColorTransform : public PixelTransform {};
