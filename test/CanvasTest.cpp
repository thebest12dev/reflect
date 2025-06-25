#include "../core/ui/Canvas.h"
#include "../core/ui/OpenGLContext.h"
#include "../core/ui/Window.h"
#include <Windows.h>
#include <chrono>
#include <random>
#include <gtest/gtest.h>
#include <thread>

namespace {
#ifdef __clang__
__attribute__((used))
#endif

void DummyRenderLoop(reflect::Window &) {}
#ifdef __clang__
__attribute__((used))
#endif
#ifdef __clang__
__attribute__((used))
#endif

void DummyBeforeRenderLoop(reflect::Window &) {}

class CanvasInstrumentalTest : public ::testing::Test {
protected:
  reflect::Window *window;
  void SetUp() override {
    HINSTANCE instance = GetModuleHandle(nullptr);
    window = new reflect::Window(instance, "instrumental");
    window->setSize({800, 500});
  }

  void TearDown() override { delete window; }
};

TEST_F(CanvasInstrumentalTest, LineTest) { 
    reflect::Canvas canvas;
  canvas.setSize({800, 500});  
    window->add(canvas);
  window->setBeforeRenderLoop(
      [&canvas](reflect::Window &window) { canvas.beginDraw();
  // random values (fuzzy)
    std::random_device rd;

    // 2. Create a random number generator (Mersenne Twister)
    std::mt19937 gen(rd());
    // 3. Define a distribution: for example, integers between 1 and 100
    std::uniform_int_distribution<> dist(1, 400);

    // 4. Generate a number

      canvas.line({dist(gen), dist(gen)}, {dist(gen), dist(gen)});
      canvas.endDraw();
  });
    window->setVisible(true);
    
  EXPECT_EQ(window->run([](reflect::Window &win) { win.close(); }), 0);
}

} // namespace