#include "TypeDefinitions.h"
#include <Windows.h>
#undef byte
typedef unsigned char byte;
#include "Notification.h"
#ifdef _WIN32
#include <iostream>
#include <stringapiset.h>

namespace cinnamontoast {
Notification::Notification() : title(""), text("") {}

Notification::Notification(std::string title, std::string text)
    : title(title), text(text) {}

Notification::Notification(std::string text) : title(""), text(text) {}
Notification::~Notification() {}

void Notification::setTitle(std::string title) { this->title = title; }

void Notification::setText(std::string text) { this->text = text; }
} // namespace cinnamontoast
#endif
