#pragma once
#include "TypeDefinitions.h"
#include <string>
namespace cinnamontoast {
class Notification {
private:
  std::string title, text;

public:
  CTOAST_API Notification();
  CTOAST_API Notification(std::string title, std::string text);
  CTOAST_API Notification(std::string text);
  CTOAST_API void setTitle(std::string title);
  CTOAST_API void setText(std::string text);
  CTOAST_API ~Notification();
  friend class Window;
};
} // namespace cinnamontoast

// add more platform support