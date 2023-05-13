#pragma once

#include "../config_reader.h"
#include "../luminance.h"
#include "filter.h"
#include <map>
#include <string>
#include <vector>

/// A filter pipeline
///
/// This is an object that can contain multiple filters, and can itself be
/// contained in another pipeline.
class FilterPipeline : public Filter {
public:
  /// Constructs a FilterPipeline.
  ///
  /// \param f filters
  FilterPipeline(std::vector<std::shared_ptr<Filter>> f);

  /// Applies this FilterPipeline.
  ///
  /// \param img image to apply the filter pipeline to
  virtual void apply(Image &img) const override;
  /// Applies this FilterPipeline without scaling filters.
  ///
  /// \param img image to apply the filter pipeline to
  virtual void apply_without_scaling(Image &img) const override;
  /// Get final dimensions of the image after applying this FilterPipeline.
  ///
  /// \param width current width of the image
  /// \param height current height of the image
  /// \param max_width maximum width the image ever reaches
  /// \param max_height maximum height the image ever reaches
  virtual void get_final_dimensions(size_t &width, size_t &height,
                                    size_t &max_width,
                                    size_t &max_height) const override;

  /// Reads a FilterPipeline using a ConfigReader instance and other params.
  static FilterPipeline
  read(ConfigReader &cr,
       std::map<std::string, std::unordered_set<std::string>> &names,
       const std::map<std::string, Luminance> &luminances,
       std::map<std::string, std::shared_ptr<FilterPipeline>> &pipelines);

private:
  std::vector<std::shared_ptr<Filter>> filters;
};
