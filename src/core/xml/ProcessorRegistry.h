#pragma once

#include "../ui/Component.h"
#include "../ui/Window.h"
#include "TypeDefinitions.h"
#include <functional>
#include <tinyxml2.h>
#include <unordered_map>
namespace reflect {
using Processor = std::function<Component &(Window *win)>;
class ProcessorRegistry {
private:
  static std::unordered_map<std::string, Processor> mapOfProcessors;

public:
  static void createProcessor(Processor processor,
                              const std::string &elementId);
  static void invokeProcessor(const std::string &elementId, Window *win);
};
} // namespace reflect