# Relpace the printf by tee_printf in libtprotobuf.a
# Need to implement the tee_printf function in external project
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Dprintf=tee_printf")

set(SGXSSL_INC /opt/intel/sgxssl/include)

# ${CMAKE_CURRENT_SOURCE_DIR}/util/time_util.cc
set(PROTOBUF_SGX_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/any.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/any_lite.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/any.pb.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/api.pb.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/arena.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/arenastring.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/descriptor.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/descriptor_database.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/descriptor.pb.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/duration.pb.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/dynamic_message.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/empty.pb.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/extension_set.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/extension_set_heavy.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/field_mask.pb.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_enum_util.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_message_bases.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_message_reflection.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_message_tctable_full.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_message_tctable_lite.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/generated_message_util.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/implicit_weak_message.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/inlined_string_field.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/io/coded_stream.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/io/gzip_stream.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/io/io_win32.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/io/printer.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/io/strtod.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/io/tokenizer.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/io/zero_copy_stream.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/io/zero_copy_stream_impl.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/io/zero_copy_stream_impl_lite.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/map.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/map_field.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/message.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/message_lite.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/parse_context.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/reflection_ops.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/repeated_field.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/repeated_ptr_field.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/service.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/source_context.pb.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/struct.pb.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/bytestream.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/common.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/int128.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/status.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/statusor.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/stringpiece.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/stringprintf.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/structurally_valid.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/strutil.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/substitute.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/time.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/text_format.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/timestamp.pb.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/type.pb.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/unknown_field_set.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/util/delimited_message_util.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/util/field_comparator.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/util/field_mask_util.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/util/internal/datapiece.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/util/internal/default_value_objectwriter.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/util/internal/error_listener.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/util/internal/field_mask_utility.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/util/internal/json_escaping.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/util/internal/json_objectwriter.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/util/internal/json_stream_parser.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/util/internal/object_writer.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/util/internal/protostream_objectsource.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/util/internal/protostream_objectwriter.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/util/internal/proto_writer.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/util/internal/type_info.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/util/internal/utility.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/util/json_util.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/util/message_differencer.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/util/type_resolver_util.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/wire_format.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/wire_format_lite.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/wrappers.pb.cc
)

include_directories(
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${SGXSSL_INC}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../
)

set(LIB_TPROTOBUF tprotobuf)
add_trusted_library(
  ${LIB_TPROTOBUF}
  SRCS ${PROTOBUF_SGX_SRCS}
  EDL ${CMAKE_CURRENT_SOURCE_DIR}/empty.edl
  EDL_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}
)

install(
    TARGETS ${LIB_TPROTOBUF}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)

