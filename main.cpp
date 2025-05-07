#include <core/CToastAPI.h>
#include <string>
#include <iostream>
#include <windows.h>


cinnamontoast::CToastAPI* api = nullptr;
cinnamontoast::ComponentId button1;
cinnamontoast::ComponentId titleText;
cinnamontoast::ComponentId textText;

void onClick(cinnamontoast::ComponentId btn) {
    const char* titleStr = api->getText(titleText);
    const char* textStr = api->getText(textText);
    MessageBoxA(nullptr, textStr, titleStr, MB_OK);
}
extern "C" __declspec(dllexport) void CToastMain(cinnamontoast::CToastAPI* api_) {
    api = api_;
    button1 = api->getComponentById("btn1");
    
    titleText = api->getComponentById("text1");
    textText = api->getComponentById("text2");
    api->setOnClick(button1, onClick);
    
};