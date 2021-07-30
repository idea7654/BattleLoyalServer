// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_UDPPROTOCOL_H_
#define FLATBUFFERS_GENERATED_UDPPROTOCOL_H_

#include "flatbuffers/flatbuffers.h"

struct Vec3;

struct Message;
struct MessageBuilder;

struct S2C_MOVE;
struct S2C_MOVEBuilder;

struct S2C_SHOOT;
struct S2C_SHOOTBuilder;

struct C2S_MOVE;
struct C2S_MOVEBuilder;

struct C2S_EXTEND_SESSION;
struct C2S_EXTEND_SESSIONBuilder;

struct C2S_REQUEST_LOGIN;
struct C2S_REQUEST_LOGINBuilder;

struct C2S_REQUEST_REGISTER;
struct C2S_REQUEST_REGISTERBuilder;

struct S2C_COMPLETE_LOGIN;
struct S2C_COMPLETE_LOGINBuilder;

struct S2C_COMPLETE_REGISTER;
struct S2C_COMPLETE_REGISTERBuilder;

struct S2C_LOGIN_ERROR;
struct S2C_LOGIN_ERRORBuilder;

struct S2C_REGISTER_ERROR;
struct S2C_REGISTER_ERRORBuilder;

enum MESSAGE_ID : uint8_t {
  MESSAGE_ID_NONE = 0,
  MESSAGE_ID_S2C_MOVE = 1,
  MESSAGE_ID_S2C_SHOOT = 2,
  MESSAGE_ID_C2S_MOVE = 3,
  MESSAGE_ID_C2S_EXTEND_SESSION = 4,
  MESSAGE_ID_C2S_REQUEST_LOGIN = 5,
  MESSAGE_ID_C2S_REQUEST_REGISTER = 6,
  MESSAGE_ID_S2C_COMPLETE_LOGIN = 7,
  MESSAGE_ID_S2C_COMPLETE_REGISTER = 8,
  MESSAGE_ID_S2C_LOGIN_ERROR = 9,
  MESSAGE_ID_S2C_REGISTER_ERROR = 10,
  MESSAGE_ID_MIN = MESSAGE_ID_NONE,
  MESSAGE_ID_MAX = MESSAGE_ID_S2C_REGISTER_ERROR
};

inline const MESSAGE_ID (&EnumValuesMESSAGE_ID())[11] {
  static const MESSAGE_ID values[] = {
    MESSAGE_ID_NONE,
    MESSAGE_ID_S2C_MOVE,
    MESSAGE_ID_S2C_SHOOT,
    MESSAGE_ID_C2S_MOVE,
    MESSAGE_ID_C2S_EXTEND_SESSION,
    MESSAGE_ID_C2S_REQUEST_LOGIN,
    MESSAGE_ID_C2S_REQUEST_REGISTER,
    MESSAGE_ID_S2C_COMPLETE_LOGIN,
    MESSAGE_ID_S2C_COMPLETE_REGISTER,
    MESSAGE_ID_S2C_LOGIN_ERROR,
    MESSAGE_ID_S2C_REGISTER_ERROR
  };
  return values;
}

inline const char * const *EnumNamesMESSAGE_ID() {
  static const char * const names[12] = {
    "NONE",
    "S2C_MOVE",
    "S2C_SHOOT",
    "C2S_MOVE",
    "C2S_EXTEND_SESSION",
    "C2S_REQUEST_LOGIN",
    "C2S_REQUEST_REGISTER",
    "S2C_COMPLETE_LOGIN",
    "S2C_COMPLETE_REGISTER",
    "S2C_LOGIN_ERROR",
    "S2C_REGISTER_ERROR",
    nullptr
  };
  return names;
}

