#pragma once
#include "../Component.h"
#include "Event.h"
#include <any>
namespace reflect {
struct KeyboardEvent : public Event {
  char key;
  unsigned short keyCode;
};
} // namespace reflect