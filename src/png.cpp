#include "png.h"
#include <png.h>

PngImage::PngImage(const std::filesystem::path &p) : path(p) {}

std::runtime_error PngImage::except(const std::string &s) const {
  std::ostringstream oss;
  oss << "Error reading png file " << std::filesystem::weakly_canonical(path)
      << ": " << s;
  return std::runtime_error(oss.str());
}

Image PngImage::read() const {
  if (!std::filesystem::exists(path)) {
    throw except("file doesn't exist!");
  }

  if (!std::filesystem::is_regular_file(path)) {
    throw except("not a regular file!");
  }

  png_structp png =
      png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  if (!png) {
    throw except("libpng initialization error!");
  }

  png_infop info = png_create_info_struct(png);
  if (!info) {
    png_destroy_read_struct(&png, &info, nullptr);
    throw except("libpng initialization error!");
  }

  if (setjmp(png_jmpbuf(png))) {
    png_destroy_read_struct(&png, &info, nullptr);
    throw except("libpng runtime error!");
  }

  FILE *fp = fopen(path.c_str(), "rb");
  if (!fp) {
    png_destroy_read_struct(&png, &info, nullptr);
    throw except("failed to open file!");
  }

  png_init_io(png, fp);
  png_read_info(png, info);

  png_uint_32 width = png_get_image_width(png, info);
  png_uint_32 height = png_get_image_height(png, info);
  png_byte color_type = png_get_color_type(png, info);
  png_byte bit_depth = png_get_bit_depth(png, info);

  if (bit_depth == 16)
    png_set_strip_16(png);

  if (color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png);

  // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
  if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8(png);

  if (png_get_valid(png, info, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png);

  // These color_type don't have an alpha channel then fill it with 0xff.
  if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY ||
      color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

  if (color_type == PNG_COLOR_TYPE_GRAY ||
      color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(png);

  png_read_update_info(png, info);

  png_bytep *row_pointers = new png_bytep[height];
  for (size_t y = 0; y < height; y++) {
    row_pointers[y] =
        new png_byte[png_get_rowbytes(png, info) / sizeof(png_byte)];
  }
  png_read_image(png, row_pointers);

  Image image(height, width);
  for (size_t i = 0; i < 3; ++i) {
    for (size_t j = 0; j < height; ++j) {
      for (size_t k = 0; k < width; ++k) {
        image[j][k][i] = row_pointers[j][k * 4 + i];
      }
    }
  }
  for (size_t i = 0; i < height; ++i) {
    delete[] row_pointers[i];
  }
  delete[] row_pointers;

  fclose(fp);

  png_destroy_read_struct(&png, &info, nullptr);

  return image;
}
