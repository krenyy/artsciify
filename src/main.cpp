#include <iostream>
#include <png.h>
#include <vector>

class ImageException : public std::exception {
public:
  ImageException(const char *msg) : m_msg(msg) {}
  virtual const char *what() const throw() { return m_msg; }

protected:
  const char *m_msg;
};

class PngException : public ImageException {
  using Self = PngException;
  PngException(const char *msg) : ImageException(msg) {}

public:
  static Self read_struct() { return Self("Couldn't create PNG read struct!"); }
  static Self info_struct() { return Self("Couldn't create PNG info struct!"); }
  static Self read() { return Self("Error reading the PNG file!"); }
  static Self open() { return Self("Error opening the PNG file!"); }
};

class PngImage {
public:
  /* heavily inspired by:
   * https://web.archive.org/web/20230421230936/https://gist.github.com/niw/5963798
   */
  static std::vector<std::vector<png_byte>> read_png(const char *filename) {
    png_structp png =
        png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
      throw PngException::read_struct();
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
      png_destroy_read_struct(&png, &info, NULL);
      throw PngException::info_struct();
    }

    if (setjmp(png_jmpbuf(png))) {
      png_destroy_read_struct(&png, &info, NULL);
      throw PngException::read();
    }

    FILE *fp = fopen(filename, "rb");
    if (!fp) {
      png_destroy_read_struct(&png, &info, NULL);
      throw PngException::open();
    }

    png_init_io(png, fp);
    png_read_info(png, info);

    png_uint_32 width = (int)png_get_image_width(png, info);
    png_uint_32 height = (int)png_get_image_height(png, info);
    png_byte color_type = (int)png_get_color_type(png, info);
    png_byte bit_depth = (int)png_get_bit_depth(png, info);

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

    std::vector<std::vector<png_byte>> pixels;
    for (size_t i = 0; i < height; ++i) {
      std::vector<png_byte> row;
      for (size_t j = 0; j < width; ++j) {
        row.push_back(row_pointers[i][j]);
      }
      delete[] row_pointers[i];
      pixels.push_back(row);
    }
    delete[] row_pointers;

    fclose(fp);

    png_destroy_read_struct(&png, &info, NULL);

    return pixels;
  }
};

int main() {
  auto x = PngImage::read_png("stepech.png");
  return 0;
}
