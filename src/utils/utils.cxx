#include "SDF1/utils/utils.h"

#include <ctime>

namespace SDF1 {
namespace utils {
void timepoint_str(
    const std::chrono::time_point<std::chrono::high_resolution_clock>& t,
    char* buff, int bufflen) {
  // time_t as_time_t;
  //   if (std::is_same<decltype(t),
  //   std::chrono::system_clock::time_point>::value) {
  //     as_time_t = std::chrono::system_clock::to_time_t(t);
  //   } else {
  //     auto tt = std::chrono::system_clock::time_point(
  //         std::chrono::duration_cast<std::chrono::microseconds>(
  //             t.time_since_epoch()));
  //     as_time_t = std::chrono::system_clock::to_time_t(tt);
  //   }
  //   std::tm tm;
  auto today = date::floor<date::days>(t);
}

void timepoint_str(const std::chrono::system_clock::time_point& t, char* buff,
                   int bufflen) {
  auto as_time_t = std::chrono::system_clock::to_time_t(tt);
  struct tm tmp;
  // auto tm = localtime_r();
}
}  // namespace utils

}  // namespace SDF1
