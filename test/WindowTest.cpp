#include <Windows.h>
#include <gtest/gtest.h>

#include "../core/Main.h"
#include "../core/ui/Window.h"
#include "../core/ui/Label.h"
namespace LIBC_NAMESPACE {
    void callbackWindow(CinnamonToast::Window& win) {
        win.close();
    }
}


TEST(WindowTest, WindowCreateTestWithComponents) {
    CinnamonToast::Window wnd(GetModuleHandle(nullptr));
    CinnamonToast::Label foo = CinnamonToast::Label("bar", { 50,50 });
    wnd.setVisible(true);
    wnd.setSize({ 100, 100 });
    wnd.setColor({ 255,255,255 });
    wnd.add(foo);
    EXPECT_EQ(wnd.run(CinnamonToast::callbackWindow), 0);

}
