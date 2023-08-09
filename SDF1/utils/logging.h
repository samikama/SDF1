#ifndef __SDF1_UTILS_LOGGING_H_
#define __SDF1_UTILS_LOGGING_H_

#include <chrono>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>

#include "SDF1/utils/utils.h"
// #include "date.h"
namespace SDF1 {
namespace utils {
class Logger;
enum class LogLevel : int {
  DEBUG = 0,
  INFO = 1,
  WARNING = 2,
  ERROR = 3,
  FATAL = 4
};

// inspired by old TF logging code

class Logger : public std::basic_ostringstream<char> {
 public:
  Logger(const char* file_name, int line_no, LogLevel severity);
  ~Logger() override;
  //   template <typename T>
  //   Logger& operator<<(const T& v) {
  //     m_oss << v;
  //   }
  //   Logger& operator<<(std::ostream& (*Fuctor)(std::ostream&)) const {
  //     Functor(m_oss);
  //     return *this;
  //   }
  static int GetLogLevel() {
    static int LOG_LEVEL = []() -> int32_t {
      auto c = std::getenv("SDF1_LOG_LEVEL");
      return AsType<int32_t>(c, 1);
    }();
    return LOG_LEVEL;
  }

  static int GetVLogLevel() {
    static int VLOG_LEVEL = []() -> int32_t {
      auto c = std::getenv("SDF1_VLOG_LEVEL");
      return AsType<int32_t>(c, 1);
    }();
    return VLOG_LEVEL;
  }

 protected:
  void ProduceLogging();

 private:
  //   std::stringstream m_oss;
  const char* m_file;
  int m_line;
  LogLevel m_severity;
};

#define _LOG_DEBUG \
  SDF1::utils::Logger(__FILE__, __LINE__, SDF1::utils::LogLevel::DEBUG)
#define _LOG_INFO \
  SDF1::utils::Logger(__FILE__, __LINE__, SDF1::utils::LogLevel::INFO)
#define _LOG_WARNING \
  SDF1::utils::Logger(__FILE__, __LINE__, SDF1::utils::LogLevel::WARNING)
#define _LOG_ERROR \
  SDF1::utils::Logger(__FILE__, __LINE__, SDF1::utils::LogLevel::ERROR)
#define _LOG_FATAL \
  SDF1::utils::Logger(__FILE__, __LINE__, SDF1::utils::LogLevel::FATAL)

#define LOG(severity) _LOG_##severity

// #define VLOG(severity)                       \
//   if (SDF1::utils::Logger::GetVLogLevel() >= \
//       SDF1::utils::LogLevel::##severity) {   \
//   }
}  // namespace utils

}  // namespace SDF1

#endif