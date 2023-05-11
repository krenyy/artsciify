#pragma once

#include "filter.h"
#include <memory>
#include <string>
#include <vector>

class FilterPipeline : public Filter {
public:
  void apply(Image &img) const override;

private:
  std::string name;
  std::vector<std::shared_ptr<Filter>> filters;
};
