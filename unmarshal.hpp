#ifndef UNMARSHAL
#define UNMARSHAL

#include "PyObj.hpp"

typedef uint8_t byte;

constexpr byte FLAG_REF = 0x80;

constexpr byte TYPE_NULL = static_cast<uint8_t>('0');
constexpr byte TYPE_CODE = static_cast<uint8_t>('c');
constexpr byte TYPE_TUPLE = static_cast<uint8_t>('(');
constexpr byte TYPE_SMALL_TUPLE = static_cast<uint8_t>(')');
constexpr byte TYPE_NONE = static_cast<uint8_t>('N');
constexpr byte TYPE_INT = static_cast<uint8_t>('i');
constexpr byte TYPE_STR = static_cast<uint8_t>('s');
constexpr byte TYPE_SHORT_ASCII = static_cast<uint8_t>('z');
constexpr byte TYPE_SHORT_ASCII_INTERNED = static_cast<uint8_t>('Z');
constexpr byte TYPE_REF = static_cast<uint8_t>('r');
constexpr byte TYPE_DICT = static_cast<uint8_t>('{');
constexpr byte TYPE_TRUE = static_cast<uint8_t>('T');
constexpr byte TYPE_FALSE = static_cast<uint8_t>('F');
constexpr byte TYPE_BINARY_FLOAT = static_cast<uint8_t>('g');

PyObjPtr unmarshal(const byte contents[]);

PyObjPtr unmarshal_code(const byte contents[], size_t& offset);

int32_t read_int32(const byte contents[], size_t& offset);

PyObjPtr _unmarshal_tuple(const byte contents[], size_t& offset);
PyObjPtr _unmarshal_dict(const byte contents[], size_t& offset);
PyObjPtr _unmarshal_str(const byte contents[], size_t& offset, bool is_short = false);
PyObjPtr _unmarshal(const byte contents[], size_t& offset);
PyObjPtr _unmarshal_nstr(const byte contents[], size_t& offset, size_t n);
FloatPtr unmarshal_float(const byte contents[], size_t& offset);
    
#endif