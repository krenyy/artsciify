#pragma once

#include "filter.h"
#include <memory>
#include <vector>

class Pipeline : Filter {
  std::string name;
  std::vector<std::shared_ptr<Filter>> filters;
};
