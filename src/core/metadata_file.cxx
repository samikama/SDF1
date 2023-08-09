#include "SDF1/core/metadata_file.h"

#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

#include "SDF1/utils/logging.h"

namespace SDF1 {
namespace Metadata {  // namespace Metadata
FileWriter::FileWriter(uint32_t file_revision, uint32_t compression,
                       uint32_t num_buckets, const std::string& comment,
                       const std::string& producer_tag,
                       const std::string& record_schema, uint64_t reserved) {
  m_header.file_revision() = file_revision;
  m_header.compression() = compression;
  m_header.comments() = comment;
  m_header.record_schema() = record_schema;
  m_header.producer_tag() = producer_tag;
  m_header_size = m_header.calculate_header_size(
      num_buckets, producer_tag, comment, record_schema, reserved);
  m_header.bucket_offsets().reserve(num_buckets);
}

bool FileWriter::open(const std::string& name) {
  if (m_file) {
    LOG(ERROR) << "File " << m_file
               << " is already open. Please close existing file first";
    return false;
  }
  m_file = ::open(name.c_str(), O_RDWR | O_CREAT);
  if (m_file == -1) {
    char buff[1024];
    strerror_r(errno, buff, 1024);
    LOG(ERROR) << "Failed to open " << name << " error is " << errno << " \""
               << buff << "\"";
    m_filename = "";
    m_file = 0;
    return false;
  }
  off_t flen = ::lseek(m_file, 0, SEEK_END);
  if (flen < 0) {
    char buff[1024];
    strerror_r(errno, buff, 1024);
    LOG(ERROR) << "Failed to seek end of " << name << " error is " << errno
               << " \"" << buff << "\"";
    m_filename = "";
    ::close(m_file);
    m_file = 0;
    return false;
  }
  if (flen > decltype(m_header)::min_size()) {
    ::lseek(m_file, 0, SEEK_SET);
    size_t min_buffer_size = decltype(m_header)::min_size();
    char* buff = (char*)malloc(min_buffer_size);
    read(m_file, buff, min_buffer_size);
    size_t full_hdr_size =
        m_header.get_header_size((uintptr_t)buff, min_buffer_size);
    if (min_buffer_size < full_hdr_size) {
      free(buff);
      buff = (char*)malloc(full_hdr_size);
    }
    if (!m_header.read((uintptr_t)buff, full_hdr_size)) {
      LOG(ERROR) << "Failed to read the header!";
      ::close(m_file);
      m_file = 0;
      m_filename = "";
      free(buff);
      buff = nullptr;
      return false;
    }
    free(buff);
    buff = nullptr;
  } else {
  }
}

bool FileWriter::close() {}
bool FileWriter::write_bucket(SDF1::Bucket* bucket) {}
bool FileWriter::reopen(const std::string& name) {
  if (m_file) {
    LOG(ERROR) << "File " << m_file
               << " is already open. Please close existing file first";
    return false;
  }
  m_file = ::open(name.c_str(), O_RDWR);
  if (m_file == -1) {
    char buff[1024];
    strerror_r(errno, buff, 1024);
    LOG(ERROR) << "Failed to open " << name << " error is " << errno << " \""
               << buff << "\"";
    m_filename = "";
    m_file = 0;
    return false;
  }
  off_t flen = ::lseek(m_file, 0, SEEK_END);
  if (flen < 0) {
    char buff[1024];
    strerror_r(errno, buff, 1024);
    LOG(ERROR) << "Failed to seek end of " << name << " error is " << errno
               << " \"" << buff << "\"";
    m_filename = "";
    ::close(m_file);
    m_file = 0;
    return false;
  }
  if (flen > decltype(m_header)::min_size()) {
    ::lseek(m_file, 0, SEEK_SET);
    size_t min_buffer_size = decltype(m_header)::min_size();
    char* buff = (char*)malloc(min_buffer_size);
    read(m_file, buff, min_buffer_size);
    size_t full_hdr_size =
        m_header.get_header_size((uintptr_t)buff, min_buffer_size);
    if (min_buffer_size < full_hdr_size) {
      free(buff);
      buff = (char*)malloc(full_hdr_size);
    }
    if (!m_header.read((uintptr_t)buff, full_hdr_size)) {
      LOG(ERROR) << "Failed to read the header!";
      ::close(m_file);
      m_file = 0;
      m_filename = "";
      free(buff);
      buff = nullptr;
      return false;
    }
    free(buff);
    buff = nullptr;
  } else {
  }
}
}  // namespace Metadata
}  // namespace SDF1
