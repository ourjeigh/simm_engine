#ifndef __TYPES_H__
#define __TYPES_H__
#pragma once

typedef wchar_t wchar;
#define WIDE(x) L##x

typedef short int16;
typedef long int32;
typedef long long int64;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;
typedef unsigned long long uint64;

typedef float real32;
typedef double real64;

const char k_byte_max = 0xFF;
const uint8 k_uint8_max = 0xFF;
const uint16 k_uint16_max = 0xFFFF;
const uint32 k_uint32_max = 0xFFFFFFFF;

#endif //__TYPES_H__