inline const char *EnumNameMESSAGE_ID(MESSAGE_ID e) {
  if (flatbuffers::IsOutRange(e, MESSAGE_ID_NONE, MESSAGE_ID_S2C_REGISTER_ERROR)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesMESSAGE_ID()[index];
}

template<typename T> struct MESSAGE_IDTraits {
  static const MESSAGE_ID enum_value = MESSAGE_ID_NONE;
};

template<> struct MESSAGE_IDTraits<S2C_MOVE> {
  static const MESSAGE_ID enum_value = MESSAGE_ID_S2C_MOVE;
};

template<> struct MESSAGE_IDTraits<S2C_SHOOT> {
  static const MESSAGE_ID enum_value = MESSAGE_ID_S2C_SHOOT;
};

template<> struct MESSAGE_IDTraits<C2S_MOVE> {
  static const MESSAGE_ID enum_value = MESSAGE_ID_C2S_MOVE;
};

template<> struct MESSAGE_IDTraits<C2S_EXTEND_SESSION> {
  static const MESSAGE_ID enum_value = MESSAGE_ID_C2S_EXTEND_SESSION;
};

template<> struct MESSAGE_IDTraits<C2S_REQUEST_LOGIN> {
  static const MESSAGE_ID enum_value = MESSAGE_ID_C2S_REQUEST_LOGIN;
};

template<> struct MESSAGE_IDTraits<C2S_REQUEST_REGISTER> {
  static const MESSAGE_ID enum_value = MESSAGE_ID_C2S_REQUEST_REGISTER;
};

template<> struct MESSAGE_IDTraits<S2C_COMPLETE_LOGIN> {
  static const MESSAGE_ID enum_value = MESSAGE_ID_S2C_COMPLETE_LOGIN;
};

template<> struct MESSAGE_IDTraits<S2C_COMPLETE_REGISTER> {
  static const MESSAGE_ID enum_value = MESSAGE_ID_S2C_COMPLETE_REGISTER;
};

template<> struct MESSAGE_IDTraits<S2C_LOGIN_ERROR> {
  static const MESSAGE_ID enum_value = MESSAGE_ID_S2C_LOGIN_ERROR;
};

template<> struct MESSAGE_IDTraits<S2C_REGISTER_ERROR> {
  static const MESSAGE_ID enum_value = MESSAGE_ID_S2C_REGISTER_ERROR;
};

bool VerifyMESSAGE_ID(flatbuffers::Verifier &verifier, const void *obj, MESSAGE_ID type);
bool VerifyMESSAGE_IDVector(flatbuffers::Verifier &verifier, const flatbuffers::Vector<flatbuffers::Offset<void>> *values, const flatbuffers::Vector<uint8_t> *types);

FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(4) Vec3 FLATBUFFERS_FINAL_CLASS {
 private:
  float x_;
  float y_;
  float z_;

 public:
  Vec3()
      : x_(0),
        y_(0),
        z_(0) {
  }
  Vec3(float _x, float _y, float _z)
      : x_(flatbuffers::EndianScalar(_x)),
        y_(flatbuffers::EndianScalar(_y)),
        z_(flatbuffers::EndianScalar(_z)) {
  }
  float x() const {
    return flatbuffers::EndianScalar(x_);
  }
  float y() const {
    return flatbuffers::EndianScalar(y_);
  }
  float z() const {
    return flatbuffers::EndianScalar(z_);
  }
};
FLATBUFFERS_STRUCT_END(Vec3, 12);

struct Message FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef MessageBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_PACKET_TYPE = 4,
    VT_PACKET = 6
  };
  MESSAGE_ID packet_type() const {
    return static_cast<MESSAGE_ID>(GetField<uint8_t>(VT_PACKET_TYPE, 0));
  }
  const void *packet() const {
    return GetPointer<const void *>(VT_PACKET);
  }
  template<typename T> const T *packet_as() const;
  const S2C_MOVE *packet_as_S2C_MOVE() const {
    return packet_type() == MESSAGE_ID_S2C_MOVE ? static_cast<const S2C_MOVE *>(packet()) : nullptr;
  }
  const S2C_SHOOT *packet_as_S2C_SHOOT() const {
    return packet_type() == MESSAGE_ID_S2C_SHOOT ? static_cast<const S2C_SHOOT *>(packet()) : nullptr;
  }
  const C2S_MOVE *packet_as_C2S_MOVE() const {
    return packet_type() == MESSAGE_ID_C2S_MOVE ? static_cast<const C2S_MOVE *>(packet()) : nullptr;
  }
  const C2S_EXTEND_SESSION *packet_as_C2S_EXTEND_SESSION() const {
    return packet_type() == MESSAGE_ID_C2S_EXTEND_SESSION ? static_cast<const C2S_EXTEND_SESSION *>(packet()) : nullptr;
  }
  const C2S_REQUEST_LOGIN *packet_as_C2S_REQUEST_LOGIN() const {
    return packet_type() == MESSAGE_ID_C2S_REQUEST_LOGIN ? static_cast<const C2S_REQUEST_LOGIN *>(packet()) : nullptr;
  }
  const C2S_REQUEST_REGISTER *packet_as_C2S_REQUEST_REGISTER() const {
    return packet_type() == MESSAGE_ID_C2S_REQUEST_REGISTER ? static_cast<const C2S_REQUEST_REGISTER *>(packet()) : nullptr;
  }
  const S2C_COMPLETE_LOGIN *packet_as_S2C_COMPLETE_LOGIN() const {
    return packet_type() == MESSAGE_ID_S2C_COMPLETE_LOGIN ? static_cast<const S2C_COMPLETE_LOGIN *>(packet()) : nullptr;
  }
  const S2C_COMPLETE_REGISTER *packet_as_S2C_COMPLETE_REGISTER() const {
    return packet_type() == MESSAGE_ID_S2C_COMPLETE_REGISTER ? static_cast<const S2C_COMPLETE_REGISTER *>(packet()) : nullptr;
  }
  const S2C_LOGIN_ERROR *packet_as_S2C_LOGIN_ERROR() const {
    return packet_type() == MESSAGE_ID_S2C_LOGIN_ERROR ? static_cast<const S2C_LOGIN_ERROR *>(packet()) : nullptr;
  }
  const S2C_REGISTER_ERROR *packet_as_S2C_REGISTER_ERROR() const {
    return packet_type() == MESSAGE_ID_S2C_REGISTER_ERROR ? static_cast<const S2C_REGISTER_ERROR *>(packet()) : nullptr;
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint8_t>(verifier, VT_PACKET_TYPE) &&
           VerifyOffset(verifier, VT_PACKET) &&
           VerifyMESSAGE_ID(verifier, packet(), packet_type()) &&
           verifier.EndTable();
  }
};

