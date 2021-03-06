// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Engine_SetLogLevel.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_Engine_5fSetLogLevel_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_Engine_5fSetLogLevel_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3011000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3011004 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/message_lite.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_Engine_5fSetLogLevel_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_Engine_5fSetLogLevel_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[2]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
namespace messages {
class Engine_SetLogLevel_Request;
class Engine_SetLogLevel_RequestDefaultTypeInternal;
extern Engine_SetLogLevel_RequestDefaultTypeInternal _Engine_SetLogLevel_Request_default_instance_;
class Engine_SetLogLevel_Response;
class Engine_SetLogLevel_ResponseDefaultTypeInternal;
extern Engine_SetLogLevel_ResponseDefaultTypeInternal _Engine_SetLogLevel_Response_default_instance_;
}  // namespace messages
PROTOBUF_NAMESPACE_OPEN
template<> ::messages::Engine_SetLogLevel_Request* Arena::CreateMaybeMessage<::messages::Engine_SetLogLevel_Request>(Arena*);
template<> ::messages::Engine_SetLogLevel_Response* Arena::CreateMaybeMessage<::messages::Engine_SetLogLevel_Response>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace messages {

// ===================================================================

class Engine_SetLogLevel_Request :
    public ::PROTOBUF_NAMESPACE_ID::MessageLite /* @@protoc_insertion_point(class_definition:messages.Engine_SetLogLevel_Request) */ {
 public:
  Engine_SetLogLevel_Request();
  virtual ~Engine_SetLogLevel_Request();

  Engine_SetLogLevel_Request(const Engine_SetLogLevel_Request& from);
  Engine_SetLogLevel_Request(Engine_SetLogLevel_Request&& from) noexcept
    : Engine_SetLogLevel_Request() {
    *this = ::std::move(from);
  }

  inline Engine_SetLogLevel_Request& operator=(const Engine_SetLogLevel_Request& from) {
    CopyFrom(from);
    return *this;
  }
  inline Engine_SetLogLevel_Request& operator=(Engine_SetLogLevel_Request&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const Engine_SetLogLevel_Request& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Engine_SetLogLevel_Request* internal_default_instance() {
    return reinterpret_cast<const Engine_SetLogLevel_Request*>(
               &_Engine_SetLogLevel_Request_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(Engine_SetLogLevel_Request& a, Engine_SetLogLevel_Request& b) {
    a.Swap(&b);
  }
  inline void Swap(Engine_SetLogLevel_Request* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline Engine_SetLogLevel_Request* New() const final {
    return CreateMaybeMessage<Engine_SetLogLevel_Request>(nullptr);
  }

  Engine_SetLogLevel_Request* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<Engine_SetLogLevel_Request>(arena);
  }
  void CheckTypeAndMergeFrom(const ::PROTOBUF_NAMESPACE_ID::MessageLite& from)
    final;
  void CopyFrom(const Engine_SetLogLevel_Request& from);
  void MergeFrom(const Engine_SetLogLevel_Request& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  void DiscardUnknownFields();
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(Engine_SetLogLevel_Request* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "messages.Engine_SetLogLevel_Request";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  std::string GetTypeName() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kLogLevelFieldNumber = 1,
  };
  // int32 logLevel = 1;
  void clear_loglevel();
  ::PROTOBUF_NAMESPACE_ID::int32 loglevel() const;
  void set_loglevel(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_loglevel() const;
  void _internal_set_loglevel(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // @@protoc_insertion_point(class_scope:messages.Engine_SetLogLevel_Request)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArenaLite _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::int32 loglevel_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_Engine_5fSetLogLevel_2eproto;
};
// -------------------------------------------------------------------

class Engine_SetLogLevel_Response :
    public ::PROTOBUF_NAMESPACE_ID::MessageLite /* @@protoc_insertion_point(class_definition:messages.Engine_SetLogLevel_Response) */ {
 public:
  Engine_SetLogLevel_Response();
  virtual ~Engine_SetLogLevel_Response();

  Engine_SetLogLevel_Response(const Engine_SetLogLevel_Response& from);
  Engine_SetLogLevel_Response(Engine_SetLogLevel_Response&& from) noexcept
    : Engine_SetLogLevel_Response() {
    *this = ::std::move(from);
  }

  inline Engine_SetLogLevel_Response& operator=(const Engine_SetLogLevel_Response& from) {
    CopyFrom(from);
    return *this;
  }
  inline Engine_SetLogLevel_Response& operator=(Engine_SetLogLevel_Response&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const Engine_SetLogLevel_Response& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Engine_SetLogLevel_Response* internal_default_instance() {
    return reinterpret_cast<const Engine_SetLogLevel_Response*>(
               &_Engine_SetLogLevel_Response_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(Engine_SetLogLevel_Response& a, Engine_SetLogLevel_Response& b) {
    a.Swap(&b);
  }
  inline void Swap(Engine_SetLogLevel_Response* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline Engine_SetLogLevel_Response* New() const final {
    return CreateMaybeMessage<Engine_SetLogLevel_Response>(nullptr);
  }

  Engine_SetLogLevel_Response* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<Engine_SetLogLevel_Response>(arena);
  }
  void CheckTypeAndMergeFrom(const ::PROTOBUF_NAMESPACE_ID::MessageLite& from)
    final;
  void CopyFrom(const Engine_SetLogLevel_Response& from);
  void MergeFrom(const Engine_SetLogLevel_Response& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  void DiscardUnknownFields();
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(Engine_SetLogLevel_Response* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "messages.Engine_SetLogLevel_Response";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  std::string GetTypeName() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kSuccessFieldNumber = 1,
    kLogLevelFieldNumber = 2,
  };
  // bool success = 1;
  void clear_success();
  bool success() const;
  void set_success(bool value);
  private:
  bool _internal_success() const;
  void _internal_set_success(bool value);
  public:

  // int32 logLevel = 2;
  void clear_loglevel();
  ::PROTOBUF_NAMESPACE_ID::int32 loglevel() const;
  void set_loglevel(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_loglevel() const;
  void _internal_set_loglevel(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // @@protoc_insertion_point(class_scope:messages.Engine_SetLogLevel_Response)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArenaLite _internal_metadata_;
  bool success_;
  ::PROTOBUF_NAMESPACE_ID::int32 loglevel_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_Engine_5fSetLogLevel_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Engine_SetLogLevel_Request

// int32 logLevel = 1;
inline void Engine_SetLogLevel_Request::clear_loglevel() {
  loglevel_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Engine_SetLogLevel_Request::_internal_loglevel() const {
  return loglevel_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Engine_SetLogLevel_Request::loglevel() const {
  // @@protoc_insertion_point(field_get:messages.Engine_SetLogLevel_Request.logLevel)
  return _internal_loglevel();
}
inline void Engine_SetLogLevel_Request::_internal_set_loglevel(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  loglevel_ = value;
}
inline void Engine_SetLogLevel_Request::set_loglevel(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_loglevel(value);
  // @@protoc_insertion_point(field_set:messages.Engine_SetLogLevel_Request.logLevel)
}

// -------------------------------------------------------------------

// Engine_SetLogLevel_Response

// bool success = 1;
inline void Engine_SetLogLevel_Response::clear_success() {
  success_ = false;
}
inline bool Engine_SetLogLevel_Response::_internal_success() const {
  return success_;
}
inline bool Engine_SetLogLevel_Response::success() const {
  // @@protoc_insertion_point(field_get:messages.Engine_SetLogLevel_Response.success)
  return _internal_success();
}
inline void Engine_SetLogLevel_Response::_internal_set_success(bool value) {
  
  success_ = value;
}
inline void Engine_SetLogLevel_Response::set_success(bool value) {
  _internal_set_success(value);
  // @@protoc_insertion_point(field_set:messages.Engine_SetLogLevel_Response.success)
}

// int32 logLevel = 2;
inline void Engine_SetLogLevel_Response::clear_loglevel() {
  loglevel_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Engine_SetLogLevel_Response::_internal_loglevel() const {
  return loglevel_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 Engine_SetLogLevel_Response::loglevel() const {
  // @@protoc_insertion_point(field_get:messages.Engine_SetLogLevel_Response.logLevel)
  return _internal_loglevel();
}
inline void Engine_SetLogLevel_Response::_internal_set_loglevel(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  loglevel_ = value;
}
inline void Engine_SetLogLevel_Response::set_loglevel(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_loglevel(value);
  // @@protoc_insertion_point(field_set:messages.Engine_SetLogLevel_Response.logLevel)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace messages

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_Engine_5fSetLogLevel_2eproto
