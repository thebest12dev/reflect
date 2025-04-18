#include "ProgressBar.h"
#include <CommCtrl.h>
#include <Windows.h>
namespace CinnamonToast {
ProgressBar::ProgressBar()
    : winstance(GetModuleHandle(nullptr)), hwnd(nullptr), maxValue(100.0f),
      minValue(0.0f), value(0.0f) {
  this->size = {0, 0};
  this->position = {0, 0};
}
void ProgressBar::setPosition(Vector2 pos) {
  position = pos;
  // Set the position of the progress bar
  SetWindowPos(hwnd, NULL, position.x, position.y, 0, 0,
               SWP_NOSIZE | SWP_NOZORDER);
}
void ProgressBar::render(HWND &parentHWND, HWND &windowHWND) {
  hwnd = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE,
                        position.x, position.y, size.x, size.y, parentHWND,
                        NULL, winstance, NULL);
  SendMessage(hwnd, PBM_SETRANGE32, (WPARAM)minValue, (LPARAM)maxValue);
  SendMessage(hwnd, PBM_SETPOS, (WPARAM)value, 0);
}
void ProgressBar::setValue(float val) {
  this->value = val;
  SendMessage(hwnd, PBM_SETPOS, (WPARAM)value, 0);
}
float ProgressBar::getValue() { return this->value; }
void ProgressBar::setMaximumValue(float val) {
  this->maxValue = val;
  SendMessage(hwnd, PBM_SETRANGE32, (WPARAM)minValue, (LPARAM)maxValue);
}
void ProgressBar::setMininumValue(float val) {
  this->minValue = val;
  SendMessage(hwnd, PBM_SETRANGE32, (WPARAM)minValue, (LPARAM)maxValue);
}
float ProgressBar::getMaximumValue() { return this->maxValue; }
float ProgressBar::getMinimumValue() { return this->minValue; }
} // namespace CinnamonToast