template<> inline const S2C_MOVE *Message::packet_as<S2C_MOVE>() const {
  return packet_as_S2C_MOVE();
}

template<> inline const S2C_SHOOT *Message::packet_as<S2C_SHOOT>() const {
  return packet_as_S2C_SHOOT();
}

template<> inline const C2S_MOVE *Message::packet_as<C2S_MOVE>() const {
  return packet_as_C2S_MOVE();
}

template<> inline const C2S_EXTEND_SESSION *Message::packet_as<C2S_EXTEND_SESSION>() const {
  return packet_as_C2S_EXTEND_SESSION();
}

template<> inline const C2S_REQUEST_LOGIN *Message::packet_as<C2S_REQUEST_LOGIN>() const {
  return packet_as_C2S_REQUEST_LOGIN();
}

template<> inline const C2S_REQUEST_REGISTER *Message::packet_as<C2S_REQUEST_REGISTER>() const {
  return packet_as_C2S_REQUEST_REGISTER();
}

template<> inline const S2C_COMPLETE_LOGIN *Message::packet_as<S2C_COMPLETE_LOGIN>() const {
  return packet_as_S2C_COMPLETE_LOGIN();
}

template<> inline const S2C_COMPLETE_REGISTER *Message::packet_as<S2C_COMPLETE_REGISTER>() const {
  return packet_as_S2C_COMPLETE_REGISTER();
}

template<> inline const S2C_LOGIN_ERROR *Message::packet_as<S2C_LOGIN_ERROR>() const {
  return packet_as_S2C_LOGIN_ERROR();
}

template<> inline const S2C_REGISTER_ERROR *Message::packet_as<S2C_REGISTER_ERROR>() const {
  return packet_as_S2C_REGISTER_ERROR();
}

struct MessageBuilder {
  typedef Message Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_packet_type(MESSAGE_ID packet_type) {
    fbb_.AddElement<uint8_t>(Message::VT_PACKET_TYPE, static_cast<uint8_t>(packet_type), 0);
  }
  void add_packet(flatbuffers::Offset<void> packet) {
    fbb_.AddOffset(Message::VT_PACKET, packet);
  }
  explicit MessageBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<Message> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Message>(end);
    return o;
  }
};

inline flatbuffers::Offset<Message> CreateMessage(
    flatbuffers::FlatBufferBuilder &_fbb,
    MESSAGE_ID packet_type = MESSAGE_ID_NONE,
    flatbuffers::Offset<void> packet = 0) {
  MessageBuilder builder_(_fbb);
  builder_.add_packet(packet);
  builder_.add_packet_type(packet_type);
  return builder_.Finish();
}

