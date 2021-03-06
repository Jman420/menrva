// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Engine_SetLogLevel.proto

#include "Engine_SetLogLevel.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
namespace messages {
class Engine_SetLogLevel_RequestDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<Engine_SetLogLevel_Request> _instance;
} _Engine_SetLogLevel_Request_default_instance_;
class Engine_SetLogLevel_ResponseDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<Engine_SetLogLevel_Response> _instance;
} _Engine_SetLogLevel_Response_default_instance_;
}  // namespace messages
static void InitDefaultsscc_info_Engine_SetLogLevel_Request_Engine_5fSetLogLevel_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::messages::_Engine_SetLogLevel_Request_default_instance_;
    new (ptr) ::messages::Engine_SetLogLevel_Request();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::messages::Engine_SetLogLevel_Request::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_Engine_SetLogLevel_Request_Engine_5fSetLogLevel_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_Engine_SetLogLevel_Request_Engine_5fSetLogLevel_2eproto}, {}};

static void InitDefaultsscc_info_Engine_SetLogLevel_Response_Engine_5fSetLogLevel_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::messages::_Engine_SetLogLevel_Response_default_instance_;
    new (ptr) ::messages::Engine_SetLogLevel_Response();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::messages::Engine_SetLogLevel_Response::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_Engine_SetLogLevel_Response_Engine_5fSetLogLevel_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_Engine_SetLogLevel_Response_Engine_5fSetLogLevel_2eproto}, {}};

namespace messages {

// ===================================================================

void Engine_SetLogLevel_Request::InitAsDefaultInstance() {
}
class Engine_SetLogLevel_Request::_Internal {
 public:
};

Engine_SetLogLevel_Request::Engine_SetLogLevel_Request()
  : ::PROTOBUF_NAMESPACE_ID::MessageLite(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:messages.Engine_SetLogLevel_Request)
}
Engine_SetLogLevel_Request::Engine_SetLogLevel_Request(const Engine_SetLogLevel_Request& from)
  : ::PROTOBUF_NAMESPACE_ID::MessageLite(),
      _internal_metadata_(nullptr) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  loglevel_ = from.loglevel_;
  // @@protoc_insertion_point(copy_constructor:messages.Engine_SetLogLevel_Request)
}

void Engine_SetLogLevel_Request::SharedCtor() {
  loglevel_ = 0;
}

Engine_SetLogLevel_Request::~Engine_SetLogLevel_Request() {
  // @@protoc_insertion_point(destructor:messages.Engine_SetLogLevel_Request)
  SharedDtor();
}

void Engine_SetLogLevel_Request::SharedDtor() {
}

void Engine_SetLogLevel_Request::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const Engine_SetLogLevel_Request& Engine_SetLogLevel_Request::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_Engine_SetLogLevel_Request_Engine_5fSetLogLevel_2eproto.base);
  return *internal_default_instance();
}


void Engine_SetLogLevel_Request::Clear() {
// @@protoc_insertion_point(message_clear_start:messages.Engine_SetLogLevel_Request)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  loglevel_ = 0;
  _internal_metadata_.Clear();
}

const char* Engine_SetLogLevel_Request::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // int32 logLevel = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          loglevel_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag, &_internal_metadata_, ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* Engine_SetLogLevel_Request::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:messages.Engine_SetLogLevel_Request)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 logLevel = 1;
  if (this->loglevel() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(1, this->_internal_loglevel(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = stream->WriteRaw(_internal_metadata_.unknown_fields().data(),
        static_cast<int>(_internal_metadata_.unknown_fields().size()), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:messages.Engine_SetLogLevel_Request)
  return target;
}

size_t Engine_SetLogLevel_Request::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:messages.Engine_SetLogLevel_Request)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // int32 logLevel = 1;
  if (this->loglevel() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_loglevel());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    total_size += _internal_metadata_.unknown_fields().size();
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void Engine_SetLogLevel_Request::CheckTypeAndMergeFrom(
    const ::PROTOBUF_NAMESPACE_ID::MessageLite& from) {
  MergeFrom(*::PROTOBUF_NAMESPACE_ID::internal::DownCast<const Engine_SetLogLevel_Request*>(
      &from));
}

void Engine_SetLogLevel_Request::MergeFrom(const Engine_SetLogLevel_Request& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:messages.Engine_SetLogLevel_Request)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.loglevel() != 0) {
    _internal_set_loglevel(from._internal_loglevel());
  }
}

void Engine_SetLogLevel_Request::CopyFrom(const Engine_SetLogLevel_Request& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:messages.Engine_SetLogLevel_Request)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Engine_SetLogLevel_Request::IsInitialized() const {
  return true;
}

void Engine_SetLogLevel_Request::InternalSwap(Engine_SetLogLevel_Request* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(loglevel_, other->loglevel_);
}

