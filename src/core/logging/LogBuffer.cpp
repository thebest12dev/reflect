#include "LogBuffer.h"

namespace CinnamonToast {
LogBuffer::LogBuffer() : fileStream("log.txt") {
  sb1 = std::cout.rdbuf();
  sb2 = fileStream.rdbuf();
}
int LogBuffer::overflow(int c) {
  if (c == EOF)
    return !EOF;
  if (sb1->sputc(c) == EOF)
    return EOF;
  if (sb2->sputc(c) == EOF)
    return EOF;
  return c;
}
int LogBuffer::sync() {
  return (sb1->pubsync() == 0 && sb2->pubsync() == 0) ? 0 : -1;
}
} // namespace CinnamonToast