struct S2C_MOVE FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef S2C_MOVEBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NICK_NAME = 4,
    VT_POS = 6,
    VT_DIR = 8
  };
  const flatbuffers::String *nick_name() const {
    return GetPointer<const flatbuffers::String *>(VT_NICK_NAME);
  }
  const Vec3 *pos() const {
    return GetStruct<const Vec3 *>(VT_POS);
  }
  const Vec3 *dir() const {
    return GetStruct<const Vec3 *>(VT_DIR);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_NICK_NAME) &&
           verifier.VerifyString(nick_name()) &&
           VerifyField<Vec3>(verifier, VT_POS) &&
           VerifyField<Vec3>(verifier, VT_DIR) &&
           verifier.EndTable();
  }
};

struct S2C_MOVEBuilder {
  typedef S2C_MOVE Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_nick_name(flatbuffers::Offset<flatbuffers::String> nick_name) {
    fbb_.AddOffset(S2C_MOVE::VT_NICK_NAME, nick_name);
  }
  void add_pos(const Vec3 *pos) {
    fbb_.AddStruct(S2C_MOVE::VT_POS, pos);
  }
  void add_dir(const Vec3 *dir) {
    fbb_.AddStruct(S2C_MOVE::VT_DIR, dir);
  }
  explicit S2C_MOVEBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<S2C_MOVE> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<S2C_MOVE>(end);
    return o;
  }
};

inline flatbuffers::Offset<S2C_MOVE> CreateS2C_MOVE(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> nick_name = 0,
    const Vec3 *pos = 0,
    const Vec3 *dir = 0) {
  S2C_MOVEBuilder builder_(_fbb);
  builder_.add_dir(dir);
  builder_.add_pos(pos);
  builder_.add_nick_name(nick_name);
  return builder_.Finish();
}

inline flatbuffers::Offset<S2C_MOVE> CreateS2C_MOVEDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *nick_name = nullptr,
    const Vec3 *pos = 0,
    const Vec3 *dir = 0) {
  auto nick_name__ = nick_name ? _fbb.CreateString(nick_name) : 0;
  return CreateS2C_MOVE(
      _fbb,
      nick_name__,
      pos,
      dir);
}

struct S2C_SHOOT FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef S2C_SHOOTBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NICK_NAME = 4,
    VT_POS = 6,
    VT_DIR = 8
  };
  const flatbuffers::String *nick_name() const {
    return GetPointer<const flatbuffers::String *>(VT_NICK_NAME);
  }
  const Vec3 *pos() const {
    return GetStruct<const Vec3 *>(VT_POS);
  }
  const Vec3 *dir() const {
    return GetStruct<const Vec3 *>(VT_DIR);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_NICK_NAME) &&
           verifier.VerifyString(nick_name()) &&
           VerifyField<Vec3>(verifier, VT_POS) &&
           VerifyField<Vec3>(verifier, VT_DIR) &&
           verifier.EndTable();
  }
};

struct S2C_SHOOTBuilder {
  typedef S2C_SHOOT Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_nick_name(flatbuffers::Offset<flatbuffers::String> nick_name) {
    fbb_.AddOffset(S2C_SHOOT::VT_NICK_NAME, nick_name);
  }
  void add_pos(const Vec3 *pos) {
    fbb_.AddStruct(S2C_SHOOT::VT_POS, pos);
  }
  void add_dir(const Vec3 *dir) {
    fbb_.AddStruct(S2C_SHOOT::VT_DIR, dir);
  }
  explicit S2C_SHOOTBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<S2C_SHOOT> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<S2C_SHOOT>(end);
    return o;
  }
};

inline flatbuffers::Offset<S2C_SHOOT> CreateS2C_SHOOT(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> nick_name = 0,
    const Vec3 *pos = 0,
    const Vec3 *dir = 0) {
  S2C_SHOOTBuilder builder_(_fbb);
  builder_.add_dir(dir);
  builder_.add_pos(pos);
  builder_.add_nick_name(nick_name);
  return builder_.Finish();
}

