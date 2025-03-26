#include <Windows.h>
#undef byte
typedef unsigned char byte;
#include <gtest/gtest.h>
#include "../core/Main.h"
#include "../core/ui/Window.h"
#include "../core/ui/Label.h"
#include "../core/ui/Label.h"
void callbackWindow(Window& win) {
    win.Close();
}
//TEST(WindowTest, WindowCreateTest) {
//    Window wnd(GetModuleHandle(nullptr));
//    wnd.SetVisible(true);
//    wnd.SetSize({ 100, 100 });
//    wnd.SetColor({ 255,255,255 });
//    EXPECT_EQ(wnd.Run(callbackWindow), 0);
//
//}
TEST(WindowTest, WindowCreateTestWithComponents) {
    Window wnd(GetModuleHandle(nullptr));
    Label foo = Label("bar", { 50,50 });
    wnd.SetVisible(true);
    wnd.SetSize({ 100, 100 });
    wnd.SetColor({ 255,255,255 });
    wnd.Add(foo);
    EXPECT_EQ(wnd.Run(callbackWindow), 0);

}
