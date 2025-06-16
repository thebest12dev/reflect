#pragma once
#include "Object.h"

#include <Windows.h>
#include <any>
namespace reflect {
class Component;
using NativeHandle = HWND;
struct NativeEvent {
  UINT uMsg;
  WPARAM wParam;
  LPARAM lParam;
};
struct Event : public Object {
  reflect::Component *component = nullptr;
  NativeHandle nativeHandle = nullptr;
  NativeEvent *nativeEvent = nullptr;
};
} // namespace reflect