#include <core/ReflectAPI.h>
#include <string>
#include <iostream>
#include <windows.h>


reflect::ReflectAPI* api = nullptr;
reflect::ComponentId button1;
reflect::ComponentId titleText;
reflect::ComponentId textText;

void onClick(reflect::ComponentId btn) {
    const char* titleStr = api->getText(titleText);
    const char* textStr = api->getText(textText);
    MessageBoxA(nullptr, textStr, titleStr, MB_OK);
}
extern "C" __declspec(dllexport) void reflectMain(reflect::ReflectAPI* api_) {
    api = api_;
    button1 = api->getComponentById("btn1");
    
    titleText = api->getComponentById("text1");
    textText = api->getComponentById("text2");
    api->setOnClick(button1, onClick);
    
};