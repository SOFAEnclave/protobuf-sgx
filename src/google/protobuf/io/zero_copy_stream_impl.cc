// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
// https://developers.google.com/protocol-buffers/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Author: kenton@google.com (Kenton Varda)
//  Based on original Protocol Buffers design by
//  Sanjay Ghemawat, Jeff Dean, and others.

#ifndef _MSC_VER
#include <unistd.h>
#include <sys/types.h>
#endif
#include <errno.h>
#include <algorithm>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/logging.h>
#include <google/protobuf/stubs/stl_util.h>
#include <google/protobuf/stubs/io_win32.h>


namespace google {
namespace protobuf {
namespace io {

#ifdef _WIN32
// Win32 lseek is broken:  If invoked on a non-seekable file descriptor, its
// return value is undefined.  We re-define it to always produce an error.
#define lseek(fd, offset, origin) ((off_t)-1)
// DO NOT include <io.h>, instead create functions in io_win32.{h,cc} and import
// them like we do below.
using google::protobuf::internal::win32::access;
using google::protobuf::internal::win32::close;
using google::protobuf::internal::win32::open;
using google::protobuf::internal::win32::read;
using google::protobuf::internal::win32::write;
#endif

// ===================================================================

ConcatenatingInputStream::ConcatenatingInputStream(
    ZeroCopyInputStream* const streams[], int count)
  : streams_(streams), stream_count_(count), bytes_retired_(0) {
}

bool ConcatenatingInputStream::Next(const void** data, int* size) {
  while (stream_count_ > 0) {
    if (streams_[0]->Next(data, size)) return true;

    // That stream is done.  Advance to the next one.
    bytes_retired_ += streams_[0]->ByteCount();
    ++streams_;
    --stream_count_;
  }

  // No more streams.
  return false;
}

void ConcatenatingInputStream::BackUp(int count) {
  if (stream_count_ > 0) {
    streams_[0]->BackUp(count);
  } else {
    GOOGLE_LOG(DFATAL) << "Can't BackUp() after failed Next().";
  }
}

bool ConcatenatingInputStream::Skip(int count) {
  while (stream_count_ > 0) {
    // Assume that ByteCount() can be used to find out how much we actually
    // skipped when Skip() fails.
    int64 target_byte_count = streams_[0]->ByteCount() + count;
    if (streams_[0]->Skip(count)) return true;

    // Hit the end of the stream.  Figure out how many more bytes we still have
    // to skip.
    int64 final_byte_count = streams_[0]->ByteCount();
    GOOGLE_DCHECK_LT(final_byte_count, target_byte_count);
    count = target_byte_count - final_byte_count;

    // That stream is done.  Advance to the next one.
    bytes_retired_ += final_byte_count;
    ++streams_;
    --stream_count_;
  }

  return false;
}

int64 ConcatenatingInputStream::ByteCount() const {
  if (stream_count_ == 0) {
    return bytes_retired_;
  } else {
    return bytes_retired_ + streams_[0]->ByteCount();
  }
}


// ===================================================================

LimitingInputStream::LimitingInputStream(ZeroCopyInputStream* input,
                                         int64 limit)
  : input_(input), limit_(limit) {
  prior_bytes_read_ = input_->ByteCount();
}

LimitingInputStream::~LimitingInputStream() {
  // If we overshot the limit, back up.
  if (limit_ < 0) input_->BackUp(-limit_);
}

bool LimitingInputStream::Next(const void** data, int* size) {
  if (limit_ <= 0) return false;
  if (!input_->Next(data, size)) return false;

  limit_ -= *size;
  if (limit_ < 0) {
    // We overshot the limit.  Reduce *size to hide the rest of the buffer.
    *size += limit_;
  }
  return true;
}

void LimitingInputStream::BackUp(int count) {
  if (limit_ < 0) {
    input_->BackUp(count - limit_);
    limit_ = count;
  } else {
    input_->BackUp(count);
    limit_ += count;
  }
}

bool LimitingInputStream::Skip(int count) {
  if (count > limit_) {
    if (limit_ < 0) return false;
    input_->Skip(limit_);
    limit_ = 0;
    return false;
  } else {
    if (!input_->Skip(count)) return false;
    limit_ -= count;
    return true;
  }
}

int64 LimitingInputStream::ByteCount() const {
  if (limit_ < 0) {
    return input_->ByteCount() + limit_ - prior_bytes_read_;
  } else {
    return input_->ByteCount() - prior_bytes_read_;
  }
}


// ===================================================================

}  // namespace io
}  // namespace protobuf
}  // namespace google
