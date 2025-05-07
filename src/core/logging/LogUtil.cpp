#include "LogUtil.h"
#include "Console.h"
#include "LogInstance.h"
namespace cinnamontoast {
namespace {

static LogInstance globalLogInstance;
static LogBuffer globalLogBuffer;
} // namespace

inline void utilities::initLogs() {
  globalLogInstance.setLogBuffer(&globalLogBuffer);
  globalLogInstance.initialize();
  console::setLogInstance(globalLogInstance);
  ctoastDebug("enabled logs!");
}
} // namespace cinnamontoast
