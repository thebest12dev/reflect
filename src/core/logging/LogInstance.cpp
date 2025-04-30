#include "LogInstance.h"
#include "Console.h"
namespace CinnamonToast {
void LogInstance::setLogBuffer(LogBuffer *buf) { this->buffer = buf; }

LogInstance::operator std::streambuf &() { return *(this->buffer); }

} // namespace CinnamonToast