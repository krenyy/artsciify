#pragma once

#include "../config_reader.h"
#include "../luminance.h"
#include "filter.h"
#include <map>
#include <string>
#include <vector>

class FilterPipeline : public Filter {
public:
  FilterPipeline(std::vector<std::shared_ptr<Filter>> f);

  virtual void apply(Image &img) const override;
  virtual void apply_without_scaling(Image &img) const override;
  virtual void get_final_dimensions(size_t &width, size_t &height,
                                    size_t &max_width,
                                    size_t &max_height) const override;

  static FilterPipeline
  read(ConfigReader &cr,
       std::map<std::string, std::unordered_set<std::string>> &names,
       const std::map<std::string, Luminance> &luminances,
       std::map<std::string, std::shared_ptr<FilterPipeline>> &pipelines);

private:
  std::vector<std::shared_ptr<Filter>> filters;
};
