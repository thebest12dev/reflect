#include "ProcessorRegistry.h"

namespace reflect {
std::unordered_map<std::string, Processor> ProcessorRegistry::mapOfProcessors =
    {};

void ProcessorRegistry::createProcessor(Processor processor,
                                        const std::string &elementId) {
  mapOfProcessors[elementId] = processor;
}
void ProcessorRegistry::invokeProcessor(const std::string &elementId,
                                        Window *win) {
  mapOfProcessors[elementId](win);
}

} // namespace reflect