#include "TextField.h"
#include "../Utilities.h"
#include "Canvas.h"
#include "Console.h"
#include "Window.h"
#include <CommCtrl.h>
namespace {
// WNDPROC originalEditProc = nullptr; // Store the original window procedure
} // namespace
namespace reflect {
// LRESULT CALLBACK TextField::editProc(HWND hwnd, UINT msg, WPARAM wParam,
//                                      LPARAM lParam) {
//   TextField *pThis = nullptr;
//   pThis = reinterpret_cast<reflect::TextField *>(
//       GetWindowLongPtr(hwnd, GWLP_USERDATA));
//
//   switch (msg) {
//   case WM_SETFOCUS:
//     pThis->focused = true;
//     if (pThis->focusCallback)
//       pThis->focusCallback(*pThis);
//     break;
//   case WM_KILLFOCUS:
//     pThis->focused = false;
//     break;
//   }
//   return CallWindowProc(originalEditProc, hwnd, msg, wParam, lParam);
// }

TextField::TextField() : focusCallback(nullptr), focused(false) {
  initializeObject(REFLECT_OBJECT_TEXTFIELD, REFLECT_OBJECT_TEXTCOMPONENT);
  this->setClassName("TextField");
  this->setStyles(WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
}
// void TextField::setSize(Vector2 size) { this->size = size; };
// void TextField::setPosition(Vector2 pos) { position = pos; };
//  void TextField::render(HWND &parentHWND, HWND &windowHWND) {
//    reflect::Window *window = reinterpret_cast<reflect::Window *>(
//        GetWindowLongPtr(windowHWND, GWLP_USERDATA));
//    hwnd = CreateWindowEx(
//        WS_EX_CLIENTEDGE, // Extended styles
//        WC_EDIT,          // Class name
//        "",               // Initial text
//        WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, position.x,
//        position.y + window->getProperty<int>("customTitleBarSize"), size.x,
//        size.y,
//
//        parentHWND, // Parent window
//        NULL,
//        winstance, // App instance
//        this       // Additional data
//    );
//    originalEditProc =
//        (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)editProc);
//    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
//    SendMessage(hwnd, WM_SETFONT, (WPARAM)reflect::utilities::getFont(),
//    TRUE);
//  }
// void TextField::setText(std::string text) {
//  this->text = text;
//  SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)text.c_str());
//}
// std::string TextField::getText() {
//  char buffer[256];
//  SendMessage(hwnd, WM_GETTEXT, sizeof(buffer), (LPARAM)buffer);
//  return std::string(buffer);
//}

bool TextField::isFocused() { return focused; }
void TextField::focus() {
  SetFocus(hwnd);
  focused = true;
};
void TextField::onFocus(void (*callback)(TextField &)) {
  focusCallback = callback;
}

void TextField::onPaint(PaintEvent) {

  Canvas &canvas = getCanvas();
  canvas.beginDraw();
  // reflectDebug("paint!");
  canvas.noStroke();

  canvas.fill({0, 0, 0});
  canvas.rect({0, 0}, size);
  canvas.fill(bgColor);
  canvas.rect({0, 0}, size, 3.0f);
  canvas.textFont("Inter");
  float textSize = canvas.textMaximumFontSize(size.y);
  canvas.textSize(textSize);

  canvas.stroke({255, 255, 255});
  canvas.text(text, {0, 0}, size);

  // Calculate cursor position
  float cursorX = 0.0f, cursorY = 0.0f;
  float ascent = canvas.textAscent();
  float descent = canvas.textDescent();
  float lineHeight = ascent + descent;

  // Split text into lines by '\n'
  std::vector<std::string> lines;
  size_t start = 0, end;
  while ((end = text.find('\n', start)) != std::string::npos) {
    lines.push_back(text.substr(start, end - start));
    start = end + 1;
  }
  lines.push_back(text.substr(start));

  //// For single-line, cursor at end of text
  // if (lines.size() == 1) {
  //   cursorX = canvas.textWidth(lines[0]);
  //   cursorY = ascent;
  // } else {
  //   // For multi-line, cursor at end of last line
  //   cursorX = canvas.textWidth(lines.back());
  //   cursorY = lineHeight * (lines.size() - 1) + ascent;
  // }
  //   Determine cursor line and column
  size_t currentLine = 0;
  size_t charsCounted = 0;
  for (size_t i = 0; i < lines.size(); ++i) {
    if (cursorIndex <= charsCounted + lines[i].size()) {
      currentLine = i;
      break;
    }
    charsCounted += lines[i].size() + 1; // +1 for '\n'
  }

  // Get column position in the line
  size_t columnIndex = cursorIndex - charsCounted;
  std::string cursorLine = lines[currentLine].substr(0, columnIndex);

  // Calculate pixel position
  cursorX = canvas.textWidth(cursorLine);
  cursorY = lineHeight * currentLine + ascent;

  // Draw the cursor (vertical line)
  if (cursorBlinking) {
    canvas.strokeWeight(1.0f);
    float x = cursorX + 0.5f;
    float y1 = cursorY - ascent;
    float y2 = cursorY + descent;

    canvas.strokeWeight(1.0f);
    canvas.line({static_cast<int>(x), static_cast<int>(y1)},
                {static_cast<int>(x), static_cast<int>(y2)});
  }

  /*canvas.line({static_cast<int>(cursorX), static_cast<int>(cursorY -
  ascent)},
              {static_cast<int>(cursorX), static_cast<int>(cursorY +
     descent)});*/

  canvas.endDraw();
}

// void TextField::onKeyPressed(KeyboardEvent k) {
//   start = std::chrono::steady_clock::now();
//   cursorBlinking = true;
//   if (k.key == '\b') {
//     if (text.size() > 0) {
//       text.pop_back();
//     }
//
//   } else if (k.key == '\r') {
//     text += "\n";
//   } else {
//     text += k.key;
//   }
//
//   paint();
// }
void TextField::onKeyPressed(KeyboardEvent k) {
  start = std::chrono::steady_clock::now();
  cursorBlinking = true;

  switch (k.keyCode) {
  case VK_LEFT:
    if (cursorIndex > 0)
      cursorIndex--;
    break;

  case VK_RIGHT:
    if (cursorIndex < text.size())
      cursorIndex++;
    break;
  }
  switch (k.key) {

  case '\b': // Backspace
    if (cursorIndex > 0 && !text.empty()) {
      text.erase(cursorIndex - 1, 1);
      cursorIndex--;
    }
    break;

  case '\r': // Enter
    text.insert(cursorIndex, "\n");
    cursorIndex++;
    break;

  default:
    if (isprint(k.key)) {
      text.insert(cursorIndex, 1, k.key);
      cursorIndex++;
    }
    break;
  }

  paint();
}

void TextField::onUpdate(UpdateEvent) {
  std::chrono::steady_clock::time_point current =
      std::chrono::steady_clock::now();

  std::chrono::duration<double> delay = current - start;
  if (delay.count() > 0.5) {
    start = std::chrono::steady_clock::now();
    cursorBlinking = !cursorBlinking;
    paint();
  }
  // paint();
}
} // namespace reflect