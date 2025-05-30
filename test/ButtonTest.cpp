#include "../core/memory/HeapPool.h"
#include "../core/ui/Button.h"
#include "../core/ui/Window.h"
#include <Windows.h>
#include <chrono>
#include <gtest/gtest.h>
#include <thread>

namespace {

class ButtonTest : public ::testing::Test {
protected:
  reflect::Button *button;

  void SetUp() override {
    reflect::initializeHeapPool(4 * 1024 * 1024);
    button = new reflect::Button("Hello", {10, 20});
  }

  void TearDown() override { delete button; }
};

TEST_F(ButtonTest, ConstructorSetsTextAndPosition) {
  EXPECT_EQ(button->getText(), "Hello");
  EXPECT_EQ(button->getPosition().x, 10);
  EXPECT_EQ(button->getPosition().y, 20);
}

TEST_F(ButtonTest, SetTextChangesText) {
  button->setText("World");
  EXPECT_EQ(button->getText(), "World");
}

TEST_F(ButtonTest, SetFontAndFontSize) {
  button->setFont("Arial");
  button->setFontSize(18);
  // No direct getter, but should not crash
}

TEST_F(ButtonTest, SetAndGetColor) {
  button->setColor(100, 150, 200);
  auto color = button->getColor();
  EXPECT_EQ(color.r, 100);
  EXPECT_EQ(color.g, 150);
  EXPECT_EQ(color.b, 200);
}

TEST_F(ButtonTest, SetAndGetPosition) {
  button->setPosition({30, 40});
  auto pos = button->getPosition();
  EXPECT_EQ(pos.x, 30);
  EXPECT_EQ(pos.y, 40);
}

TEST_F(ButtonTest, SetAndGetSize) {
  button->setSize({120, 30});
  auto size = button->getSize();
  EXPECT_EQ(size.x, 120);
  EXPECT_EQ(size.y, 30);
}

TEST_F(ButtonTest, SetAndGetVisibility) {
  button->setVisible(true);
  EXPECT_FALSE(button->getVisible());
  button->setVisible(false);
  EXPECT_FALSE(button->getVisible());
}

// Instrumental: Render Button in a window and check for crash
TEST(ButtonInstrumental, RenderButtonInWindow) {
  HINSTANCE instance = GetModuleHandle(nullptr);
  reflect::Window window(instance, "Buttontest");
  reflect::Button Button("Instrumental", {50, 50});
  window.add(Button, "Button");
  window.setVisible(true);
  // Run the window for a short time, then close

  int result = window.run([](reflect::Window &win) { win.close(); });
  EXPECT_EQ(result, 0);
}

} // namespace
