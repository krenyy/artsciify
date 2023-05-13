#pragma once

#include "../../config_reader.h"
#include "../../luminance.h"
#include "text_transform.h"
#include <map>
#include <optional>
#include <vector>

/// An ASCII text transform.
class AsciiTextTransform : public TextTransform {
public:
  /// An ASCII gradient<->weight representation.
  class Map : public std::map<double, unsigned char> {
  public:
    /// Builds a gradient<->weight map.
    ///
    /// \param characters the ascii gradient
    /// \param brightnesses the gradient weights
    static std::optional<Map> build(std::string characters,
                                    std::vector<double> brightnesses);

    /// Reads an AsciiTextTransform::Map using a ConfigReader instance.
    static Map read(ConfigReader &cr);
  };

  /// Creates an instance of AsciiTextTransform.
  ///
  /// \param luminance a luminance value
  /// \param map a gradient<->weight map
  AsciiTextTransform(Luminance luminance, Map map);
  /// Applies this AsciiTextTransform.
  ///
  /// \param s a string to apply the transform to
  /// \param pixel reference pixel
  void apply(std::string &s, const Color pixel) const override;

  /// Reads an AsciiTextTransform using a ConfigReader instance.
  static AsciiTextTransform
  read(ConfigReader &cr,
       std::map<std::string, std::unordered_set<std::string>> &names,
       const std::map<std::string, AsciiTextTransform::Map> &gradients,
       const std::map<std::string, Luminance> &luminances);

private:
  Luminance lum;
  Map m;
};
