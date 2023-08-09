#pragma once
#include <array>
#include <cstdint>
#include <cstring>
#include <limits>
#include <string>
#include <vector>
namespace SDF1 {
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

enum Header_t { RAW = 0, METADATA = 1, NUM_HEADERS };
static const std::array<const char[], 2> HEADER_MAGIC{"SDF1-RAW", "SDF1META"};
template <Header_t T>
class FileHeader {
 public:
  uint32_t& file_version() const { return m_hdr.file_version; }
  uint32_t& compression() { return m_hdr.compression; }
  uint64_t& file_revision() { return m_hdr.file_revision; }
  uint64_t& last_processing_time() { return m_hdr.last_processing_time; }
  uint64_t& maximum_compressed_bucket_size() {
    return m_hdr.maximum_compressed_bucket_size;
  }
  uint64_t& max_uncompressed_size() { return m_hdr.max_uncompressed_size; }
  uint32_t& compression_dict_size() { return m_hdr.compression_dict_size; };
  uint32_t num_buckets() { return m_buckets.size(); };
  uint64_t& reserved_len() { return m_hdr.reserved_len; };
  // const references
  constexpr const uint32_t& file_version() const { return m_hdr.file_version; };
  constexpr const uint32_t& compression() const { return m_hdr.compression; };
  constexpr const uint64_t& file_revision() const {
    return m_hdr.file_revision;
  };
  constexpr const uint64_t& last_processing_time() const {
    return m_hdr.last_processing_time;
  };
  constexpr const uint64_t& maximum_compressed_bucket_size() const {
    return m_hdr.maximum_compressed_bucket_size;
  };
  constexpr const uint64_t& max_uncompressed_size() const {
    return m_hdr.max_uncompressed_size;
  };
  constexpr const uint32_t& compression_dict_size() const {
    return m_hdr.compression_dict_size;
  };
  constexpr const uint64_t& reserved_len() const;

  uint64_t* const get_bucket_offsets() const {
    if (m_hdr.num_buckets) {
      return (uint64_t*)(m_buckets.data());
    }
    return nullptr;
  };
  size_t comment_len() const { return m_comments.size(); };
  size_t producer_tag_len() const { return m_producer.size(); }
  uint32_t record_schema_len() const { return m_record_schema.size(); }

  const std::string& get_producer_tag() const { return m_producer; };
  const std::string& get_comments() const { return m_comments; };
  const std::string& get_record_schema() const { return m_record_schema; };
  const std::string& key() const { return m_key; }
  bool read(uintptr_t buff, size_t buff_len) {
    size_t buff_size = get_header_size(buff, buff_len);
    // if incorrect header type or insufficient buffer
    if (buff_size == 0 || buff_size < buff_len) {
      return false;
    }
    FileHeader_s* h = (FileHeader_s*)buff;
    // incorrect file type!
    if (strncmp(h->key, HEADER_MAGIC[T], 8) != 0) {
      return false;
    }
    m_hdr = *h;
    uintptr_t payload = buff + sizeof(FileHeader_s);
    uint64_t* bucket_offsets = (uint64_t*)payload;
    m_buckets.assign(bucket_offsets, bucket_offsets + h->num_buckets);
    m_producer.assign((char*)(payload + producer_offset()),
                      h->producer_tag_len);
    m_comments.assign((char*)(payload + comment_offset()), h->comment_len);
    m_record_schema.assign((char*)(payload + schema_offset()),
                           h->record_schema_len);
    m_key.assign(h->key, std::strlen(HEADER_MAGIC[T]));
    return true;
  }

  bool write(uintptr_t buff, size_t buff_len) {
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
  static uint64_t calculate_header_size(int32_t num_buckets,
                                        const std::string& producer_tag,
                                        const std::string& comment,
                                        const std::string& schema,
                                        uint32_t reserve) {
    return sizeof(FileHeader) + num_buckets * (sizeof(uint64_t)) +
           producer_tag.size() + comment.size() + schema.size() + reserve;
  }
  constexpr uint64_t size() const {
    return sizeof(FileHeader) + reserved_offset() + m_hdr.reserved_len;
  }

  static uint64_t get_header_size(const uintptr_t buff, size_t buff_len) {
    if (buff_len < sizeof(FileHeader_s)) {
      return 0;
    }
    FileHeader_s* h = (FileHeader_s*)(buff);

    if (std::memcmp(h->key, HEADER_MAGIC[T],
                    strlen(HEADER_MAGIC[T]))) {  // header magic is not correct!
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
}  // namespace SDF1