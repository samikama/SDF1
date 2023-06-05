#pragma once
#include <cstdint>
#include <cstring>
#include <vector>

namespace SDF1 {
constexpr uint64_t OFFSET_TABLE_MASK(0x8000000000000000);
struct RecordHash {
  uint64_t hash[4];
};
struct RecordHashOffset {
  RecordHash h;
  uint64_t offset;
};

class Bucket {
 public:
  Bucket(bool with_offsets = false)
      : m_num_records(0),
        m_compressed_size(0),
        m_uncompressed_size(0),
        m_with_offsets(with_offsets) {}
  static const size_t storage_size() {}
  const uint64_t uncompressed_size() { return m_uncompressed_size; };
  uint64_t num_records() { return m_num_records & ~OFFSET_TABLE_MASK; }
  bool with_offsets() { return m_with_offsets; }
  bool compressed() { return m_compressed_size != m_uncompressed_size; }

 private:
  uint64_t m_num_records;
  uint64_t m_compressed_size;
  uint64_t m_uncompressed_size;
  // should this be a map?
  std::vector<RecordHashOffset> m_offsets;
  bool m_with_offsets;
};
}  // namespace SDF1
