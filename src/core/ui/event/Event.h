#pragma once
#include "Object.h"
#include "../Component.h"
namespace reflect {
class Event : public Object {
  unsigned short eventId;
  Component *component;
  REFLECT_API Event(); 
  REFLECT_API virtual ~Event() = 0;
};
} // namespace reflect