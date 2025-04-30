#include "LogUtil.h"
#include "Console.h"
#include "LogInstance.h"
namespace CinnamonToast {
namespace {

static LogInstance globalLogInstance;
static LogBuffer globalLogBuffer;
} // namespace

inline void Utilities::initLogs() {
  globalLogInstance.setLogBuffer(&globalLogBuffer);
  globalLogInstance.initialize();
  Console::setLogInstance(globalLogInstance);
  ctoastDebug("enabled logs!");
}
} // namespace CinnamonToast
