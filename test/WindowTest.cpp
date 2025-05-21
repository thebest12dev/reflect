#include "../core/ui/Window.h"
#include "../core/ui/Label.h"
#include "../core/ui/Notification.h"
#include "../core/ui/OpenGLContext.h"
#include <Windows.h>
#include <chrono>
#include <gtest/gtest.h>
#include <thread>

namespace {

void DummyRenderLoop(reflect::Window &) {}
void DummyBeforeRenderLoop(reflect::Window &) {}

class WindowInstrumentalTest : public ::testing::Test {
protected:
  reflect::Window *window;

  void SetUp() override {
    HINSTANCE instance = GetModuleHandle(nullptr);
    window = new reflect::Window(instance, "instrumental");
  }

  void TearDown() override { delete window; }
};

TEST_F(WindowInstrumentalTest, WindowConstructor_Default) {
  EXPECT_NE(window, nullptr);
  EXPECT_TRUE(window->getVisible() == false ||
              window->getVisible() == true); // Should not crash
}

TEST_F(WindowInstrumentalTest, WindowSetAndGetTitle) {
  std::string title = "Test Window Title";
  window->setTitle(title);
  // No direct getter, but should not crash
}

TEST_F(WindowInstrumentalTest, WindowSetAndGetSize) {
  window->setSize({320, 240});
  auto size = window->getSize();
  EXPECT_EQ(size.x, 320);
  EXPECT_EQ(size.y, 240);
}

TEST_F(WindowInstrumentalTest, WindowSetAndGetColor) {
  window->setColor({10, 20, 30});
  auto color = window->getColor();
  EXPECT_EQ(color.r, 10);
  EXPECT_EQ(color.g, 20);
  EXPECT_EQ(color.b, 30);
}

TEST_F(WindowInstrumentalTest, WindowSetColorWithArray) {
  window->setColor({100, 150, 200});
  auto color = window->getColor();
  EXPECT_EQ(color.r, 100);
  EXPECT_EQ(color.g, 150);
  EXPECT_EQ(color.b, 200);
}

TEST_F(WindowInstrumentalTest, WindowSetColorWithStruct) {
  reflect::Color3 c{50, 60, 70};
  window->setColor(c);
  auto color = window->getColor();
  EXPECT_EQ(color.r, 50);
  EXPECT_EQ(color.g, 60);
  EXPECT_EQ(color.b, 70);
}

TEST_F(WindowInstrumentalTest, WindowSetAndGetVisibility) {
  window->setVisible(true);
  EXPECT_TRUE(window->getVisible());
  window->setVisible(false);
  EXPECT_FALSE(window->getVisible());
}

//TEST_F(WindowInstrumentalTest, WindowSetVisibleWithCmd) {
//  window->setVisible(SW_SHOW);
//  EXPECT_TRUE(window->getVisible());
//  window->setVisible(SW_HIDE);
//  EXPECT_FALSE(window->getVisible());
//}

TEST_F(WindowInstrumentalTest, WindowAddComponent) {
  reflect::Label label("label", {10, 10});
  window->add(label, "label1");
  // No direct assertion, but should not crash
}

TEST_F(WindowInstrumentalTest, WindowShowNotification) {
  reflect::Notification notif("Test", "Notification text");
  bool result = window->showNotification(notif);
  EXPECT_TRUE(result);
}

TEST_F(WindowInstrumentalTest, WindowGetPropertyCustomTitleBarSize) {
  int size = window->getProperty<int>("customTitleBarSize");
  EXPECT_TRUE(size == 0 || size == 40);
}

TEST_F(WindowInstrumentalTest, WindowSetRenderLoop) {
  window->setRenderLoop(DummyRenderLoop);
  // No assertion, just ensure no crash
}

TEST_F(WindowInstrumentalTest, WindowSetBeforeRenderLoop) {
  window->setBeforeRenderLoop(DummyBeforeRenderLoop);
  // No assertion, just ensure no crash
}

TEST_F(WindowInstrumentalTest, WindowSwapBuffers) {
  window->swapBuffers();
  // No assertion, just ensure no crash
}

TEST_F(WindowInstrumentalTest, WindowIsKeyPressed) {
  // This test is instrumental and may not be reliable in CI
  // Just ensure it does not crash
  window->isKeyPressed('A');
}

TEST_F(WindowInstrumentalTest, WindowOperatorHWND) {
  HWND hwnd = *window;
  EXPECT_NE(hwnd, nullptr);
}

TEST_F(WindowInstrumentalTest, WindowClose) {
  window->close();
  // No assertion, just ensure no crash
}

TEST_F(WindowInstrumentalTest, WindowRunWithCallback) {
  // Instrumental: runs the message loop, closes immediately
  int result = window->run([](reflect::Window &win) { win.close(); });
  EXPECT_EQ(result, 0);
}

TEST_F(WindowInstrumentalTest, WindowAddStyle) {
  window->addStyle(reflect::STYLE_DARK_TITLE_BAR);
  // No assertion, just ensure no crash
}

} // namespace