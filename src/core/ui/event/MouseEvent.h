#pragma once
#include "../Component.h"
#include "Event.h"
#include <any>
namespace reflect {
enum MouseClickType { LEFT_CLICK, RIGHT_CLICK, MIDDLE_CLICK };
struct MouseEvent : public Event {
  int x, y;
  MouseClickType clickType;
};
} // namespace reflect