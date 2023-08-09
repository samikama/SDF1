#ifndef __SDF1__CORE_RECORD_H_
#define __SDF1__CORE_RECORD_H_
#include <unistd.h>

#include <string>

#include "SDF1/utils/utils.h"
namespace SDF1 {
struct RecordHash {
  uint64_t hash[4];
};

struct Record {
  RecordHash hash;
  uint32_t key_len;
  uint32_t payload_len;
  char payload[];
};

struct RecordPayload {
  uint32_t len;
  const char* data;
};
namespace RH {

static const std::string Key(const Record* r) {
  return std::string(r->payload, r->key_len);
}

static RecordPayload Payload(const Record* r) {
  return {r->payload_len, (r->payload + SDF1::utils::Ceil64bit(r->key_len))};
}
static size_t RecordLength(const Record* r) {
  return sizeof(RecordHash) + SDF1::utils::Ceil64bit(r->key_len) +
         SDF1::utils::Ceil64bit(r->payload_len) + sizeof(Record::key_len) +
         sizeof(Record::payload_len);
}

static size_t RecordLength(uint32_t key_len, uint32_t payload_len) {
  return sizeof(RecordHash) + SDF1::utils::Ceil64bit(key_len) +
         SDF1::utils::Ceil64bit(payload_len) + sizeof(Record::key_len) +
         sizeof(Record::payload_len);
}
}  // namespace RH

}  // namespace SDF1

#endif