inline flatbuffers::Offset<S2C_SHOOT> CreateS2C_SHOOTDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *nick_name = nullptr,
    const Vec3 *pos = 0,
    const Vec3 *dir = 0) {
  auto nick_name__ = nick_name ? _fbb.CreateString(nick_name) : 0;
  return CreateS2C_SHOOT(
      _fbb,
      nick_name__,
      pos,
      dir);
}

struct C2S_MOVE FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef C2S_MOVEBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NICK_NAME = 4,
    VT_POS = 6,
    VT_DIR = 8
  };
  const flatbuffers::String *nick_name() const {
    return GetPointer<const flatbuffers::String *>(VT_NICK_NAME);
  }
  const Vec3 *pos() const {
    return GetStruct<const Vec3 *>(VT_POS);
  }
  const Vec3 *dir() const {
    return GetStruct<const Vec3 *>(VT_DIR);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_NICK_NAME) &&
           verifier.VerifyString(nick_name()) &&
           VerifyField<Vec3>(verifier, VT_POS) &&
           VerifyField<Vec3>(verifier, VT_DIR) &&
           verifier.EndTable();
  }
};

struct C2S_MOVEBuilder {
  typedef C2S_MOVE Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_nick_name(flatbuffers::Offset<flatbuffers::String> nick_name) {
    fbb_.AddOffset(C2S_MOVE::VT_NICK_NAME, nick_name);
  }
  void add_pos(const Vec3 *pos) {
    fbb_.AddStruct(C2S_MOVE::VT_POS, pos);
  }
  void add_dir(const Vec3 *dir) {
    fbb_.AddStruct(C2S_MOVE::VT_DIR, dir);
  }
  explicit C2S_MOVEBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<C2S_MOVE> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<C2S_MOVE>(end);
    return o;
  }
};

inline flatbuffers::Offset<C2S_MOVE> CreateC2S_MOVE(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> nick_name = 0,
    const Vec3 *pos = 0,
    const Vec3 *dir = 0) {
  C2S_MOVEBuilder builder_(_fbb);
  builder_.add_dir(dir);
  builder_.add_pos(pos);
  builder_.add_nick_name(nick_name);
  return builder_.Finish();
}

inline flatbuffers::Offset<C2S_MOVE> CreateC2S_MOVEDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *nick_name = nullptr,
    const Vec3 *pos = 0,
    const Vec3 *dir = 0) {
  auto nick_name__ = nick_name ? _fbb.CreateString(nick_name) : 0;
  return CreateC2S_MOVE(
      _fbb,
      nick_name__,
      pos,
      dir);
}

struct C2S_EXTEND_SESSION FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef C2S_EXTEND_SESSIONBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NICK_NAME = 4
  };
  const flatbuffers::String *nick_name() const {
    return GetPointer<const flatbuffers::String *>(VT_NICK_NAME);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_NICK_NAME) &&
           verifier.VerifyString(nick_name()) &&
           verifier.EndTable();
  }
};

struct C2S_EXTEND_SESSIONBuilder {
  typedef C2S_EXTEND_SESSION Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_nick_name(flatbuffers::Offset<flatbuffers::String> nick_name) {
    fbb_.AddOffset(C2S_EXTEND_SESSION::VT_NICK_NAME, nick_name);
  }
  explicit C2S_EXTEND_SESSIONBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<C2S_EXTEND_SESSION> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<C2S_EXTEND_SESSION>(end);
    return o;
  }
};

inline flatbuffers::Offset<C2S_EXTEND_SESSION> CreateC2S_EXTEND_SESSION(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> nick_name = 0) {
  C2S_EXTEND_SESSIONBuilder builder_(_fbb);
  builder_.add_nick_name(nick_name);
  return builder_.Finish();
}

inline flatbuffers::Offset<C2S_EXTEND_SESSION> CreateC2S_EXTEND_SESSIONDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *nick_name = nullptr) {
  auto nick_name__ = nick_name ? _fbb.CreateString(nick_name) : 0;
  return CreateC2S_EXTEND_SESSION(
      _fbb,
      nick_name__);
}

struct C2S_REQUEST_LOGIN FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef C2S_REQUEST_LOGINBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_EMAIL = 4,
    VT_PASSWORD = 6
  };
  const flatbuffers::String *email() const {
    return GetPointer<const flatbuffers::String *>(VT_EMAIL);
  }
  const flatbuffers::String *password() const {
    return GetPointer<const flatbuffers::String *>(VT_PASSWORD);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_EMAIL) &&
           verifier.VerifyString(email()) &&
           VerifyOffset(verifier, VT_PASSWORD) &&
           verifier.VerifyString(password()) &&
           verifier.EndTable();
  }
};

