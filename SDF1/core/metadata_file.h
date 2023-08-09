#pragma once
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

#include "SDF1/core/bucket.h"
#include "SDF1/core/file_header.h"

namespace SDF1 {
namespace Metadata {
class FileWriter {
 public:
  FileWriter(uint32_t file_revision, uint32_t compression, uint32_t num_buckets,
             const std::string& comment, const std::string& producer_tag,
             const std::string& record_schema, uint64_t reserved = 0);
  // Open a new file, overwrite if needed.
  bool virtual open(const std::string& name);
  // Close file, flush header and buckets
  bool virtual close();
  // Append a bucket to the file and update in-memory structures
  bool virtual write_bucket(SDF1::Bucket* bucket);
  // Reopen an existing file, update in-memory structures from file if possible.
  bool virtual reopen(const std::string& name);

 private:
  int m_file;
  size_t m_header_size;
  std::string m_filename;
  SDF1::FileHeader<SDF1::Header_t::METADATA> m_header;
};
}  // namespace Metadata

}  // namespace SDF1
