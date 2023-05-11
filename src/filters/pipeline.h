#pragma once

#include "../config_reader.h"
#include "../luminance.h"
#include "filter.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

class FilterPipeline : public Filter {
public:
  FilterPipeline(std::vector<std::shared_ptr<Filter>> f);

  void apply(Image &img) const override;
  static FilterPipeline
  read(ConfigReader &cr,
       std::map<std::string, std::unordered_set<std::string>> &names,
       const std::map<std::string, Luminance> &luminances,
       std::map<std::string, std::shared_ptr<FilterPipeline>> &pipelines);

private:
  std::vector<std::shared_ptr<Filter>> filters;
};