struct C2S_REQUEST_LOGINBuilder {
  typedef C2S_REQUEST_LOGIN Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_email(flatbuffers::Offset<flatbuffers::String> email) {
    fbb_.AddOffset(C2S_REQUEST_LOGIN::VT_EMAIL, email);
  }
  void add_password(flatbuffers::Offset<flatbuffers::String> password) {
    fbb_.AddOffset(C2S_REQUEST_LOGIN::VT_PASSWORD, password);
  }
  explicit C2S_REQUEST_LOGINBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<C2S_REQUEST_LOGIN> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<C2S_REQUEST_LOGIN>(end);
    return o;
  }
};

inline flatbuffers::Offset<C2S_REQUEST_LOGIN> CreateC2S_REQUEST_LOGIN(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> email = 0,
    flatbuffers::Offset<flatbuffers::String> password = 0) {
  C2S_REQUEST_LOGINBuilder builder_(_fbb);
  builder_.add_password(password);
  builder_.add_email(email);
  return builder_.Finish();
}

inline flatbuffers::Offset<C2S_REQUEST_LOGIN> CreateC2S_REQUEST_LOGINDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *email = nullptr,
    const char *password = nullptr) {
  auto email__ = email ? _fbb.CreateString(email) : 0;
  auto password__ = password ? _fbb.CreateString(password) : 0;
  return CreateC2S_REQUEST_LOGIN(
      _fbb,
      email__,
      password__);
}

struct C2S_REQUEST_REGISTER FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef C2S_REQUEST_REGISTERBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_EMAIL = 4,
    VT_NICKNAME = 6,
    VT_PASSWORD = 8
  };
  const flatbuffers::String *email() const {
    return GetPointer<const flatbuffers::String *>(VT_EMAIL);
  }
  const flatbuffers::String *nickname() const {
    return GetPointer<const flatbuffers::String *>(VT_NICKNAME);
  }
  const flatbuffers::String *password() const {
    return GetPointer<const flatbuffers::String *>(VT_PASSWORD);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_EMAIL) &&
           verifier.VerifyString(email()) &&
           VerifyOffset(verifier, VT_NICKNAME) &&
           verifier.VerifyString(nickname()) &&
           VerifyOffset(verifier, VT_PASSWORD) &&
           verifier.VerifyString(password()) &&
           verifier.EndTable();
  }
};

struct C2S_REQUEST_REGISTERBuilder {
  typedef C2S_REQUEST_REGISTER Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_email(flatbuffers::Offset<flatbuffers::String> email) {
    fbb_.AddOffset(C2S_REQUEST_REGISTER::VT_EMAIL, email);
  }
  void add_nickname(flatbuffers::Offset<flatbuffers::String> nickname) {
    fbb_.AddOffset(C2S_REQUEST_REGISTER::VT_NICKNAME, nickname);
  }
  void add_password(flatbuffers::Offset<flatbuffers::String> password) {
    fbb_.AddOffset(C2S_REQUEST_REGISTER::VT_PASSWORD, password);
  }
  explicit C2S_REQUEST_REGISTERBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<C2S_REQUEST_REGISTER> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<C2S_REQUEST_REGISTER>(end);
    return o;
  }
};

inline flatbuffers::Offset<C2S_REQUEST_REGISTER> CreateC2S_REQUEST_REGISTER(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> email = 0,
    flatbuffers::Offset<flatbuffers::String> nickname = 0,
    flatbuffers::Offset<flatbuffers::String> password = 0) {
  C2S_REQUEST_REGISTERBuilder builder_(_fbb);
  builder_.add_password(password);
  builder_.add_nickname(nickname);
  builder_.add_email(email);
  return builder_.Finish();
}

inline flatbuffers::Offset<C2S_REQUEST_REGISTER> CreateC2S_REQUEST_REGISTERDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *email = nullptr,
    const char *nickname = nullptr,
    const char *password = nullptr) {
  auto email__ = email ? _fbb.CreateString(email) : 0;
  auto nickname__ = nickname ? _fbb.CreateString(nickname) : 0;
  auto password__ = password ? _fbb.CreateString(password) : 0;
  return CreateC2S_REQUEST_REGISTER(
      _fbb,
      email__,
      nickname__,
      password__);
}

