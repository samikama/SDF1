#pragma once
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

namespace SDF1 {
namespace Metadata {

constexpr char HEADER_MAGIC[] = "SDF1Meta";
class FileHeader {
 public:
  uint32_t& file_version();
  uint32_t& compression();
  uint64_t& file_revision();
  uint64_t& last_processing_time();
  uint64_t& maximum_compressed_bucket_size();
  uint64_t& max_uncompressed_size();
  uint32_t& compression_dict_size();
  uint32_t num_buckets();
  uint64_t& reserved_len();
  // const references
  constexpr const uint32_t& file_version() const;
  constexpr const uint32_t& compression() const;
  constexpr const uint64_t& file_revision() const;
  constexpr const uint64_t& last_processing_time() const;
  constexpr const uint64_t& maximum_compressed_bucket_size() const;
  constexpr const uint64_t& max_uncompressed_size() const;
  constexpr const uint32_t& compression_dict_size() const;
  constexpr const uint64_t& reserved_len() const;

  uint64_t* const get_bucket_offsets() const;
  size_t comment_len() const;
  size_t producer_tag_len() const;
  uint32_t record_schema_len() const;

  const std::string& get_producer_tag() const;
  const std::string& get_comments() const;
  const std::string& get_record_schema() const;
  const std::string& key() const { return m_key; }
  bool read(uintptr_t buff, size_t buff_len);
  bool write(uintptr_t buff, size_t buff_len);
  static uint64_t calculate_header_size(int32_t num_buckets,
                                        const std::string& producer_tag,
                                        const std::string& comment,
                                        const std::string& schema,
                                        uint32_t reserve) {
    return sizeof(FileHeader) + num_buckets * (sizeof(uint64_t)) +
           producer_tag.size() + comment.size() + schema.size() + reserve;
  }

  static uint64_t calculate_header_size(const uintptr_t buff, size_t buff_len) {
    if (buff_len < sizeof(FileHeader_s)) {
      return 0;
    }
    FileHeader_s* h = (FileHeader_s*)(buff);
    if (std::memcmp(h->key, HEADER_MAGIC,
                    strlen(HEADER_MAGIC))) {  // header magic is not correct!
      return 0;
    }
    return sizeof(FileHeader_s) + h->comment_len + h->producer_tag_len +
           h->num_buckets * sizeof(uint64_t) + h->comment_len +
           h->record_schema_len + h->reserved_len;
  };

 private:
  struct FileHeader_s;
  constexpr uint64_t producer_offset() const {
    return m_buckets.size() * sizeof(uint64_t);
  };

  constexpr uint64_t comment_offset() const {
    return producer_offset() + m_producer.size();
  };

  constexpr uint64_t schema_offset() const {
    return comment_offset() + m_comments.size();
  }

  constexpr uint64_t reserved_offset() const {
    schema_offset() + m_record_schema.size();
  }

  constexpr uint64_t size() const {
    return sizeof(FileHeader) + reserved_offset() + m_hdr.reserved_len;
  }

  struct FileHeader_s {
    char key[8];
    uint32_t file_version;
    uint32_t compression;
    uint64_t file_revision;
    uint64_t last_processing_time;
    uint64_t maximum_compressed_bucket_size;
    uint64_t max_uncompressed_size;
    uint32_t compression_dict_size;
    uint16_t comment_len;
    uint16_t producer_tag_len;
    uint32_t record_schema_len;
    uint32_t num_buckets;
    uint64_t reserved_len;
  };

  FileHeader_s m_hdr;
  std::vector<uint64_t> m_buckets;
  std::string m_producer;
  std::string m_comments;
  std::string m_record_schema;
  std::string m_key;
};

}  // namespace Metadata

}  // namespace SDF1
