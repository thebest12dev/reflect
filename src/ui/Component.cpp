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
#include "Component.h"
#include <cstdint>
#include "../TypeDefinitions.h"
#include "Vector2.h"
#include <string>
ctoast Component::Component() : position(Vector2(0,0)), size(Vector2(0,0)) {
    
}
ctoast Component::~Component()  {
    
}
#ifdef __linux__
    void ctoast Component::SetColor(uint8_t r, uint8_t g, uint8_t b) {
        this->color[0] = r;
        this->color[1] = g;
        this->color[2] = b;
    };
    ctoast Vector2 ctoast Component::GetPosition() {
        return position;
    };
    ctoast Vector2 ctoast Component::GetSize() {
        return size;
    };
    // string ctoast Component::GetProperty(string property) {
    //     return "";
    // }
    bool ctoast Component::GetVisible() {
        return visible;
    };
#endif