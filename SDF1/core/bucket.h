#ifndef _SDF1_CORE_BUCKET_H_
#define _SDF1_CORE_BUCKET_H_

#include <cstdint>
#include <cstring>
#include <vector>

#include "SDF1/core/errors.h"
#include "SDF1/core/record.h"
namespace SDF1 {
constexpr uint64_t OFFSET_TABLE_MASK(0x8000000000000000);
struct RecordHashOffset {
  RecordHash h;
  uint64_t offset;
};

struct BucketHeader_s {
  uint64_t compressed_size;
  uint64_t uncompressed_size;
  uint64_t num_records;
};

class Bucket {
 public:
  Bucket(bool with_offsets = false)
      : m_num_records(0),
        m_compressed_size(0),
        m_uncompressed_size(0),
        m_with_offsets(with_offsets) {}
  const size_t storage_size() { return m_compressed_size; }
  const uint64_t uncompressed_size() { return m_uncompressed_size; };
  uint64_t& num_records() { return m_num_records; }
  const uint64_t& num_records() const { return m_num_records; }
  bool with_offsets() { return m_with_offsets; }
  bool compressed() { return m_compressed_size != m_uncompressed_size; }
  char*& record_buffer() { return m_record_buffer; }
  std::vector<RecordHashOffset>& record_offsets() { return m_offsets; }
  errors::Status read(char* file_buff, size_t bufflen);
  errors::Status write(char* output_buff, bool compress = false);
  errors::Status get_record(const RecordHash& hash, Record*& record);
  errors::Status get_record(size_t pos, Record*& record);

 private:
  uint64_t m_num_records;
  uint64_t m_compressed_size;
  uint64_t m_uncompressed_size;
  // should this be a map?
  std::vector<RecordHashOffset> m_offsets;
  char* m_record_buffer;
  char* m_file_buff;
  size_t m_bufflen;
  bool m_with_offsets;
};
}  // namespace SDF1

#endif