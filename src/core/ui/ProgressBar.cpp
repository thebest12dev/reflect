#include "ProgressBar.h"
#include "Canvas.h"
#include "Window.h"
#include <CommCtrl.h>
#include <Windows.h>
namespace reflect {
ProgressBar::ProgressBar() : minValue(0.0f), value(0.0f), maxValue(100.0f) {
  initializeObject(REFLECT_OBJECT_PROGRESSBAR, REFLECT_OBJECT_COMPONENT);
  this->setClassName("ProgressBar");
  this->setStyles(WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
}
// void ProgressBar::render(HWND &parentHWND, HWND &windowHWND) {
//   reflect::Window *window = reinterpret_cast<reflect::Window *>(
//       GetWindowLongPtr(windowHWND, GWLP_USERDATA));
//   hwnd = CreateWindowEx(
//       0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE, position.x,
//       position.y + window->getProperty<int>("customTitleBarSize"), size.x,
//       size.y, parentHWND, NULL, winstance, NULL);
//   SendMessage(hwnd, PBM_SETRANGE32, (WPARAM)minValue, (LPARAM)maxValue);
//   SendMessage(hwnd, PBM_SETPOS, (WPARAM)value, 0);
// }
void ProgressBar::setValue(float val) {
  this->value = val;
  // SendMessage(hwnd, PBM_SETPOS, (WPARAM)value, 0);
}
float ProgressBar::getValue() { return this->value; }
void ProgressBar::onPaint(PaintEvent pe) {
  Canvas &canvas = getCanvas();
  canvas.beginDraw();
  // reflectDebug("paint!");
  canvas.noStroke();

  canvas.fill(bgColor);
  canvas.rect({0, 0}, size, 3.0f);
  canvas.fill(fgColor);
  canvas.rect({0, 0}, {static_cast<int>((value / maxValue) * size.x), size.y},
              3.0f);
  canvas.endDraw();
}
void ProgressBar::setMaximumValue(float val) {
  this->maxValue = val;
  // SendMessage(hwnd, PBM_SETRANGE32, (WPARAM)minValue, (LPARAM)maxValue);
}
void ProgressBar::setMininumValue(float val) {
  this->minValue = val;
  // SendMessage(hwnd, PBM_SETRANGE32, (WPARAM)minValue, (LPARAM)maxValue);
}
float ProgressBar::getMaximumValue() { return this->maxValue; }
float ProgressBar::getMinimumValue() { return this->minValue; }
} // namespace reflect