#pragma once

#include <cstdint>
#include <string>
#include <vector>

#define BEGIN_NS(ns) namespace ns {

#define END_NS(ns) }

#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || __cplusplus >= 201703L)
    #define CPP_17_PLUS
#endif


typedef uint8_t Token;

typedef uint8_t Byte;
typedef double Number;

using String = std::string;
using Bytes = std::vector<uint8_t>;
using ByteString = std::vector<uint16_t>;
