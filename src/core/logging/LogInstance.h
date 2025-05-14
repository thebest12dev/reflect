#pragma once

#include "ConsoleHelper.h"
#include "LogBuffer.h"
#include "TypeDefinitions.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace {
std::string getFilesystemSafeTime() {
  // Get current time as time_t
  auto now = std::chrono::system_clock::now();
  std::time_t t = std::chrono::system_clock::to_time_t(now);

  // Convert to UTC
  std::tm utc_tm;
#if defined(_WIN32) || defined(_WIN64)
  gmtime_s(&utc_tm, &t);
#else
  gmtime_r(&t, &utc_tm); // POSIX
#endif

  // Format time
  std::ostringstream oss;
  oss << std::put_time(&utc_tm,
                       "%Y-%m-%dT%H-%M-%SZ"); // Replace colons with hyphens
  return oss.str();
}
} // namespace

namespace reflect {

class LogInstance {
protected:
  LogBuffer *buffer = nullptr;

public:
  REFLECT_API inline void setLogBuffer(LogBuffer *buf);
  REFLECT_API void initialize() {
    if (buffer) {
      buffer->setOutputFile("logs/log_" + getFilesystemSafeTime() + ".log");
    }
  }
  template <typename T> REFLECT_API inline void print(const T &obj) noexcept {
    try {
      std::cout << obj;
    } catch (const std::exception &e) {
      std::cerr << "[ERROR] [" << __func__ << "]: " << e.what() << std::endl;
    }
  };

  template <typename T> REFLECT_API inline void println(const T &obj) {
    try {
      std::cout << obj << std::endl;
    } catch (const std::exception &e) {
      std::cerr << "[ERROR] [" << __func__ << "]: " << e.what() << std::endl;
    }
  };
  ;

  template <typename T>
  REFLECT_API inline void error(const T &obj, std::string ctx = "default") {
    try {
      std::cerr << "[ERROR] " << "[" << ctx << "]: " << obj << std::endl;
    } catch (const std::exception &e) {
      std::cerr << "[ERROR] [" << __func__ << "] " << e.what() << std::endl;
    }
  };

  template <typename T>
  REFLECT_API inline void info(const T &obj, std::string ctx = "default") {
    try {
      std::cout << "[INFO] " << "[" << ctx << "]: " << obj << std::endl;

    } catch (const std::exception &e) {
      std::cerr << "[ERROR] [" << __func__ << "] " << e.what() << std::endl;
    }
  };

  template <typename T>
  REFLECT_API inline void warn(const T &obj, std::string ctx = "default") {
    try {
      std::cout << "[WARN] " << "[" << ctx << "]: " << obj << std::endl;
    } catch (const std::exception &e) {
      std::cerr << "[ERROR] [" << __func__ << "] " << e.what() << std::endl;
    }
  };

  template <typename T>
  REFLECT_API inline void debug(const T &obj, std::string ctx = "default") {
    try {
      if (reflect::console::getDebugEnabled())
        std::cout << "[DEBUG] " << "[" << ctx << "]: " << obj << std::endl;
    } catch (const std::exception &e) {
      std::cerr << "[ERROR] [" << __func__ << "] " << e.what() << std::endl;
    }
  };
  REFLECT_API operator std::streambuf &();
};
} // namespace reflect