std::string Engine_SetLogLevel_Request::GetTypeName() const {
  return "messages.Engine_SetLogLevel_Request";
}


// ===================================================================

void Engine_SetLogLevel_Response::InitAsDefaultInstance() {
}
class Engine_SetLogLevel_Response::_Internal {
 public:
};

Engine_SetLogLevel_Response::Engine_SetLogLevel_Response()
  : ::PROTOBUF_NAMESPACE_ID::MessageLite(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:messages.Engine_SetLogLevel_Response)
}
Engine_SetLogLevel_Response::Engine_SetLogLevel_Response(const Engine_SetLogLevel_Response& from)
  : ::PROTOBUF_NAMESPACE_ID::MessageLite(),
      _internal_metadata_(nullptr) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::memcpy(&success_, &from.success_,
    static_cast<size_t>(reinterpret_cast<char*>(&loglevel_) -
    reinterpret_cast<char*>(&success_)) + sizeof(loglevel_));
  // @@protoc_insertion_point(copy_constructor:messages.Engine_SetLogLevel_Response)
}

void Engine_SetLogLevel_Response::SharedCtor() {
  ::memset(&success_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&loglevel_) -
      reinterpret_cast<char*>(&success_)) + sizeof(loglevel_));
}

Engine_SetLogLevel_Response::~Engine_SetLogLevel_Response() {
  // @@protoc_insertion_point(destructor:messages.Engine_SetLogLevel_Response)
  SharedDtor();
}

void Engine_SetLogLevel_Response::SharedDtor() {
}

void Engine_SetLogLevel_Response::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const Engine_SetLogLevel_Response& Engine_SetLogLevel_Response::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_Engine_SetLogLevel_Response_Engine_5fSetLogLevel_2eproto.base);
  return *internal_default_instance();
}


void Engine_SetLogLevel_Response::Clear() {
// @@protoc_insertion_point(message_clear_start:messages.Engine_SetLogLevel_Response)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  ::memset(&success_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&loglevel_) -
      reinterpret_cast<char*>(&success_)) + sizeof(loglevel_));
  _internal_metadata_.Clear();
}

const char* Engine_SetLogLevel_Response::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // bool success = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          success_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int32 logLevel = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          loglevel_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag, &_internal_metadata_, ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* Engine_SetLogLevel_Response::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:messages.Engine_SetLogLevel_Response)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // bool success = 1;
  if (this->success() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteBoolToArray(1, this->_internal_success(), target);
  }

  // int32 logLevel = 2;
  if (this->loglevel() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(2, this->_internal_loglevel(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = stream->WriteRaw(_internal_metadata_.unknown_fields().data(),
        static_cast<int>(_internal_metadata_.unknown_fields().size()), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:messages.Engine_SetLogLevel_Response)
  return target;
}

size_t Engine_SetLogLevel_Response::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:messages.Engine_SetLogLevel_Response)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // bool success = 1;
  if (this->success() != 0) {
    total_size += 1 + 1;
  }

  // int32 logLevel = 2;
  if (this->loglevel() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_loglevel());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    total_size += _internal_metadata_.unknown_fields().size();
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void Engine_SetLogLevel_Response::CheckTypeAndMergeFrom(
    const ::PROTOBUF_NAMESPACE_ID::MessageLite& from) {
  MergeFrom(*::PROTOBUF_NAMESPACE_ID::internal::DownCast<const Engine_SetLogLevel_Response*>(
      &from));
}

void Engine_SetLogLevel_Response::MergeFrom(const Engine_SetLogLevel_Response& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:messages.Engine_SetLogLevel_Response)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.success() != 0) {
    _internal_set_success(from._internal_success());
  }
  if (from.loglevel() != 0) {
    _internal_set_loglevel(from._internal_loglevel());
  }
}

void Engine_SetLogLevel_Response::CopyFrom(const Engine_SetLogLevel_Response& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:messages.Engine_SetLogLevel_Response)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Engine_SetLogLevel_Response::IsInitialized() const {
  return true;
}

void Engine_SetLogLevel_Response::InternalSwap(Engine_SetLogLevel_Response* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(success_, other->success_);
  swap(loglevel_, other->loglevel_);
}

std::string Engine_SetLogLevel_Response::GetTypeName() const {
  return "messages.Engine_SetLogLevel_Response";
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace messages
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::messages::Engine_SetLogLevel_Request* Arena::CreateMaybeMessage< ::messages::Engine_SetLogLevel_Request >(Arena* arena) {
  return Arena::CreateInternal< ::messages::Engine_SetLogLevel_Request >(arena);
}
template<> PROTOBUF_NOINLINE ::messages::Engine_SetLogLevel_Response* Arena::CreateMaybeMessage< ::messages::Engine_SetLogLevel_Response >(Arena* arena) {
  return Arena::CreateInternal< ::messages::Engine_SetLogLevel_Response >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
