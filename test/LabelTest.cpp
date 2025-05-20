#include "../core/ui/Label.h"
#include "../core/ui/Window.h"
#include "../core/memory/HeapPool.h"
#include <Windows.h>
#include <chrono>
#include <gtest/gtest.h>
#include <thread>

namespace {

class LabelTest : public ::testing::Test {
protected:
  reflect::Label *label;

  void SetUp() override {
    reflect::initializeHeapPool(4 * 1024 * 1024);
    label = new reflect::Label("Hello", {10, 20});
  }

  void TearDown() override { delete label; }
};

TEST_F(LabelTest, ConstructorSetsTextAndPosition) {
  EXPECT_EQ(label->getText(), "Hello");
  EXPECT_EQ(label->getPosition().x, 10);
  EXPECT_EQ(label->getPosition().y, 20);
}

TEST_F(LabelTest, SetTextChangesText) {
  label->setText("World");
  EXPECT_EQ(label->getText(), "World");
}

TEST_F(LabelTest, SetFontAndFontSize) {
  label->setFont("Arial");
  label->setFontSize(18);
  // No direct getter, but should not crash
}

TEST_F(LabelTest, SetAndGetColor) {
  label->setColor(100, 150, 200);
  auto color = label->getColor();
  EXPECT_EQ(color.r, 100);
  EXPECT_EQ(color.g, 150);
  EXPECT_EQ(color.b, 200);
}

TEST_F(LabelTest, SetAndGetPosition) {
  label->setPosition({30, 40});
  auto pos = label->getPosition();
  EXPECT_EQ(pos.x, 30);
  EXPECT_EQ(pos.y, 40);
}

TEST_F(LabelTest, SetAndGetSize) {
  label->setSize({120, 30});
  auto size = label->getSize();
  EXPECT_EQ(size.x, 120);
  EXPECT_EQ(size.y, 30);
}

TEST_F(LabelTest, SetAndGetVisibility) {
  label->setVisible(true);
  EXPECT_FALSE(label->getVisible());
  label->setVisible(false);
  EXPECT_FALSE(label->getVisible());
}

// Instrumental: Render label in a window and check for crash
TEST(LabelInstrumental, RenderLabelInWindow) {
  HINSTANCE instance = GetModuleHandle(nullptr);
  reflect::Window window(instance, "labeltest");
  reflect::Label label("Instrumental", {50, 50});
  window.add(label, "label");
  window.setVisible(true);
  // Run the window for a short time, then close
  
  int result = window.run([](reflect::Window &win) { win.close(); });
  EXPECT_EQ(result, 0);
}

} // namespace
