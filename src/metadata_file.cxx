#include "SDF1/metadata_file.h"

#include <cstring>

namespace SDF1 {
namespace Metadata {

uint32_t& FileHeader::file_version() { return m_hdr.file_version; }
uint32_t& FileHeader::compression() { return m_hdr.compression; }
uint64_t& FileHeader::file_revision() { return m_hdr.file_revision; }
uint64_t& FileHeader::last_processing_time() {
  return m_hdr.last_processing_time;
}
uint64_t& FileHeader::maximum_compressed_bucket_size() {
  return m_hdr.maximum_compressed_bucket_size;
}
uint64_t& FileHeader::max_uncompressed_size() {
  return m_hdr.max_uncompressed_size;
}
uint32_t& FileHeader::compression_dict_size() {
  return m_hdr.compression_dict_size;
}
uint32_t FileHeader::num_buckets() { return m_buckets.size(); }
uint64_t& FileHeader::reserved_len() { return m_hdr.reserved_len; }
// const references
const uint32_t& FileHeader::file_version() const { return m_hdr.file_version; }
const uint32_t& FileHeader::compression() const { return m_hdr.compression; }
const uint64_t& FileHeader::file_revision() const {
  return m_hdr.file_revision;
}
const uint64_t& FileHeader::last_processing_time() const {
  return m_hdr.last_processing_time;
}
const uint64_t& FileHeader::maximum_compressed_bucket_size() const {
  return m_hdr.maximum_compressed_bucket_size;
}
const uint64_t& FileHeader::max_uncompressed_size() const {
  return m_hdr.max_uncompressed_size;
}
const uint32_t& FileHeader::compression_dict_size() const {
  return m_hdr.compression_dict_size;
}
size_t FileHeader::comment_len() const { return m_comments.length(); }
size_t FileHeader::producer_tag_len() const { return m_producer.length(); }
uint32_t FileHeader::record_schema_len() const {
  return m_record_schema.length();
}
const uint64_t& FileHeader::reserved_len() const { return m_hdr.reserved_len; }

uint64_t* const FileHeader::get_bucket_offsets() const {
  if (m_hdr.num_buckets) {
    return (uint64_t*)(m_buckets.data());
  }
  return nullptr;
}
const std::string& FileHeader::get_producer_tag() const {  // makes a copy
  return m_producer;
}
const std::string& FileHeader::get_comments() const { return m_comments; }
const std::string& FileHeader::get_record_schema() const {
  return m_record_schema;
}
const std::string& FileHeader::key() const { return m_key; }

bool FileHeader::read(uintptr_t buff, size_t buff_len) {
  if (calculate_header_size(buff, buff_len) < buff_len) return false;
  FileHeader_s* h = (FileHeader_s*)buff;
  m_hdr = *h;
  uintptr_t payload = buff + sizeof(FileHeader_s);
  uint64_t* bucket_offsets = (uint64_t*)payload;
  m_buckets.assign(bucket_offsets, bucket_offsets + h->num_buckets);
  m_producer.assign((char*)(payload + producer_offset()), h->producer_tag_len);
  m_comments.assign((char*)(payload + comment_offset()), h->comment_len);
  m_record_schema.assign((char*)(payload + schema_offset()),
                         h->record_schema_len);
  m_key.assign(h->key, std::strlen(HEADER_MAGIC));
  return true;
}

bool FileHeader::write(uintptr_t buff, size_t buff_len) {
  if (size() > buff_len) return false;
  if (!buff) return false;
  uintptr_t buff_location(buff);

  m_hdr.comment_len = m_comments.size();
  m_hdr.record_schema_len = m_record_schema.size();
  m_hdr.num_buckets = m_buckets.size();
  m_hdr.producer_tag_len = m_producer.size();
  // header
  std::memcpy((void*)buff_location, &m_hdr, sizeof(FileHeader_s));
  buff_location += sizeof(FileHeader_s);
  // bucket offsets
  std::memcpy((void*)(buff_location), m_buckets.data(),
              sizeof(decltype(m_buckets)::value_type) * m_buckets.size());
  buff_location += sizeof(decltype(m_buckets)::value_type) * m_buckets.size();
  // producer tag
  std::memcpy((void*)buff_location, m_producer.data(), m_producer.size());
  buff_location += m_producer.size();
  // comment
  std::memcpy((void*)buff_location, m_comments.data(), m_comments.size());
  buff_location += m_comments.size();
  // record schema
  std::memcpy((void*)buff_location, m_record_schema.data(),
              m_record_schema.size());
  buff_location += m_record_schema.size();
}

}  // namespace Metadata

}  // namespace SDF1
