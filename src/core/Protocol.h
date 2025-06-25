#pragma once
#include "Object.h"

namespace reflect {
class REFLECT_API Protocol : Object {
private:
  std::string name;

protected:
  Protocol();

public:
  /**
   * @brief Whether or not the protocol is registered into the Windows Registry
   * (or equivalent in other operating systems).
   *
   * @return If the protocol is registered.
   */
  bool isRegistered();

  /**
   * @brief Returns the parsed URL (excluding protocol).
   *
   * @return The parsed URL (e.g. foo/bar?baz=true from
   * myapp://foo/bar?baz=true)
   */
  std::string getParsedURL();
  /**
   * @brief Returns the full URL (including protocol).
   *
   * @return The URL (e.g. myapp://foo/bar?baz=true)
   */
  std::string getURL();
  /**
   * @brief Returns the protocol name.
   *
   * @return The protocol name(e.g. myapp)
   */
  std::string getProtocolName();
  ~Protocol();
  /**
   * @brief Creates a Protocol object by protocol name.
   *
   * @param protocolName The protocol name (e.g. myapp)
   * @return The created Protocol object
   */
  static Protocol from(std::string protocolName);

  /**
   * @brief Creates a Protocol object, but requiring the full URL.
   *
   * @param url The fully-qualified URL (e.g. myapp://foo)
   * @return The created Protocol object
   */
  static Protocol from(std::string url);
  friend class ProtocolManager;
}
} // namespace reflect