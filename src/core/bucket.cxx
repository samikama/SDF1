#include "SDF1/core/bucket.h"

SDF1::errors::Status SDF1::Bucket::read(char* file_buff, size_t bufflen) {
  if (m_file_buff) {
    return SDF1::errors::FailedPreconditionError(
        "This bucket alread have an assigned buffer!");
  }
  if (bufflen < sizeof(BucketHeader_s)) {
    return SDF1::errors::InvalidArgumentError(
        "Invalid bucket buffer. Insufficient size");
  }
  BucketHeader_s* bh = (BucketHeader_s*)file_buff;

  if (bufflen <
      (sizeof(BucketHeader_s) + bh->num_records * sizeof(RecordHashOffset))) {
    return SDF1::errors::InvalidArgumentError("Insufficient bucket buffer!");
  }
  m_file_buff = file_buff;
  m_bufflen = bufflen;
  // This works because BucketHeader_s is 192 bytes and would be packed
  RecordHashOffset* rhbegin = (RecordHashOffset*)(bh + 1);
  m_offsets.assign(rhbegin, rhbegin + bh->num_records);
  if (bh->compressed_size == 0) {  // uncompressed
    m_record_buffer = (char*)(rhbegin + bh->num_records);
  } else {
    return SDF1::errors::UnimplementedError(
        "Compressed buffers are not yet implemented!");
  }
  return Status();
}
SDF1::errors::Status SDF1::Bucket::get_record(const RecordHash& hash,
                                              Record*& record) {
  return Status();
}

SDF1::errors::Status SDF1::Bucket::get_record(size_t pos, Record*& record) {
  return Status();
}
