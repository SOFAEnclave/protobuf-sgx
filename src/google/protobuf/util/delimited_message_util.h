// Adapted from the patch of kenton@google.com (Kenton Varda)
// See https://github.com/protocolbuffers/protobuf/pull/710 for details.

#ifndef GOOGLE_PROTOBUF_UTIL_DELIMITED_MESSAGE_UTIL_H__
#define GOOGLE_PROTOBUF_UTIL_DELIMITED_MESSAGE_UTIL_H__

#include <google/protobuf/message_lite.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

#include <google/protobuf/port_def.inc>

namespace google {
namespace protobuf {
namespace util {

// Read a single size-delimited message from the given stream. Delimited
// format allows a single file or stream to contain multiple messages,
// whereas normally parsing consumes the entire input. A delimited message
// is a varint encoding the message size followed by a message of exactly
// that size.
//
// If |clean_eof| is not NULL, then it will be set to indicate whether the
// stream ended cleanly. That is, if the stream ends without this method
// having read any data at all from it, then *clean_eof will be set true,
// otherwise it will be set false. Note that these methods return false
// on EOF, but they also return false on other errors, so |clean_eof| is
// needed to distinguish a clean end from errors.
bool PROTOBUF_EXPORT ParseDelimitedFromCodedStream(MessageLite* message,
                                                   io::CodedInputStream* input,
                                                   bool* clean_eof);

// Write a single size-delimited message from the given stream. Delimited
// format allows a single file or stream to contain multiple messages,
// whereas normally writing multiple non-delimited messages to the same
// stream would cause them to be merged. A delimited message is a varint
// encoding the message size followed by a message of exactly that size.
bool PROTOBUF_EXPORT SerializeDelimitedToZeroCopyStream(
    const MessageLite& message, io::ZeroCopyOutputStream* output);

bool PROTOBUF_EXPORT SerializeDelimitedToCodedStream(
    const MessageLite& message, io::CodedOutputStream* output);

}  // namespace util
}  // namespace protobuf
}  // namespace google

#include <google/protobuf/port_undef.inc>

#endif  // GOOGLE_PROTOBUF_UTIL_DELIMITED_MESSAGE_UTIL_H__
