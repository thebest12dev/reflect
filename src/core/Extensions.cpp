#include "Extensions.h"

namespace reflect {
constexpr bool ExtensionManager::isExtensionAvailable(Extension ext) {
  switch (ext) {
  case reflect::EXTENSION_NETWORKING:
    // check if Reflect.Networking is available
#ifdef REFLECT_EXT_NETWORKING
    return true;
#else
    return false;
#endif // REFLECT_EXT_NETWORKING

    break;
  case reflect::EXTENSION_UPDATER:
#ifdef REFLECT_EXT_UPDATER
    return true;
#else
    return false;
#endif // REFLECT_EXT_NETWORKING
    break;
  case reflect::EXTENSION_LUA:
#ifdef REFLECT_LUA
    return true;
#else
    return false;
#endif // REFLECT_EXT_NETWORKING
    break;
  default:
    return false;
    break;
  }
};
} // namespace reflect