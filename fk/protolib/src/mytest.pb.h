// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: mytest.proto

#ifndef PROTOBUF_mytest_2eproto__INCLUDED
#define PROTOBUF_mytest_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace protobuf_mytest_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[2];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
void InitDefaultsmystery_boxImpl();
void InitDefaultsmystery_box();
void InitDefaultsmystery_box_listImpl();
void InitDefaultsmystery_box_list();
inline void InitDefaults() {
  InitDefaultsmystery_box();
  InitDefaultsmystery_box_list();
}
}  // namespace protobuf_mytest_2eproto
namespace mytest {
class mystery_box;
class mystery_boxDefaultTypeInternal;
extern mystery_boxDefaultTypeInternal _mystery_box_default_instance_;
class mystery_box_list;
class mystery_box_listDefaultTypeInternal;
extern mystery_box_listDefaultTypeInternal _mystery_box_list_default_instance_;
}  // namespace mytest
namespace mytest {

// ===================================================================

class mystery_box : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:mytest.mystery_box) */ {
 public:
  mystery_box();
  virtual ~mystery_box();

  mystery_box(const mystery_box& from);

  inline mystery_box& operator=(const mystery_box& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  mystery_box(mystery_box&& from) noexcept
    : mystery_box() {
    *this = ::std::move(from);
  }

  inline mystery_box& operator=(mystery_box&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const mystery_box& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const mystery_box* internal_default_instance() {
    return reinterpret_cast<const mystery_box*>(
               &_mystery_box_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  void Swap(mystery_box* other);
  friend void swap(mystery_box& a, mystery_box& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline mystery_box* New() const PROTOBUF_FINAL { return New(NULL); }

  mystery_box* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const mystery_box& from);
  void MergeFrom(const mystery_box& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(mystery_box* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional string name_varchar = 6;
  bool has_name_varchar() const;
  void clear_name_varchar();
  static const int kNameVarcharFieldNumber = 6;
  const ::std::string& name_varchar() const;
  void set_name_varchar(const ::std::string& value);
  #if LANG_CXX11
  void set_name_varchar(::std::string&& value);
  #endif
  void set_name_varchar(const char* value);
  void set_name_varchar(const char* value, size_t size);
  ::std::string* mutable_name_varchar();
  ::std::string* release_name_varchar();
  void set_allocated_name_varchar(::std::string* name_varchar);

  // optional string name_blob = 7;
  bool has_name_blob() const;
  void clear_name_blob();
  static const int kNameBlobFieldNumber = 7;
  const ::std::string& name_blob() const;
  void set_name_blob(const ::std::string& value);
  #if LANG_CXX11
  void set_name_blob(::std::string&& value);
  #endif
  void set_name_blob(const char* value);
  void set_name_blob(const char* value, size_t size);
  ::std::string* mutable_name_blob();
  ::std::string* release_name_blob();
  void set_allocated_name_blob(::std::string* name_blob);

  // optional string name_medblob = 8;
  bool has_name_medblob() const;
  void clear_name_medblob();
  static const int kNameMedblobFieldNumber = 8;
  const ::std::string& name_medblob() const;
  void set_name_medblob(const ::std::string& value);
  #if LANG_CXX11
  void set_name_medblob(::std::string&& value);
  #endif
  void set_name_medblob(const char* value);
  void set_name_medblob(const char* value, size_t size);
  ::std::string* mutable_name_medblob();
  ::std::string* release_name_medblob();
  void set_allocated_name_medblob(::std::string* name_medblob);

  // optional int32 name_int = 1;
  bool has_name_int() const;
  void clear_name_int();
  static const int kNameIntFieldNumber = 1;
  ::google::protobuf::int32 name_int() const;
  void set_name_int(::google::protobuf::int32 value);

  // optional int32 name_tiny_int = 2;
  bool has_name_tiny_int() const;
  void clear_name_tiny_int();
  static const int kNameTinyIntFieldNumber = 2;
  ::google::protobuf::int32 name_tiny_int() const;
  void set_name_tiny_int(::google::protobuf::int32 value);

  // optional int64 name_bigint = 4;
  bool has_name_bigint() const;
  void clear_name_bigint();
  static const int kNameBigintFieldNumber = 4;
  ::google::protobuf::int64 name_bigint() const;
  void set_name_bigint(::google::protobuf::int64 value);

  // optional double name_double = 5;
  bool has_name_double() const;
  void clear_name_double();
  static const int kNameDoubleFieldNumber = 5;
  double name_double() const;
  void set_name_double(double value);

  // optional int64 name_date = 9;
  bool has_name_date() const;
  void clear_name_date();
  static const int kNameDateFieldNumber = 9;
  ::google::protobuf::int64 name_date() const;
  void set_name_date(::google::protobuf::int64 value);

  // optional int64 name_time2 = 10;
  bool has_name_time2() const;
  void clear_name_time2();
  static const int kNameTime2FieldNumber = 10;
  ::google::protobuf::int64 name_time2() const;
  void set_name_time2(::google::protobuf::int64 value);

  // optional uint32 name_uint = 3;
  bool has_name_uint() const;
  void clear_name_uint();
  static const int kNameUintFieldNumber = 3;
  ::google::protobuf::uint32 name_uint() const;
  void set_name_uint(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:mytest.mystery_box)
 private:
  void set_has_name_int();
  void clear_has_name_int();
  void set_has_name_tiny_int();
  void clear_has_name_tiny_int();
  void set_has_name_uint();
  void clear_has_name_uint();
  void set_has_name_bigint();
  void clear_has_name_bigint();
  void set_has_name_double();
  void clear_has_name_double();
  void set_has_name_varchar();
  void clear_has_name_varchar();
  void set_has_name_blob();
  void clear_has_name_blob();
  void set_has_name_medblob();
  void clear_has_name_medblob();
  void set_has_name_date();
  void clear_has_name_date();
  void set_has_name_time2();
  void clear_has_name_time2();

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable int _cached_size_;
  ::google::protobuf::internal::ArenaStringPtr name_varchar_;
  ::google::protobuf::internal::ArenaStringPtr name_blob_;
  ::google::protobuf::internal::ArenaStringPtr name_medblob_;
  ::google::protobuf::int32 name_int_;
  ::google::protobuf::int32 name_tiny_int_;
  ::google::protobuf::int64 name_bigint_;
  double name_double_;
  ::google::protobuf::int64 name_date_;
  ::google::protobuf::int64 name_time2_;
  ::google::protobuf::uint32 name_uint_;
  friend struct ::protobuf_mytest_2eproto::TableStruct;
  friend void ::protobuf_mytest_2eproto::InitDefaultsmystery_boxImpl();
};
// -------------------------------------------------------------------

class mystery_box_list : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:mytest.mystery_box_list) */ {
 public:
  mystery_box_list();
  virtual ~mystery_box_list();

  mystery_box_list(const mystery_box_list& from);

  inline mystery_box_list& operator=(const mystery_box_list& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  mystery_box_list(mystery_box_list&& from) noexcept
    : mystery_box_list() {
    *this = ::std::move(from);
  }

  inline mystery_box_list& operator=(mystery_box_list&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const mystery_box_list& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const mystery_box_list* internal_default_instance() {
    return reinterpret_cast<const mystery_box_list*>(
               &_mystery_box_list_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    1;

  void Swap(mystery_box_list* other);
  friend void swap(mystery_box_list& a, mystery_box_list& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline mystery_box_list* New() const PROTOBUF_FINAL { return New(NULL); }

  mystery_box_list* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const mystery_box_list& from);
  void MergeFrom(const mystery_box_list& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(mystery_box_list* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // repeated .mytest.mystery_box items = 1;
  int items_size() const;
  void clear_items();
  static const int kItemsFieldNumber = 1;
  const ::mytest::mystery_box& items(int index) const;
  ::mytest::mystery_box* mutable_items(int index);
  ::mytest::mystery_box* add_items();
  ::google::protobuf::RepeatedPtrField< ::mytest::mystery_box >*
      mutable_items();
  const ::google::protobuf::RepeatedPtrField< ::mytest::mystery_box >&
      items() const;

  // @@protoc_insertion_point(class_scope:mytest.mystery_box_list)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable int _cached_size_;
  ::google::protobuf::RepeatedPtrField< ::mytest::mystery_box > items_;
  friend struct ::protobuf_mytest_2eproto::TableStruct;
  friend void ::protobuf_mytest_2eproto::InitDefaultsmystery_box_listImpl();
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// mystery_box

// optional int32 name_int = 1;
inline bool mystery_box::has_name_int() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void mystery_box::set_has_name_int() {
  _has_bits_[0] |= 0x00000008u;
}
inline void mystery_box::clear_has_name_int() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void mystery_box::clear_name_int() {
  name_int_ = 0;
  clear_has_name_int();
}
inline ::google::protobuf::int32 mystery_box::name_int() const {
  // @@protoc_insertion_point(field_get:mytest.mystery_box.name_int)
  return name_int_;
}
inline void mystery_box::set_name_int(::google::protobuf::int32 value) {
  set_has_name_int();
  name_int_ = value;
  // @@protoc_insertion_point(field_set:mytest.mystery_box.name_int)
}

// optional int32 name_tiny_int = 2;
inline bool mystery_box::has_name_tiny_int() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void mystery_box::set_has_name_tiny_int() {
  _has_bits_[0] |= 0x00000010u;
}
inline void mystery_box::clear_has_name_tiny_int() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void mystery_box::clear_name_tiny_int() {
  name_tiny_int_ = 0;
  clear_has_name_tiny_int();
}
inline ::google::protobuf::int32 mystery_box::name_tiny_int() const {
  // @@protoc_insertion_point(field_get:mytest.mystery_box.name_tiny_int)
  return name_tiny_int_;
}
inline void mystery_box::set_name_tiny_int(::google::protobuf::int32 value) {
  set_has_name_tiny_int();
  name_tiny_int_ = value;
  // @@protoc_insertion_point(field_set:mytest.mystery_box.name_tiny_int)
}

// optional uint32 name_uint = 3;
inline bool mystery_box::has_name_uint() const {
  return (_has_bits_[0] & 0x00000200u) != 0;
}
inline void mystery_box::set_has_name_uint() {
  _has_bits_[0] |= 0x00000200u;
}
inline void mystery_box::clear_has_name_uint() {
  _has_bits_[0] &= ~0x00000200u;
}
inline void mystery_box::clear_name_uint() {
  name_uint_ = 0u;
  clear_has_name_uint();
}
inline ::google::protobuf::uint32 mystery_box::name_uint() const {
  // @@protoc_insertion_point(field_get:mytest.mystery_box.name_uint)
  return name_uint_;
}
inline void mystery_box::set_name_uint(::google::protobuf::uint32 value) {
  set_has_name_uint();
  name_uint_ = value;
  // @@protoc_insertion_point(field_set:mytest.mystery_box.name_uint)
}

// optional int64 name_bigint = 4;
inline bool mystery_box::has_name_bigint() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void mystery_box::set_has_name_bigint() {
  _has_bits_[0] |= 0x00000020u;
}
inline void mystery_box::clear_has_name_bigint() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void mystery_box::clear_name_bigint() {
  name_bigint_ = GOOGLE_LONGLONG(0);
  clear_has_name_bigint();
}
inline ::google::protobuf::int64 mystery_box::name_bigint() const {
  // @@protoc_insertion_point(field_get:mytest.mystery_box.name_bigint)
  return name_bigint_;
}
inline void mystery_box::set_name_bigint(::google::protobuf::int64 value) {
  set_has_name_bigint();
  name_bigint_ = value;
  // @@protoc_insertion_point(field_set:mytest.mystery_box.name_bigint)
}

// optional double name_double = 5;
inline bool mystery_box::has_name_double() const {
  return (_has_bits_[0] & 0x00000040u) != 0;
}
inline void mystery_box::set_has_name_double() {
  _has_bits_[0] |= 0x00000040u;
}
inline void mystery_box::clear_has_name_double() {
  _has_bits_[0] &= ~0x00000040u;
}
inline void mystery_box::clear_name_double() {
  name_double_ = 0;
  clear_has_name_double();
}
inline double mystery_box::name_double() const {
  // @@protoc_insertion_point(field_get:mytest.mystery_box.name_double)
  return name_double_;
}
inline void mystery_box::set_name_double(double value) {
  set_has_name_double();
  name_double_ = value;
  // @@protoc_insertion_point(field_set:mytest.mystery_box.name_double)
}

// optional string name_varchar = 6;
inline bool mystery_box::has_name_varchar() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void mystery_box::set_has_name_varchar() {
  _has_bits_[0] |= 0x00000001u;
}
inline void mystery_box::clear_has_name_varchar() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void mystery_box::clear_name_varchar() {
  name_varchar_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_name_varchar();
}
inline const ::std::string& mystery_box::name_varchar() const {
  // @@protoc_insertion_point(field_get:mytest.mystery_box.name_varchar)
  return name_varchar_.GetNoArena();
}
inline void mystery_box::set_name_varchar(const ::std::string& value) {
  set_has_name_varchar();
  name_varchar_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:mytest.mystery_box.name_varchar)
}
#if LANG_CXX11
inline void mystery_box::set_name_varchar(::std::string&& value) {
  set_has_name_varchar();
  name_varchar_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:mytest.mystery_box.name_varchar)
}
#endif
inline void mystery_box::set_name_varchar(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  set_has_name_varchar();
  name_varchar_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:mytest.mystery_box.name_varchar)
}
inline void mystery_box::set_name_varchar(const char* value, size_t size) {
  set_has_name_varchar();
  name_varchar_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:mytest.mystery_box.name_varchar)
}
inline ::std::string* mystery_box::mutable_name_varchar() {
  set_has_name_varchar();
  // @@protoc_insertion_point(field_mutable:mytest.mystery_box.name_varchar)
  return name_varchar_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* mystery_box::release_name_varchar() {
  // @@protoc_insertion_point(field_release:mytest.mystery_box.name_varchar)
  clear_has_name_varchar();
  return name_varchar_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void mystery_box::set_allocated_name_varchar(::std::string* name_varchar) {
  if (name_varchar != NULL) {
    set_has_name_varchar();
  } else {
    clear_has_name_varchar();
  }
  name_varchar_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), name_varchar);
  // @@protoc_insertion_point(field_set_allocated:mytest.mystery_box.name_varchar)
}

// optional string name_blob = 7;
inline bool mystery_box::has_name_blob() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void mystery_box::set_has_name_blob() {
  _has_bits_[0] |= 0x00000002u;
}
inline void mystery_box::clear_has_name_blob() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void mystery_box::clear_name_blob() {
  name_blob_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_name_blob();
}
inline const ::std::string& mystery_box::name_blob() const {
  // @@protoc_insertion_point(field_get:mytest.mystery_box.name_blob)
  return name_blob_.GetNoArena();
}
inline void mystery_box::set_name_blob(const ::std::string& value) {
  set_has_name_blob();
  name_blob_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:mytest.mystery_box.name_blob)
}
#if LANG_CXX11
inline void mystery_box::set_name_blob(::std::string&& value) {
  set_has_name_blob();
  name_blob_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:mytest.mystery_box.name_blob)
}
#endif
inline void mystery_box::set_name_blob(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  set_has_name_blob();
  name_blob_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:mytest.mystery_box.name_blob)
}
inline void mystery_box::set_name_blob(const char* value, size_t size) {
  set_has_name_blob();
  name_blob_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:mytest.mystery_box.name_blob)
}
inline ::std::string* mystery_box::mutable_name_blob() {
  set_has_name_blob();
  // @@protoc_insertion_point(field_mutable:mytest.mystery_box.name_blob)
  return name_blob_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* mystery_box::release_name_blob() {
  // @@protoc_insertion_point(field_release:mytest.mystery_box.name_blob)
  clear_has_name_blob();
  return name_blob_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void mystery_box::set_allocated_name_blob(::std::string* name_blob) {
  if (name_blob != NULL) {
    set_has_name_blob();
  } else {
    clear_has_name_blob();
  }
  name_blob_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), name_blob);
  // @@protoc_insertion_point(field_set_allocated:mytest.mystery_box.name_blob)
}

// optional string name_medblob = 8;
inline bool mystery_box::has_name_medblob() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void mystery_box::set_has_name_medblob() {
  _has_bits_[0] |= 0x00000004u;
}
inline void mystery_box::clear_has_name_medblob() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void mystery_box::clear_name_medblob() {
  name_medblob_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_name_medblob();
}
inline const ::std::string& mystery_box::name_medblob() const {
  // @@protoc_insertion_point(field_get:mytest.mystery_box.name_medblob)
  return name_medblob_.GetNoArena();
}
inline void mystery_box::set_name_medblob(const ::std::string& value) {
  set_has_name_medblob();
  name_medblob_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:mytest.mystery_box.name_medblob)
}
#if LANG_CXX11
inline void mystery_box::set_name_medblob(::std::string&& value) {
  set_has_name_medblob();
  name_medblob_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:mytest.mystery_box.name_medblob)
}
#endif
inline void mystery_box::set_name_medblob(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  set_has_name_medblob();
  name_medblob_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:mytest.mystery_box.name_medblob)
}
inline void mystery_box::set_name_medblob(const char* value, size_t size) {
  set_has_name_medblob();
  name_medblob_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:mytest.mystery_box.name_medblob)
}
inline ::std::string* mystery_box::mutable_name_medblob() {
  set_has_name_medblob();
  // @@protoc_insertion_point(field_mutable:mytest.mystery_box.name_medblob)
  return name_medblob_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* mystery_box::release_name_medblob() {
  // @@protoc_insertion_point(field_release:mytest.mystery_box.name_medblob)
  clear_has_name_medblob();
  return name_medblob_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void mystery_box::set_allocated_name_medblob(::std::string* name_medblob) {
  if (name_medblob != NULL) {
    set_has_name_medblob();
  } else {
    clear_has_name_medblob();
  }
  name_medblob_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), name_medblob);
  // @@protoc_insertion_point(field_set_allocated:mytest.mystery_box.name_medblob)
}

