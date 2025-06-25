#pragma once
#include "Object.h"
#include "Protocol.h"
#include <vector>
namespace reflect {
class REFLECT_API ProtocolManager : Object {
public:
  static std::vector<Protocol> getRegisteredProtocols();
  static Protocol getProtocol();
  static bool registerProtocol(Protocol proto);
}
} // namespace reflect