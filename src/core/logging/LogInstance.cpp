#include "LogInstance.h"
#include "Console.h"
namespace reflect {
void LogInstance::setLogBuffer(LogBuffer *buf) { this->buffer = buf; }

LogInstance::operator std::streambuf &() { return *(this->buffer); }

} // namespace reflect