// optional int64 name_date = 9;
inline bool mystery_box::has_name_date() const {
  return (_has_bits_[0] & 0x00000080u) != 0;
}
inline void mystery_box::set_has_name_date() {
  _has_bits_[0] |= 0x00000080u;
}
inline void mystery_box::clear_has_name_date() {
  _has_bits_[0] &= ~0x00000080u;
}
inline void mystery_box::clear_name_date() {
  name_date_ = GOOGLE_LONGLONG(0);
  clear_has_name_date();
}
inline ::google::protobuf::int64 mystery_box::name_date() const {
  // @@protoc_insertion_point(field_get:mytest.mystery_box.name_date)
  return name_date_;
}
inline void mystery_box::set_name_date(::google::protobuf::int64 value) {
  set_has_name_date();
  name_date_ = value;
  // @@protoc_insertion_point(field_set:mytest.mystery_box.name_date)
}

// optional int64 name_time2 = 10;
inline bool mystery_box::has_name_time2() const {
  return (_has_bits_[0] & 0x00000100u) != 0;
}
inline void mystery_box::set_has_name_time2() {
  _has_bits_[0] |= 0x00000100u;
}
inline void mystery_box::clear_has_name_time2() {
  _has_bits_[0] &= ~0x00000100u;
}
inline void mystery_box::clear_name_time2() {
  name_time2_ = GOOGLE_LONGLONG(0);
  clear_has_name_time2();
}
inline ::google::protobuf::int64 mystery_box::name_time2() const {
  // @@protoc_insertion_point(field_get:mytest.mystery_box.name_time2)
  return name_time2_;
}
inline void mystery_box::set_name_time2(::google::protobuf::int64 value) {
  set_has_name_time2();
  name_time2_ = value;
  // @@protoc_insertion_point(field_set:mytest.mystery_box.name_time2)
}