struct S2C_COMPLETE_LOGIN FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef S2C_COMPLETE_LOGINBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NICKNAME = 4
  };
  const flatbuffers::String *nickname() const {
    return GetPointer<const flatbuffers::String *>(VT_NICKNAME);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_NICKNAME) &&
           verifier.VerifyString(nickname()) &&
           verifier.EndTable();
  }
};

struct S2C_COMPLETE_LOGINBuilder {
  typedef S2C_COMPLETE_LOGIN Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_nickname(flatbuffers::Offset<flatbuffers::String> nickname) {
    fbb_.AddOffset(S2C_COMPLETE_LOGIN::VT_NICKNAME, nickname);
  }
  explicit S2C_COMPLETE_LOGINBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<S2C_COMPLETE_LOGIN> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<S2C_COMPLETE_LOGIN>(end);
    return o;
  }
};

inline flatbuffers::Offset<S2C_COMPLETE_LOGIN> CreateS2C_COMPLETE_LOGIN(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> nickname = 0) {
  S2C_COMPLETE_LOGINBuilder builder_(_fbb);
  builder_.add_nickname(nickname);
  return builder_.Finish();
}

inline flatbuffers::Offset<S2C_COMPLETE_LOGIN> CreateS2C_COMPLETE_LOGINDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *nickname = nullptr) {
  auto nickname__ = nickname ? _fbb.CreateString(nickname) : 0;
  return CreateS2C_COMPLETE_LOGIN(
      _fbb,
      nickname__);
}

struct S2C_COMPLETE_REGISTER FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef S2C_COMPLETE_REGISTERBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_ISSUCCESS = 4
  };
  bool issuccess() const {
    return GetField<uint8_t>(VT_ISSUCCESS, 0) != 0;
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint8_t>(verifier, VT_ISSUCCESS) &&
           verifier.EndTable();
  }
};

struct S2C_COMPLETE_REGISTERBuilder {
  typedef S2C_COMPLETE_REGISTER Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_issuccess(bool issuccess) {
    fbb_.AddElement<uint8_t>(S2C_COMPLETE_REGISTER::VT_ISSUCCESS, static_cast<uint8_t>(issuccess), 0);
  }
  explicit S2C_COMPLETE_REGISTERBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<S2C_COMPLETE_REGISTER> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<S2C_COMPLETE_REGISTER>(end);
    return o;
  }
};

inline flatbuffers::Offset<S2C_COMPLETE_REGISTER> CreateS2C_COMPLETE_REGISTER(
    flatbuffers::FlatBufferBuilder &_fbb,
    bool issuccess = false) {
  S2C_COMPLETE_REGISTERBuilder builder_(_fbb);
  builder_.add_issuccess(issuccess);
  return builder_.Finish();
}

struct S2C_LOGIN_ERROR FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef S2C_LOGIN_ERRORBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_MESSAGE = 4
  };
  const flatbuffers::String *message() const {
    return GetPointer<const flatbuffers::String *>(VT_MESSAGE);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_MESSAGE) &&
           verifier.VerifyString(message()) &&
           verifier.EndTable();
  }
};

struct S2C_LOGIN_ERRORBuilder {
  typedef S2C_LOGIN_ERROR Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_message(flatbuffers::Offset<flatbuffers::String> message) {
    fbb_.AddOffset(S2C_LOGIN_ERROR::VT_MESSAGE, message);
  }
  explicit S2C_LOGIN_ERRORBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<S2C_LOGIN_ERROR> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<S2C_LOGIN_ERROR>(end);
    return o;
  }
};

inline flatbuffers::Offset<S2C_LOGIN_ERROR> CreateS2C_LOGIN_ERROR(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> message = 0) {
  S2C_LOGIN_ERRORBuilder builder_(_fbb);
  builder_.add_message(message);
  return builder_.Finish();
}

inline flatbuffers::Offset<S2C_LOGIN_ERROR> CreateS2C_LOGIN_ERRORDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *message = nullptr) {
  auto message__ = message ? _fbb.CreateString(message) : 0;
  return CreateS2C_LOGIN_ERROR(
      _fbb,
      message__);
}

