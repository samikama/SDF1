#include "SDF1/utils/logging.h"

#include "SDF1/utils/utils.h"
#ifndef SOURCE_PREFIX_LENGTH
#define SOURCE_PREFIX_LENGTH 0
#endif
namespace SDF1 {
namespace utils {
Logger::Logger(const char* file_name, int line_no, LogLevel severity)
    : m_file(file_name), m_line(line_no), m_severity(severity) {}
Logger::~Logger() { ProduceLogging(); }
void Logger::ProduceLogging() {
  if (GetLogLevel() <= (int)m_severity) {
    auto now = std::chrono::high_resolution_clock::now();
    uint64_t nowms = std::chrono::duration_cast<std::chrono::milliseconds>(
                         now.time_since_epoch())
                         .count();
    // // time_t tseconds = nowms / 1000;

    // // struct tm tm;
    // // localtime_r(&tseconds, &tm);
    char LogHdr[1024];
    uint64_t tseconds = nowms / 1000;
    size_t used = timepoint_to_datestr(tseconds, LogHdr, 1024, false);
    int32_t trem = nowms - tseconds;
    // size_t used = strftime(LogHdr, 1024, "%T", &tm);
    snprintf(LogHdr + used, 1024 - used, ".%03d LOG[%c] %s:%d", trem,
             "DIWEF"[int(m_severity)], m_file + SOURCE_PREFIX_LENGTH, m_line);
    fprintf(stderr, "%s %s\n", LogHdr, str().c_str());
  }
}
}  // namespace utils

}  // namespace SDF1