// -------------------------------------------------------------------

// mystery_box_list

// repeated .mytest.mystery_box items = 1;
inline int mystery_box_list::items_size() const {
  return items_.size();
}
inline void mystery_box_list::clear_items() {
  items_.Clear();
}
inline const ::mytest::mystery_box& mystery_box_list::items(int index) const {
  // @@protoc_insertion_point(field_get:mytest.mystery_box_list.items)
  return items_.Get(index);
}
inline ::mytest::mystery_box* mystery_box_list::mutable_items(int index) {
  // @@protoc_insertion_point(field_mutable:mytest.mystery_box_list.items)
  return items_.Mutable(index);
}
inline ::mytest::mystery_box* mystery_box_list::add_items() {
  // @@protoc_insertion_point(field_add:mytest.mystery_box_list.items)
  return items_.Add();
}
inline ::google::protobuf::RepeatedPtrField< ::mytest::mystery_box >*
mystery_box_list::mutable_items() {
  // @@protoc_insertion_point(field_mutable_list:mytest.mystery_box_list.items)
  return &items_;
}
inline const ::google::protobuf::RepeatedPtrField< ::mytest::mystery_box >&
mystery_box_list::items() const {
  // @@protoc_insertion_point(field_list:mytest.mystery_box_list.items)
  return items_;
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace mytest

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_mytest_2eproto__INCLUDED
