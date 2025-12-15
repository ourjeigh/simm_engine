#include <asserts.h>
#ifndef __TYPES_H__
#define __TYPES_H__
#pragma once

typedef wchar_t wchar;
#define WIDE(x) L##x

typedef unsigned char byte;

typedef char int8;
typedef short int16;
typedef long int32;
typedef long long int64;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;
typedef unsigned long long uint64;

typedef float real32;
typedef double real64;

const byte k_byte_max = 0xFF;
const char k_char_max = 0xFF;

const int8 k_int8_max = 0x7F;
const int16 k_int16_max = 0x7FFF;
const int32 k_int32_max = 0x7FFFFFFF;
const int64 k_int64_max = 0x7FFFFFFFFFFFFFFF;

const uint8 k_uint8_max = 0xFF;
const uint16 k_uint16_max = 0xFFFF;
const uint32 k_uint32_max = 0xFFFFFFFF;

// conversions
inline int32 int64_to_int32(int64 in)
{
	ASSERT(in <= static_cast<int64>(k_int32_max));
	return static_cast<int32>(in);
}

inline uint32 uint64_to_uint32(uint64 in)
{
	ASSERT(in <= static_cast<uint64>(k_uint32_max));
	return static_cast<uint32>(in);
}

inline int32 uint32_to_int32(uint32 in)
{
	ASSERT(in <= static_cast<uint32>(k_int32_max));
	return static_cast<int32>(in);
}

inline uint32 int32_to_uint32(int32 in)
{
	ASSERT(in >= 0);
	return static_cast<uint32>(in);
}

inline uint64 int64_to_uint64(int64 in)
{
	ASSERT(in >= 0);
	return static_cast<uint64>(in);
}

inline int64 uint64_to_int64(uint64 in)
{
	ASSERT(in <= static_cast<uint64>(k_int64_max));
	return static_cast<int64>(in);
}

inline int32 uint64_to_int32(uint64 in)
{
	ASSERT(in <= static_cast<uint64>(k_int32_max));
	return static_cast<int32>(in);
}

inline uint32 int64_to_uint32(int64 in)
{
	ASSERT(in >= 0);
	ASSERT(in <= static_cast<int64>(k_uint32_max));
	return static_cast<uint32>(in);
}

inline uint32 real32_to_uint32(real32 in)
{
	ASSERT(in >= 0.0f);
	ASSERT(in <= static_cast<real32>(k_uint32_max));
	return static_cast<uint32>(in);
}

inline uint32 real64_to_uint32(real64 in)
{
	ASSERT(in >= 0.0);
	ASSERT(in <= static_cast<real64>(k_uint32_max));
	return static_cast<uint32>(in);
}

#endif //__TYPES_H__