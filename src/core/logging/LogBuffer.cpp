#include "LogBuffer.h"
#include <windows.h>
namespace CinnamonToast {
LogBuffer::LogBuffer() : fileStream("log.txt") {
  sb1 = std::cout.rdbuf();
  sb2 = fileStream.rdbuf();
}
int LogBuffer::overflow(int c) {
  __try {
    if (sb1 == nullptr || sb2 == nullptr)
      return EOF;
    if (c == EOF)
      return !EOF;
    if (sb1->sputc(c) == EOF)
      return EOF;
    if (sb2->sputc(c) == EOF)
      return EOF;
  } __except (EXCEPTION_EXECUTE_HANDLER) {
    // Handle exception
    return EOF;
  }

  return c;
}
int LogBuffer::sync() {
  __try {

    if (sb1 == nullptr || sb2 == nullptr)
      return -1;
    return (sb1->pubsync() == 0 && sb2->pubsync() == 0) ? 0 : -1;
  } __except (EXCEPTION_EXECUTE_HANDLER) {
    // Handle exception
    return -1;
  }
}
} // namespace CinnamonToast