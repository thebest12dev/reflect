#include "LogUtil.h"
#include "Console.h"
#include "LogInstance.h"
namespace reflect {
namespace {

static LogInstance globalLogInstance;
static LogBuffer globalLogBuffer;
} // namespace

inline void utilities::initLogs() {
  globalLogInstance.setLogBuffer(&globalLogBuffer);
  globalLogInstance.initialize();
  console::setLogInstance(globalLogInstance);
  reflectDebug("enabled logs!");
}
} // namespace reflect
