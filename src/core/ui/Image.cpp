#include "Image.h"
#include "Window.h"
#include <wincodec.h>
#pragma comment(lib, "windowscodecs")

namespace reflect {
Image::Image() {
  initializeObject(REFLECT_OBJECT_IMAGE, REFLECT_OBJECT_COMPONENT);
}
void Image::setImageLocation(std::string image, std::string id) {
  this->imageIds[id] = image;
}
std::string Image::getActiveImage() { return this->activeImage; };
void Image::setActiveImage(std::string id) {
  this->activeImage = id;
  return;
}
void Image::registerClass() {
  WNDCLASS wc = {};
  wc.lpszClassName = "D2D1BitmapImage";
  wc.hInstance = GetModuleHandle(nullptr);
  wc.lpfnWndProc = imageProc;
  RegisterClass(&wc);
}

std::string wideToAnsi(const std::wstring &wstr) {
  if (wstr.empty())
    return std::string();

  // Get the required size for the buffer
  int size_needed =
      WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), (int)wstr.length(), nullptr,
                          0, nullptr, nullptr);

  std::string result(size_needed, 0);
  WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), (int)wstr.length(), &result[0],
                      size_needed, nullptr, nullptr);

  return result;
}

void Image::getBitmap(std::wstring imagePath, ID2D1RenderTarget *pRenderTarget,
                      ID2D1Bitmap **bitmap) {
  IWICImagingFactory2 *pWICFactory = nullptr;
  HRESULT hr =
      CoCreateInstance(CLSID_WICImagingFactory2, nullptr, CLSCTX_INPROC_SERVER,
                       IID_PPV_ARGS(&pWICFactory));
  if (FAILED(hr)) {
    std::wcerr << L"Failed to create WIC imaging factory." << std::endl;
    return;
  }

  IWICBitmapDecoder *pDecoder = nullptr;

  if (imagePath.rfind(L"resources://", 0) == 0) {
    // --- Handle embedded resource ---
    std::wstring resourceName = imagePath.substr(12); // Strip "resources://"
    HMODULE hModule = GetModuleHandle(nullptr);
    HRSRC hRes =
        FindResourceA(hModule, wideToAnsi(resourceName).c_str(), RT_RCDATA);
    if (!hRes) {
      std::wcerr << L"Failed to find resource: " << resourceName << std::endl;
      pWICFactory->Release();
      return;
    }

    HGLOBAL hData = LoadResource(hModule, hRes);
    if (!hData) {
      std::wcerr << L"Failed to load resource: " << resourceName << std::endl;
      pWICFactory->Release();
      return;
    }

    void *pResourceData = LockResource(hData);
    DWORD resourceSize = SizeofResource(hModule, hRes);
    if (!pResourceData || resourceSize == 0) {
      std::wcerr << L"Failed to lock or get size of resource." << std::endl;
      pWICFactory->Release();
      return;
    }

    IWICStream *pStream = nullptr;
    hr = pWICFactory->CreateStream(&pStream);
    if (FAILED(hr)) {
      std::wcerr << L"Failed to create WIC stream." << std::endl;
      pWICFactory->Release();
      return;
    }

    hr = pStream->InitializeFromMemory(reinterpret_cast<BYTE *>(pResourceData),
                                       resourceSize);
    if (FAILED(hr)) {
      std::wcerr << L"Failed to initialize stream from memory." << std::endl;
      pStream->Release();
      pWICFactory->Release();
      return;
    }

    hr = pWICFactory->CreateDecoderFromStream(
        pStream, nullptr, WICDecodeMetadataCacheOnLoad, &pDecoder);
    pStream->Release();

    if (FAILED(hr)) {
      std::wcerr << L"Failed to create decoder from stream." << std::endl;
      pWICFactory->Release();
      return;
    }

  } else {
    // --- Handle file path ---
    hr = pWICFactory->CreateDecoderFromFilename(
        imagePath.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad,
        &pDecoder);
    if (FAILED(hr)) {
      std::wcerr << L"Failed to load image from file." << std::endl;
      pWICFactory->Release();
      return;
    }
  }

  IWICBitmapFrameDecode *pFrame = nullptr;
  hr = pDecoder->GetFrame(0, &pFrame);
  if (FAILED(hr)) {
    std::wcerr << L"Failed to get image frame." << std::endl;
    pDecoder->Release();
    pWICFactory->Release();
    return;
  }

  IWICFormatConverter *pConverter = nullptr;
  hr = pWICFactory->CreateFormatConverter(&pConverter);
  if (FAILED(hr)) {
    std::wcerr << L"Failed to create format converter." << std::endl;
    pFrame->Release();
    pDecoder->Release();
    pWICFactory->Release();
    return;
  }

  hr = pConverter->Initialize(pFrame, GUID_WICPixelFormat32bppPBGRA,
                              WICBitmapDitherTypeNone, nullptr, 0.0,
                              WICBitmapPaletteTypeCustom);
  if (FAILED(hr)) {
    std::wcerr << L"Failed to initialize format converter." << std::endl;
    pConverter->Release();
    pFrame->Release();
    pDecoder->Release();
    pWICFactory->Release();
    return;
  }

  hr = pRenderTarget->CreateBitmapFromWicBitmap(pConverter, nullptr, bitmap);
  if (FAILED(hr)) {
    std::wcerr << L"Failed to create bitmap from WIC converter." << std::endl;
  }

  // Cleanup
  pConverter->Release();
  pFrame->Release();
  pDecoder->Release();
  pWICFactory->Release();
}
LRESULT CALLBACK Image::imageProc(WindowHandle hwnd, UINT uMsg, WPARAM wParam,
                                  LPARAM lParam) {
  reflect::Image *pThis = nullptr;

  if (uMsg == WM_CREATE) {
    CREATESTRUCT *pCreate = reinterpret_cast<CREATESTRUCT *>(lParam);
    pThis = reinterpret_cast<reflect::Image *>(pCreate->lpCreateParams);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    reflect::Window *win = reinterpret_cast<reflect::Window *>(
        GetWindowLongPtr(GetParent(hwnd), GWLP_USERDATA));

    RECT rc;

    GetClientRect(hwnd, &rc);
    // Initialize Direct2D
    ID2D1Factory *pFactory =
        win->getProperty<ID2D1Factory *>("direct2DFactory");
    D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties();
    D2D1_HWND_RENDER_TARGET_PROPERTIES hwndRTProps =
        D2D1::HwndRenderTargetProperties(
            hwnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top));

    HRESULT hr = pFactory->CreateHwndRenderTarget(
        rtProps, hwndRTProps,
        &(pThis->childRenderTarget)); // <--- store it on pThis

  } else {
    pThis = reinterpret_cast<reflect::Image *>(
        GetWindowLongPtr(hwnd, GWLP_USERDATA));
  }
  reflect::Window *win = reinterpret_cast<reflect::Window *>(
      GetWindowLongPtr(GetParent(hwnd), GWLP_USERDATA));

  ID2D1Factory *pFactory = win->getProperty<ID2D1Factory *>("direct2DFactory");
  if (pThis) {

    // Delegate the handling of messages to the instance

    switch (uMsg) {

    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hwnd, &ps);
      pThis->childRenderTarget->BeginDraw();
      ImageBitmap *bitmap = pThis->bitmaps[pThis->activeImage];
      std::string &path = pThis->imageIds[pThis->activeImage];
      if (!pThis->bitmaps[pThis->activeImage]) {
        int sizeNeeded = MultiByteToWideChar(
            CP_UTF8, 0, pThis->imageIds[pThis->activeImage].c_str(), -1,
            nullptr, 0);
        std::wstring wstr(sizeNeeded - 1, 0);
        MultiByteToWideChar(CP_UTF8, 0, path.c_str(), -1, &wstr[0], sizeNeeded);

        getBitmap(wstr, pThis->childRenderTarget, &(bitmap));
      }
      D2D1_RECT_F rect = {0, 0, pThis->size.x, pThis->size.y};

      ID2D1SolidColorBrush *pBrush = nullptr;
      HRESULT hr = pThis->childRenderTarget->CreateSolidColorBrush(
          D2D1::ColorF(pThis->bgColor.r, pThis->bgColor.g, pThis->bgColor.b),
          &pBrush);

      pThis->childRenderTarget->FillRectangle(rect, pBrush);
      pThis->childRenderTarget->DrawBitmap(bitmap, rect);
      pThis->childRenderTarget->EndDraw();
      EndPaint(hwnd, &ps);

      return 0;
    }
    }
  }

  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void Image::render(WindowHandle &parentHWND, WindowHandle &windowHWND) {
  registerClass();

  hwnd = CreateWindowEx(0, "D2D1BitmapImage", nullptr, WS_CHILD | WS_VISIBLE,
                        position.x, position.y, size.x, size.y, parentHWND,
                        nullptr, GetModuleHandle(nullptr), this);
};
} // namespace reflect