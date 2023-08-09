#ifndef __SDF1_CORE_ERRORS_H_
#define __SDF1_CORE_ERRORS_H_
#include <string>

#include "absl/status/status.h"

namespace SDF1 {
namespace errors {
typedef absl::StatusCode Code;
using namespace absl;
}  // namespace errors

typedef absl::Status Status;

// class Status {
//  public:
//   enum StatusCode {
//     OK = 0,
//     FAILURE = 1,
//     INVALID_DATA = 2,
//     INSUFFICIENT_PERMISSIONS = 3,
//     FAILED_PRECONDITION = 4,
//     UNKNOWN_FAILURE = 5
//   };

//   constexpr Status() : m_s(StatusCode::OK){};
//   Status(StatusCode s, const std::string& msg) : m_s(s), m_msg(msg) {}
//   constexpr Status OK() { return Status(); }

//  private:
//   STATUS m_s;
//   std::string m_msg
// }

}  // namespace SDF1

#endif