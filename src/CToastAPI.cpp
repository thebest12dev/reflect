/**
* Copyright (c) 2025 thebest12lines
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software
* and associated documentation files (the "Software"), to deal in the Software without restriction,
* including without limitation the rights to use, copy, modify, merge, publish, distribute,
* sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
* BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
typedef unsigned char byte;
#include <Windows.h>
#include "ui/Label.h"
#include "CToastAPI.h"
#include "ui/Component.h"
#include "ui/Components.h"
#include "Console.h"
#include <iostream>
#include "ui/TextComponent.h"
#include <vector>
using namespace CinnamonToast;
namespace {
	vector<const char*> cull = {};
}

ComponentId ExternalAPI::GetComponentById(const char* id) {
	
	cull.push_back(id);
	return cull.size();
};
const char* ExternalAPI::GetComponentText(ComponentId ref) {
    // Ensure the ref is within bounds
    if (ref - 1 < 0 || ref - 1 >= cull.size()) {
        
        return nullptr;  // Return null if invalid
    }

    // Attempt dynamic_cast to TextComponent*
    TextComponent* comp = (TextComponent*) (Components::GetComponentById(cull[ref - 1]));
    Label* label = dynamic_cast<Label*>(comp);
    if (label) {

        string* text = new string(label->GetText());
        const char* textc = text->c_str();

        return textc;
    } else {
        Console::warn("cast for component failed! (nullptr)", "GetComponentText");
        return nullptr;  // Return null if the cast fails
    }
}
