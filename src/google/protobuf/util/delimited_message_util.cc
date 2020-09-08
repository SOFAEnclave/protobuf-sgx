// Adapted from the patch of kenton@google.com (Kenton Varda)
// See https://github.com/protocolbuffers/protobuf/pull/710 for details.

#include <google/protobuf/util/delimited_message_util.h>

namespace google {
namespace protobuf {
namespace util {

bool ParseDelimitedFromCodedStream(MessageLite* message, io::CodedInputStream* input, bool* clean_eof) {
  if (clean_eof != NULL) *clean_eof = false;
  int start = input->CurrentPosition();

  // Read the size.
  uint32 size;
  if (!input->ReadVarint32(&size)) {
    if (clean_eof != NULL) *clean_eof = input->CurrentPosition() == start;
    return false;
  }

  // Tell the stream not to read beyond that size.
  google::protobuf::io::CodedInputStream::Limit limit = input->PushLimit(size);

  // Parse the message.
  if (!message->MergeFromCodedStream(input)) return false;
  if (!input->ConsumedEntireMessage()) return false;

  // Release the limit.
  input->PopLimit(limit);

  return true;
}

bool SerializeDelimitedToZeroCopyStream(const MessageLite& message, io::ZeroCopyOutputStream* output) {
  google::protobuf::io::CodedOutputStream coded_output(output);
  return SerializeDelimitedToCodedStream(message, &coded_output);
}

bool SerializeDelimitedToCodedStream(const MessageLite& message, io::CodedOutputStream* output) {
  // Write the size.
  int size = message.ByteSize();
  output->WriteVarint32(size);

  // Write the content.
  uint8* buffer = output->GetDirectBufferForNBytesAndAdvance(size);
  if (buffer != NULL) {
    // Optimization: The message fits in one buffer, so use the faster
    // direct-to-array serialization path.
    message.SerializeWithCachedSizesToArray(buffer);
  } else {
    // Slightly-slower path when the message is multiple buffers.
    message.SerializeWithCachedSizes(output);
    if (output->HadError()) return false;
  }

  return true;
}

}  // namespace util
}  // namespace protobuf
}  // namespace google
