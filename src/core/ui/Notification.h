#pragma once
#include "TypeDefinitions.h"
#include <string>
namespace reflect {
class Notification {
private:
  std::string title, text;

public:
  REFLECT_API Notification();
  REFLECT_API Notification(std::string title, std::string text);
  REFLECT_API Notification(std::string text);
  REFLECT_API void setTitle(std::string title);
  REFLECT_API void setText(std::string text);
  REFLECT_API ~Notification();
  friend class Window;
};
} // namespace reflect

// add more platform support