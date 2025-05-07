#pragma once
#include "TypeDefinitions.h"
#define UTILITY(classname)                                                     \
  classname() = delete;                                                        \
  classname(const classname &) = delete;                                       \
  classname &operator=(const classname &) = delete

namespace cinnamontoast {
typedef int Extension;
enum Extensions {
  EXTENSION_NETWORKING = 0x0000,
  EXTENSION_UPDATER = 0x0001,
  EXTENSION_LUA = 0x0002,
};
class ExtensionManager {
public:
  UTILITY(ExtensionManager);
  static constexpr bool isExtensionAvailable(Extension ext);
};
} // namespace cinnamontoast