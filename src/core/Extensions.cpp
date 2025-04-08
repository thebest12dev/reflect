#include "Extensions.h"

namespace CinnamonToast {
constexpr bool ExtensionManager::isExtensionAvailable(Extension ext) {
  switch (ext) {
  case CinnamonToast::EXTENSION_NETWORKING:
    // check if CinnamonToast.Networking is available
#ifdef CTOAST_EXT_NETWORKING
    return true;
#else
    return false;
#endif // CTOAST_EXT_NETWORKING

    break;
  case CinnamonToast::EXTENSION_UPDATER:
#ifdef CTOAST_EXT_UPDATER
    return true;
#else
    return false;
#endif // CTOAST_EXT_NETWORKING
    break;
  case CinnamonToast::EXTENSION_LUA:
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
} // namespace CinnamonToast