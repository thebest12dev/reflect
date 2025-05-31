#pragma once

#include "Component.h"
#include "TypeDefinitions.h"
#include <d2d1.h>
#include <functional>

namespace reflect {
/**
 * @brief A 2D canvas class that serves as an abstraction to the lower level
 * drawing (Direct2D, OpenGL, etc.).
 *
 * It uses a identical API to Processing, but they are NOT the same.
 * This is the intended component to be used for custom drawing areas or for 2D
 * games. Note that this only provides bare methods, you must handle higher
 * level things (sprites, collision, etc.)
 *
 * This is ideal for use in the rendering main loop of the Window class, or with
 * the onPaint method.
 *
 * The Processing methods are simply a lower level but near-equivalent
 * representation of low level graphics APIs. Stick to ComponentOnCanvas as much
 * as you can.
 *
 * The ComponentOnCanvas object allows you to render multiple components on a
 * single canvas.
 *
 * There can be multiple canvases, but only one can be active for the
 * ComponentOnCanvas object.
 *
 * Also, via the getCanvas() method in the Component class, you can access the
 * canvas that every component has.
 */
class Canvas : public Component {
protected:
  ID2D1HwndRenderTarget *childRenderTarget;
  ID2D1SolidColorBrush *fillBrush;
  ID2D1Factory *factory;
  ID2D1SolidColorBrush *strokeBrush;
  std::vector<std::function<void(ID2D1HwndRenderTarget *)>> drawingCommands;

  static LRESULT CALLBACK canvasProc(HWND hwnd, UINT uMsg, WPARAM wParam,
                                     LPARAM lParam);

  bool shouldFill = true;
  bool shouldStroke = false;
  float strokeWidth = 1.0f;
  bool canvasPainted = false;
  bool isDrawing = false;
  bool retainedMode = false;

public:
  static constexpr bool CLOSE = true;
  static constexpr unsigned char ROUND = 0;
  static constexpr unsigned char PROJECT = 1;
  static constexpr unsigned char SQUARE = 2;
  static constexpr unsigned char MITER = 1;
  static constexpr unsigned char BEVEL = 2;
  static constexpr unsigned char CENTER = 0;
  static constexpr unsigned char RADIUS = 1;
  static constexpr unsigned char CORNER = 2;
  static constexpr unsigned char CORNERS = 3;
  static constexpr unsigned char OPEN = 0;
  static constexpr unsigned char CHORD = 1;
  static constexpr unsigned char PIE = 2;
  static constexpr unsigned char LEFT = 1;
  static constexpr unsigned char RIGHT = 2;
  static constexpr unsigned char TOP = 1;
  static constexpr unsigned char BOTTOM = 2;
  static constexpr unsigned char BASELINE = 3;

  REFLECT_API void point(Vector2 point);
  REFLECT_API void line(Vector2 start, Vector2 end);
  REFLECT_API void triangle(Vector2 first, Vector2 second, Vector2 third);
  REFLECT_API void rect(Vector2 position, Vector2 size, float radius = 0);
  REFLECT_API void quad(Vector2 first, Vector2 second, Vector2 third,
                        Vector2 fourth);
  REFLECT_API void ellipse(Vector2 position, Vector2 size);
  REFLECT_API void arc(Vector2 position, Vector2 size,
                       Vector2Float32 startStop);

  REFLECT_API void bezier(Vector2 first, Vector2 second, Vector2 third,
                          Vector2 fourth);
  REFLECT_API void bezierVertex(Vector2 second, Vector2 third, Vector2 fourth);
  REFLECT_API void curve(Vector2 first, Vector2 second, Vector2 third,
                         Vector2 fourth);
  REFLECT_API void curveVertex(Vector2 pos);

  REFLECT_API void vertex(Vector2 pos);
  REFLECT_API void beginShape();
  REFLECT_API void endShape(bool close);
  REFLECT_API void fill(Color3 color, unsigned char alpha = 255);
  REFLECT_API void stroke(Color3 color, unsigned char alpha = 255);
  REFLECT_API void noFill();
  REFLECT_API void noStroke();
  REFLECT_API void strokeWeight(float weight);
  REFLECT_API void strokeCap(unsigned char flag);
  REFLECT_API void strokeJoin(unsigned char flag);
  // color, red, green, blue and alpha methods are not needed; use Color3
  // instead lerpColor is also not needed; use Color3::lerp(Color3 other, float
  // amt).

  REFLECT_API void ellipseMode(unsigned char flag);
  REFLECT_API void rectMode(unsigned char flag);
  REFLECT_API void arcMode(unsigned char flag);
  REFLECT_API void shapeMode(unsigned char flag);
  REFLECT_API void text(const std::string &text, Vector2 pos,
                        Vector2 size = {-32067, -32067});
  REFLECT_API void textFont(const std::string &fontName);
  REFLECT_API void textSize(unsigned char size);
  REFLECT_API void textAlign(unsigned char flag1, unsigned char flag2);
  REFLECT_API void textLeading(float leading);
  REFLECT_API float textWidth(const std::string &text);
  REFLECT_API void textAscent();
  REFLECT_API void textDescent();
  // clear is the same as background, just for naming
  REFLECT_API void background(Color3 color, unsigned char alpha = 255);
  REFLECT_API void clear();
  REFLECT_API void beginDraw();
  REFLECT_API void endDraw();
  REFLECT_API void render(HWND &parentHWND, HWND &windowHWND);
  REFLECT_API void repaint();
  REFLECT_API Canvas();
  REFLECT_API Canvas(Component *thisObject);
  // not done, still plenty of methods to add

  friend class Component;
};
} // namespace reflect