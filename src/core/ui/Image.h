#pragma once
#include "Component.h"
#include <d2d1.h>
#include <unordered_map>
namespace reflect {
using ImageBitmap = ID2D1Bitmap;
class Image : public Component {
protected:
  std::string activeImage = "default";

  std::unordered_map<std::string, std::string> imageIds;
  std::unordered_map<std::string, ImageBitmap *> bitmaps;
  bool hovered = false;
  ID2D1HwndRenderTarget *childRenderTarget;
  void registerClass();
  static void getBitmap(std::wstring image, ID2D1RenderTarget *pRenderTarget,
                        ImageBitmap **bitmap);
  static LRESULT CALLBACK imageProc(HWND hwnd, UINT uMsg, WPARAM wParam,
                                    LPARAM lParam);

public:
  REFLECT_API Image();
  REFLECT_API void setImageLocation(std::string image,
                                    std::string id = "default");
  REFLECT_API void setActiveImage(std::string id = "default");
  REFLECT_API std::string getActiveImage();
  REFLECT_API void render(HWND &parentHWND, HWND &windowHWND);
};
} // namespace reflect