struct S2C_REGISTER_ERROR FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef S2C_REGISTER_ERRORBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_MESSAGE = 4
  };
  const flatbuffers::String *message() const {
    return GetPointer<const flatbuffers::String *>(VT_MESSAGE);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_MESSAGE) &&
           verifier.VerifyString(message()) &&
           verifier.EndTable();
  }
};

struct S2C_REGISTER_ERRORBuilder {
  typedef S2C_REGISTER_ERROR Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_message(flatbuffers::Offset<flatbuffers::String> message) {
    fbb_.AddOffset(S2C_REGISTER_ERROR::VT_MESSAGE, message);
  }
  explicit S2C_REGISTER_ERRORBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<S2C_REGISTER_ERROR> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<S2C_REGISTER_ERROR>(end);
    return o;
  }
};

inline flatbuffers::Offset<S2C_REGISTER_ERROR> CreateS2C_REGISTER_ERROR(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> message = 0) {
  S2C_REGISTER_ERRORBuilder builder_(_fbb);
  builder_.add_message(message);
  return builder_.Finish();
}

inline flatbuffers::Offset<S2C_REGISTER_ERROR> CreateS2C_REGISTER_ERRORDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *message = nullptr) {
  auto message__ = message ? _fbb.CreateString(message) : 0;
  return CreateS2C_REGISTER_ERROR(
      _fbb,
      message__);
}

inline bool VerifyMESSAGE_ID(flatbuffers::Verifier &verifier, const void *obj, MESSAGE_ID type) {
  switch (type) {
    case MESSAGE_ID_NONE: {
      return true;
    }
    case MESSAGE_ID_S2C_MOVE: {
      auto ptr = reinterpret_cast<const S2C_MOVE *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case MESSAGE_ID_S2C_SHOOT: {
      auto ptr = reinterpret_cast<const S2C_SHOOT *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case MESSAGE_ID_C2S_MOVE: {
      auto ptr = reinterpret_cast<const C2S_MOVE *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case MESSAGE_ID_C2S_EXTEND_SESSION: {
      auto ptr = reinterpret_cast<const C2S_EXTEND_SESSION *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case MESSAGE_ID_C2S_REQUEST_LOGIN: {
      auto ptr = reinterpret_cast<const C2S_REQUEST_LOGIN *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case MESSAGE_ID_C2S_REQUEST_REGISTER: {
      auto ptr = reinterpret_cast<const C2S_REQUEST_REGISTER *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case MESSAGE_ID_S2C_COMPLETE_LOGIN: {
      auto ptr = reinterpret_cast<const S2C_COMPLETE_LOGIN *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case MESSAGE_ID_S2C_COMPLETE_REGISTER: {
      auto ptr = reinterpret_cast<const S2C_COMPLETE_REGISTER *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case MESSAGE_ID_S2C_LOGIN_ERROR: {
      auto ptr = reinterpret_cast<const S2C_LOGIN_ERROR *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case MESSAGE_ID_S2C_REGISTER_ERROR: {
      auto ptr = reinterpret_cast<const S2C_REGISTER_ERROR *>(obj);
      return verifier.VerifyTable(ptr);
    }
    default: return true;
  }
}

inline bool VerifyMESSAGE_IDVector(flatbuffers::Verifier &verifier, const flatbuffers::Vector<flatbuffers::Offset<void>> *values, const flatbuffers::Vector<uint8_t> *types) {
  if (!values || !types) return !values && !types;
  if (values->size() != types->size()) return false;
  for (flatbuffers::uoffset_t i = 0; i < values->size(); ++i) {
    if (!VerifyMESSAGE_ID(
        verifier,  values->Get(i), types->GetEnum<MESSAGE_ID>(i))) {
      return false;
    }
  }
  return true;
}

inline const Message *GetMessage(const void *buf) {
  return flatbuffers::GetRoot<Message>(buf);
}

inline const Message *GetSizePrefixedMessage(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<Message>(buf);
}

inline bool VerifyMessageBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<Message>(nullptr);
}

inline bool VerifySizePrefixedMessageBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<Message>(nullptr);
}

inline void FinishMessageBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<Message> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedMessageBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<Message> root) {
  fbb.FinishSizePrefixed(root);
}

#endif  // FLATBUFFERS_GENERATED_UDPPROTOCOL_H_
