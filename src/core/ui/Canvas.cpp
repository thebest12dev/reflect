#include "Canvas.h"
#include "Console.h"
#include "Window.h"
#include <numbers>
namespace reflect {
void Canvas::arc(Vector2 position, Vector2 size, Vector2Float32 startStop) {
  // Center of the ellipse
  D2D1_POINT_2F centerPoint = D2D1::Point2F(position.x, position.y);
  D2D1_SIZE_F radius =
      D2D1::SizeF(size.x / 2.0f, size.y / 2.0f); // Half-width and half-height

  auto normalizeDegrees = [](float deg) {
    while (deg < 0)
      deg += 360.0f;
    while (deg >= 360.0f)
      deg -= 360.0f;
    return deg;
  };

  //// Clamp sweep to [-360, 360]

  float startAngle = startStop.x;
  float sweepAngle = startStop.y - startStop.x;

  D2D1_POINT_2F startPoint =
      D2D1::Point2F(centerPoint.x + radius.width * cos(startAngle),
                    centerPoint.y + radius.height * sin(startAngle));

  D2D1_POINT_2F endPoint = D2D1::Point2F(
      centerPoint.x + radius.width * cos(startAngle + sweepAngle),
      centerPoint.y + radius.height * sin(startAngle + sweepAngle));

  // Set rotation angle (0 for no rotation)
  FLOAT rotationAngle = 0.0f;

  // Sweep direction (counterclockwise for positive angles)
  D2D1_SWEEP_DIRECTION sweepDirection =
      (sweepAngle >= 0.0f) ? D2D1_SWEEP_DIRECTION_CLOCKWISE
                           : D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE;

  // Arc size (small arc if sweep < 180°, large arc if sweep > 180°)
  D2D1_ARC_SIZE arcSize = (std::fabs(sweepAngle) > 3.14159f)
                              ? D2D1_ARC_SIZE_LARGE
                              : D2D1_ARC_SIZE_SMALL;

  // D2D1_ARC_SIZE arcSize = D2D1_ARC_SIZE_SMALL;
  //   Create the arc segment
  D2D1_ARC_SEGMENT arcSegment = {
      endPoint,       // End point
      radius,         // Radius
      0.0f,           // Rotation angle (0 here)
      sweepDirection, // Sweep direction
      arcSize         // Arc size
  };

  // Create the path geometry for the arc
  ID2D1PathGeometry *pPathGeometry = nullptr;
  ID2D1GeometrySink *pSink = nullptr;

  factory->CreatePathGeometry(&pPathGeometry);
  pPathGeometry->Open(&pSink);

  pSink->BeginFigure(startPoint, D2D1_FIGURE_BEGIN_FILLED);
  pSink->AddArc(arcSegment);
  pSink->EndFigure(D2D1_FIGURE_END_OPEN);
  pSink->Close();

  // Draw the arc
  childRenderTarget->DrawGeometry(pPathGeometry, strokeBrush, strokeWidth);

  // Clean up
  pPathGeometry->Release();
  canvasPainted = false;
}
Canvas::Canvas(Component *thisObject) {
  // we will take the object's hwnd

  HWND injectedHWND = thisObject->hwnd;
  this->hwnd = injectedHWND;
}
Canvas::Canvas() {}
void Canvas::point(Vector2 point) {
  // drawingCommands.push_back([point, this](ID2D1HwndRenderTarget
  // *renderTarget) {
  childRenderTarget->FillEllipse(
      D2D1::Ellipse(D2D1::Point2F(point.x, point.y), 2.0f, 2.0f), strokeBrush);
  // });
  canvasPainted = false;
}
void Canvas::repaint() { InvalidateRect(hwnd, nullptr, true); }
LRESULT CALLBACK Canvas::canvasProc(HWND hwnd, UINT uMsg, WPARAM wParam,
                                    LPARAM lParam) {
  Canvas *pThis =
      reinterpret_cast<Canvas *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
  if (uMsg == WM_CREATE) {
    CREATESTRUCT *pCreate = reinterpret_cast<CREATESTRUCT *>(lParam);
    pThis = reinterpret_cast<Canvas *>(pCreate->lpCreateParams);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    pThis->childRenderTarget = nullptr;
    pThis->fillBrush = nullptr;
    pThis->strokeBrush = nullptr;
    pThis->factory =
        reinterpret_cast<Window *>(
            GetWindowLongPtr(GetAncestor(hwnd, GA_ROOT), GWLP_USERDATA))
            ->getProperty<ID2D1Factory *>("direct2DFactory");
    if (pThis->factory) {
      RECT rc;
      GetClientRect(hwnd, &rc);
      D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties();
      D2D1_HWND_RENDER_TARGET_PROPERTIES hwndRTProps =
          D2D1::HwndRenderTargetProperties(
              hwnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top));
      HRESULT hr = pThis->factory->CreateHwndRenderTarget(
          rtProps, hwndRTProps, &(pThis->childRenderTarget));
      if (FAILED(hr)) {
        // Handle error
        return -1;
      }
      pThis->childRenderTarget->CreateSolidColorBrush(
          D2D1::ColorF(D2D1::ColorF::Black), &pThis->strokeBrush);
      pThis->childRenderTarget->CreateSolidColorBrush(
          D2D1::ColorF(D2D1::ColorF::Black), &pThis->fillBrush);
    }
  }
  if (pThis) {
    switch (uMsg) {

    case WM_PAINT: {
      PAINTSTRUCT p;
      BeginPaint(hwnd, &p);

      // if (!pThis->canvasPainted) {

      pThis->childRenderTarget->BeginDraw();
      for (std::function<void(ID2D1HwndRenderTarget *)> command :
           pThis->drawingCommands) {
        command(pThis->childRenderTarget);
      }
      pThis->childRenderTarget->EndDraw();
      //  pThis->canvasPainted = true;
      // }
      EndPaint(hwnd, &p);
      return 0;
    }
    }
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
void Canvas::fill(Color3 color, unsigned char alpha) {
  // drawingCommands.push_back([this, color, alpha](ID2D1HwndRenderTarget *) {
  if (fillBrush) {
    fillBrush->Release();
  }
  childRenderTarget->CreateSolidColorBrush(
      {color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 1.0f}, &fillBrush);
  shouldFill = true;
  //});
  canvasPainted = false;
}
void Canvas::stroke(Color3 color, unsigned char alpha) {
  // drawingCommands.push_back([this, color, alpha](ID2D1HwndRenderTarget *) {
  if (strokeBrush) {
    strokeBrush->Release();
  }
  childRenderTarget->CreateSolidColorBrush(
      {color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 1.0f},
      &strokeBrush);
  shouldStroke = true;
  // });
  canvasPainted = false;
}
void Canvas::strokeWeight(float weight) {
  // drawingCommands.push_back(
  //[this, weight](ID2D1HwndRenderTarget *) {
  strokeWidth = weight;
  //});
  canvasPainted = false;
}
void Canvas::noFill() {
  // drawingCommands.push_back(
  //   [this](ID2D1HwndRenderTarget *) {
  shouldFill = false;
  //});
  canvasPainted = false;
}
void Canvas::noStroke() {
  // drawingCommands.push_back(
  //[this](ID2D1HwndRenderTarget *) {
  shouldStroke = false;
  // });
  canvasPainted = false;
}
void Canvas::background(Color3 color, unsigned char alpha) {
  bgColor = color;
  // drawingCommands.push_back([color, alpha, this](ID2D1HwndRenderTarget *) {
  if (isDrawing) {
    childRenderTarget->Clear({bgColor.r, bgColor.g, bgColor.b, alpha / 255.0f});
  }

  //});
  canvasPainted = false;
}
void Canvas::beginDraw() {

  childRenderTarget->BeginDraw();
  isDrawing = true;
}
void Canvas::endDraw() {
  childRenderTarget->EndDraw();
  isDrawing = false;
}
void Canvas::triangle(Vector2 first, Vector2 second, Vector2 third) {

  if (isDrawing) {
    ID2D1PathGeometry *pTriangleGeometry = nullptr;
    factory->CreatePathGeometry(&pTriangleGeometry);

    // Define the triangle geometry
    ID2D1GeometrySink *pSink = nullptr;
    pTriangleGeometry->Open(&pSink);
    // Define the triangle points
    D2D1_POINT_2F points[] = {
        D2D1::Point2F(first.x, first.y),   // Vertex 1
        D2D1::Point2F(second.x, second.y), // Vertex 2
        D2D1::Point2F(third.x, third.y)    // Vertex 3
    };

    // Start the path at the first vertex
    pSink->BeginFigure(points[0], D2D1_FIGURE_BEGIN_FILLED);

    // Add lines to the next vertices
    pSink->AddLine(points[1]);
    pSink->AddLine(points[2]);

    // Close the path to form the triangle
    pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

    // Close the geometry sink
    pSink->Close();
    pSink->Release();
    if (shouldStroke) {

      childRenderTarget->DrawGeometry(pTriangleGeometry, strokeBrush,
                                      strokeWidth);
    }
    if (shouldFill) {
      childRenderTarget->FillGeometry(pTriangleGeometry, fillBrush);
    }
  }
  canvasPainted = false;
}
void Canvas::line(Vector2 start, Vector2 end) {

  // drawingCommands.push_back([start, end, this](ID2D1HwndRenderTarget *) {
  if (isDrawing) {
    // Define the start and end points of the line
    D2D1_POINT_2F startPoint =
        D2D1::Point2F(start.x, start.y); // Start point (x1, y1)
    D2D1_POINT_2F endPoint = D2D1::Point2F(end.x, end.y); // End point (x2, y2)

    // Draw the line
    childRenderTarget->DrawLine(startPoint, endPoint, strokeBrush,
                                strokeWidth); // 2.0f is the line width
  }
  //});
  canvasPainted = false;
}
void Canvas::quad(Vector2 first, Vector2 second, Vector2 third,
                  Vector2 fourth) {
  if (isDrawing) {
    // Assume you already have ID2D1Factory* pFactory and ID2D1RenderTarget*
    // pRenderTarget

    // Step 1: Create a path geometry
    ID2D1PathGeometry *pPathGeometry = nullptr;
    factory->CreatePathGeometry(&pPathGeometry);

    // Step 2: Open the geometry for drawing
    ID2D1GeometrySink *pSink = nullptr;
    pPathGeometry->Open(&pSink);

    // Step 3: Define the four corners of the quad
    D2D1_POINT_2F p1 = D2D1::Point2F(first.x, first.y);
    D2D1_POINT_2F p2 = D2D1::Point2F(second.x, second.y);
    D2D1_POINT_2F p3 = D2D1::Point2F(third.x, third.y);
    D2D1_POINT_2F p4 = D2D1::Point2F(fourth.x, fourth.y);

    // Step 4: Draw the shape
    pSink->BeginFigure(p1, D2D1_FIGURE_BEGIN_FILLED);
    pSink->AddLine(p2);
    pSink->AddLine(p3);
    pSink->AddLine(p4);
    pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

    pSink->Close();
    pSink->Release();

    pPathGeometry->Release();
  }
}
void Canvas::rect(Vector2 position, Vector2 size, float radius) {
  // drawingCommands.push_back(
  //   [this, position, size, radius](ID2D1HwndRenderTarget *) {
  if (isDrawing) {
    if (shouldStroke) {
      if (radius > 0.0f) {
        this->childRenderTarget->DrawRoundedRectangle(
            D2D1::RoundedRect(D2D1_RECT_F(position.x, position.y,
                                          position.x + size.x,
                                          position.y + size.y),
                              radius, radius),
            strokeBrush, strokeWidth);
      } else {
        this->childRenderTarget->DrawRectangle(
            D2D1_RECT_F(position.x, position.y, position.x + size.x,
                        position.y + size.y),

            strokeBrush, strokeWidth);
      }
    }
    if (shouldFill) {
      if (radius > 0.0f) {
        this->childRenderTarget->FillRoundedRectangle(
            D2D1::RoundedRect(D2D1_RECT_F(position.x, position.y,
                                          position.x + size.x,
                                          position.y + size.y),
                              radius, radius),
            fillBrush);
      } else {
        this->childRenderTarget->FillRectangle(
            D2D1_RECT_F(position.x, position.y, position.x + size.x,
                        position.y + size.y),

            fillBrush);
      }
    }
  }

  // });
}
void Canvas::clear() {
  // drawingCommands.clear();
  // drawingCommands.push_back([this](ID2D1HwndRenderTarget *) {
  if (isDrawing) {
    childRenderTarget->Clear({bgColor.r, bgColor.g, bgColor.b, 1.0f});
  }

  //});
  canvasPainted = false;
}
void Canvas::render(HWND &parentHWND, HWND &windowHWND) {

  initializeObject(REFLECT_OBJECT_CANVAS, REFLECT_OBJECT_COMPONENT);
  factory =
      reinterpret_cast<Window *>(GetWindowLongPtr(windowHWND, GWLP_USERDATA))
          ->getProperty<ID2D1Factory *>("direct2DFactory");
  if (this->hwnd) {
    RECT rc;
    GetClientRect(hwnd, &rc);
    D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties();
    D2D1_HWND_RENDER_TARGET_PROPERTIES hwndRTProps =
        D2D1::HwndRenderTargetProperties(
            hwnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top));
    HRESULT hr = factory->CreateHwndRenderTarget(rtProps, hwndRTProps,
                                                 &(childRenderTarget));

    childRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black),
                                             &strokeBrush);
    childRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black),
                                             &fillBrush);
  } else if (!this->hwnd) {
    WNDCLASS wc = {};
    wc.lpszClassName = "ReflectCanvas";
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpfnWndProc = canvasProc;
    RegisterClass(&wc);
    hwnd = CreateWindowEx(
        0, "ReflectCanvas", nullptr,
        WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, position.x,
        position.y + reinterpret_cast<Window *>(
                         GetWindowLongPtr(windowHWND, GWLP_USERDATA))
                         ->getProperty<int>("customTitleBarSize"),
        size.x, size.y, parentHWND, nullptr, GetModuleHandle(nullptr), this);
  }
}
} // namespace reflect