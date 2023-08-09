#ifndef __SDF1_CORE_UTILS_UTILS_H__
#define __SDF1_CORE_UTILS_UTILS_H__
#include <chrono>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <string>
#include <type_traits>

namespace SDF1 {
namespace utils {
// Round up the unsigned number to nearest 64 bit multiple
// ceil64bit(V)=(V/8)*8+8 when V is integer
template <typename T,
          typename std::enable_if<std::is_integral<T>::value &&
                                  std::is_unsigned<T>::value>::type* = nullptr>
constexpr size_t Ceil64bit(T size) {
  size_t V(size);
  return ((V >> 3) + 1) << 3;
}

template <typename T>
T AsType(const char*, const T&);

template <typename T>
T AsType(const std::string& s, const T& def) {
  if (s.empty()) {
    return def;
  }
  return AsType<T>(s.c_str(), def);
}

template <>
int32_t AsType(const char* c, const int32_t& def) {
  if (c == nullptr) return def;
  return std::strtol(c, nullptr, 10);
}

template <>
uint32_t AsType(const char* c, const uint32_t& def) {
  if (c == nullptr) return def;
  return std::strtoul(c, nullptr, 10);
}

template <>
int64_t AsType(const char* c, const int64_t& def) {
  if (c == nullptr) return def;
  return std::strtoll(c, nullptr, 10);
}

template <>
uint64_t AsType(const char* c, const uint64_t& def) {
  if (c == nullptr) return def;
  return std::strtoull(c, nullptr, 10);
}

template <>
float AsType(const char* c, const float& def) {
  if (c == nullptr) return def;
  return std::strtof(c, nullptr);
}

template <>
double AsType(const char* c, const double& def) {
  if (c == nullptr) return def;
  return std::strtod(c, nullptr);
}

size_t timepoint_to_datestr(uint64_t timepoint_seconds, char* buff,
                            size_t bufflen, bool with_date = false) {
  time_t tseconds = (time_t)timepoint_seconds;
  struct tm tm;
  localtime_r(&tseconds, &tm);
  size_t written = strftime(buff, bufflen, (with_date ? "%F %T" : "%T"), &tm);
  return written;
}

}  // namespace utils

}  // namespace SDF1

#endif