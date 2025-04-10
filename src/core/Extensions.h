#pragma once
#include "TypeDefinitions.h"
#define UtilityClass(classname)                                                \
  classname() = delete;                                                        \
  classname(const classname &) = delete;                                       \
  classname &operator=(const classname &) = delete

namespace CinnamonToast {
typedef int Extension;
enum Extensions {
  EXTENSION_NETWORKING = 0x0000,
  EXTENSION_UPDATER = 0x0001,
  EXTENSION_LUA = 0x0002,
};
class ExtensionManager {
public:
  UtilityClass(ExtensionManager);
  static constexpr bool isExtensionAvailable(Extension ext);
};
} // namespace CinnamonToast