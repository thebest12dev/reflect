#include "ConsoleHelper.h"
namespace {
bool debugEnabled = false;
}
namespace CinnamonToast {
void Console::setDebugEnabled(bool enabled) { debugEnabled = enabled; }
bool Console::getDebugEnabled() { return debugEnabled; }
} // namespace CinnamonToast