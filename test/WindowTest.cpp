#include "../core/ui/Window.h"
#include "../core/Main.h"
#include "../core/ui/Label.h"
#include <Windows.h>
#include <gtest/gtest.h>

namespace LIBC_NAMESPACE {
void callbackWindow(cinnamontoast::Window &win) { win.close(); }
} // namespace LIBC_NAMESPACE

class WindowTestFixture : public ::testing::Test {
protected:
  cinnamontoast::Window *window;

  void SetUp() override {
    HINSTANCE instance = GetModuleHandle(nullptr);
    window = new cinnamontoast::Window(instance, "win");
  }

  void TearDown() override { delete window; }
};

TEST_F(WindowTestFixture, WindowCreateTestWithComponents) {
  cinnamontoast::Label label("bar", {50, 50});
  window->setVisible(true);
  window->setSize({100, 100});
  window->setSize({100, 100});
  window->setColor({255, 255, 255});
  window->add(label);
  EXPECT_EQ(window->run(LIBC_NAMESPACE::callbackWindow), 0);
}

TEST_F(WindowTestFixture, WindowSetSizeTest) {
  window->setSize({200, 200});
  EXPECT_EQ(window->getSize().x, 200);
  EXPECT_EQ(window->getSize().y, 200);
}

TEST_F(WindowTestFixture, WindowSetColorTest) {
  window->setColor({128, 128, 128});
  auto color = window->getColor();
  EXPECT_EQ(color.r, 128);
  EXPECT_EQ(color.g, 128);
  EXPECT_EQ(color.b, 128);
}
TEST_F(WindowTestFixture, WindowNotificationTest) {
  cinnamontoast::Notification notification("Test title", "Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
  bool result = window->showNotification(notification);
  EXPECT_TRUE(result);
}
TEST_F(WindowTestFixture, WindowVisibilityTest) {
  window->setVisible(true);
  EXPECT_TRUE(window->getVisible());

  window->setVisible(false);
  EXPECT_FALSE(window->getVisible());
}
