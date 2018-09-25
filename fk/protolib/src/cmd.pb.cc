// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: cmd.proto

#include "cmd.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// This is a temporary google only hack
#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
#include "third_party/protobuf/version.h"
#endif
// @@protoc_insertion_point(includes)
namespace proto {
class CMDDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<CMD>
      _instance;
} _CMD_default_instance_;
}  // namespace proto
namespace protobuf_cmd_2eproto {
void InitDefaultsCMDImpl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
  ::google::protobuf::internal::InitProtobufDefaultsForceUnique();
#else
  ::google::protobuf::internal::InitProtobufDefaults();
#endif  // GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
  {
    void* ptr = &::proto::_CMD_default_instance_;
    new (ptr) ::proto::CMD();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::proto::CMD::InitAsDefaultInstance();
}

void InitDefaultsCMD() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &InitDefaultsCMDImpl);
}

::google::protobuf::Metadata file_level_metadata[1];
const ::google::protobuf::EnumDescriptor* file_level_enum_descriptors[1];

const ::google::protobuf::uint32 TableStruct::offsets[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::proto::CMD, _has_bits_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::proto::CMD, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
};
static const ::google::protobuf::internal::MigrationSchema schemas[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  { 0, 5, sizeof(::proto::CMD)},
};

static ::google::protobuf::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::google::protobuf::Message*>(&::proto::_CMD_default_instance_),
};

void protobuf_AssignDescriptors() {
  AddDescriptors();
  ::google::protobuf::MessageFactory* factory = NULL;
  AssignDescriptors(
      "cmd.proto", schemas, file_default_instances, TableStruct::offsets, factory,
      file_level_metadata, file_level_enum_descriptors, NULL);
}

void protobuf_AssignDescriptorsOnce() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &protobuf_AssignDescriptors);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_PROTOBUF_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::internal::RegisterAllTypes(file_level_metadata, 1);
}

void AddDescriptorsImpl() {
  InitDefaults();
  static const char descriptor[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
      "\n\tcmd.proto\022\005proto\"\226\001\n\003CMD\"\216\001\n\007SvrBase\022\030"
      "\n\024CMD_SOCKET_CLIENT_IN\020\001\022\031\n\025CMD_SOCKET_C"
      "LIENT_OUT\020\002\022\033\n\027CMD_REGISTER_SERVER_REQ\020\003"
      "\022\033\n\027CMD_REGISTER_SERVER_RSP\020\004\022\024\n\020CMD_SVR"
      "_HEATBEAT\020\005"
  };
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
      descriptor, 171);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "cmd.proto", &protobuf_RegisterTypes);
}

void AddDescriptors() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &AddDescriptorsImpl);
}
// Force AddDescriptors() to be called at dynamic initialization time.
struct StaticDescriptorInitializer {
  StaticDescriptorInitializer() {
    AddDescriptors();
  }
} static_descriptor_initializer;
}  // namespace protobuf_cmd_2eproto
namespace proto {
const ::google::protobuf::EnumDescriptor* CMD_SvrBase_descriptor() {
  protobuf_cmd_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_cmd_2eproto::file_level_enum_descriptors[0];
}
bool CMD_SvrBase_IsValid(int value) {
  switch (value) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
      return true;
    default:
      return false;
  }
}

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const CMD_SvrBase CMD::CMD_SOCKET_CLIENT_IN;
const CMD_SvrBase CMD::CMD_SOCKET_CLIENT_OUT;
const CMD_SvrBase CMD::CMD_REGISTER_SERVER_REQ;
const CMD_SvrBase CMD::CMD_REGISTER_SERVER_RSP;
const CMD_SvrBase CMD::CMD_SVR_HEATBEAT;
const CMD_SvrBase CMD::SvrBase_MIN;
const CMD_SvrBase CMD::SvrBase_MAX;
const int CMD::SvrBase_ARRAYSIZE;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

// ===================================================================

void CMD::InitAsDefaultInstance() {
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

CMD::CMD()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (GOOGLE_PREDICT_TRUE(this != internal_default_instance())) {
    ::protobuf_cmd_2eproto::InitDefaultsCMD();
  }
  SharedCtor();
  // @@protoc_insertion_point(constructor:proto.CMD)
}
CMD::CMD(const CMD& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(NULL),
      _has_bits_(from._has_bits_),
      _cached_size_(0) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:proto.CMD)
}

void CMD::SharedCtor() {
  _cached_size_ = 0;
}

CMD::~CMD() {
  // @@protoc_insertion_point(destructor:proto.CMD)
  SharedDtor();
}

void CMD::SharedDtor() {
}

void CMD::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* CMD::descriptor() {
  ::protobuf_cmd_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_cmd_2eproto::file_level_metadata[kIndexInFileMessages].descriptor;
}

const CMD& CMD::default_instance() {
  ::protobuf_cmd_2eproto::InitDefaultsCMD();
  return *internal_default_instance();
}

CMD* CMD::New(::google::protobuf::Arena* arena) const {
  CMD* n = new CMD;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void CMD::Clear() {
// @@protoc_insertion_point(message_clear_start:proto.CMD)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _has_bits_.Clear();
  _internal_metadata_.Clear();
}

bool CMD::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:proto.CMD)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
  handle_unusual:
    if (tag == 0) {
      goto success;
    }
    DO_(::google::protobuf::internal::WireFormat::SkipField(
          input, tag, _internal_metadata_.mutable_unknown_fields()));
  }
success:
  // @@protoc_insertion_point(parse_success:proto.CMD)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:proto.CMD)
  return false;
#undef DO_
}

void CMD::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:proto.CMD)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (_internal_metadata_.have_unknown_fields()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        _internal_metadata_.unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:proto.CMD)
}

::google::protobuf::uint8* CMD::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:proto.CMD)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (_internal_metadata_.have_unknown_fields()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:proto.CMD)
  return target;
}

size_t CMD::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:proto.CMD)
  size_t total_size = 0;

  if (_internal_metadata_.have_unknown_fields()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        _internal_metadata_.unknown_fields());
  }
  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void CMD::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:proto.CMD)
  GOOGLE_DCHECK_NE(&from, this);
  const CMD* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const CMD>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:proto.CMD)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:proto.CMD)
    MergeFrom(*source);
  }
}

void CMD::MergeFrom(const CMD& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:proto.CMD)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

}

void CMD::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:proto.CMD)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void CMD::CopyFrom(const CMD& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:proto.CMD)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool CMD::IsInitialized() const {
  return true;
}

void CMD::Swap(CMD* other) {
  if (other == this) return;
  InternalSwap(other);
}
void CMD::InternalSwap(CMD* other) {
  using std::swap;
  swap(_has_bits_[0], other->_has_bits_[0]);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata CMD::GetMetadata() const {
  protobuf_cmd_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_cmd_2eproto::file_level_metadata[kIndexInFileMessages];
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace proto

// @@protoc_insertion_point(global_scope)
