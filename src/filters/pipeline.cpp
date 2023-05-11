#include "pipeline.h"

void FilterPipeline::apply(Image &img) const {
  for (const std::shared_ptr<Filter> &f : filters) {
    f->apply(img);
  }
}
