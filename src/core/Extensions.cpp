#include "Extensions.h"

namespace cinnamontoast {
constexpr bool ExtensionManager::isExtensionAvailable(Extension ext) {
  switch (ext) {
  case cinnamontoast::EXTENSION_NETWORKING:
    // check if CinnamonToast.Networking is available
#ifdef CTOAST_EXT_NETWORKING
    return true;
#else
    return false;
#endif // CTOAST_EXT_NETWORKING

    break;
  case cinnamontoast::EXTENSION_UPDATER:
#ifdef CTOAST_EXT_UPDATER
    return true;
#else
    return false;
#endif // CTOAST_EXT_NETWORKING
    break;
  case cinnamontoast::EXTENSION_LUA:
#ifdef CTOAST_LUA
    return true;
#else
    return false;
#endif // CTOAST_EXT_NETWORKING
    break;
  default:
    return false;
    break;
  }
};
} // namespace cinnamontoast