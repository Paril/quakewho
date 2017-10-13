#pragma once

#include <type_traits>

#define BITFLAG_TYPE(T) std::underlying_type_t<T>
#define MAKE_BITFLAGS(T) \
inline T operator~ (const T &a) { return (T) ~(BITFLAG_TYPE(T)) a; } \
inline T operator| (const T &a, const T &b) { return (T) ((BITFLAG_TYPE(T)) a | (BITFLAG_TYPE(T)) b); } \
inline T operator& (const T &a, const T &b) { return (T) ((BITFLAG_TYPE(T)) a & (BITFLAG_TYPE(T)) b); } \
inline T operator^ (const T &a, const T &b) { return (T) ((BITFLAG_TYPE(T)) a ^ (BITFLAG_TYPE(T)) b); } \
inline T &operator|= (T &a, const T &b) { return (T &) ((BITFLAG_TYPE(T) &) a |= (BITFLAG_TYPE(T)) b); } \
inline T &operator&= (T &a, const T &b) { return (T &) ((BITFLAG_TYPE(T) &) a &= (BITFLAG_TYPE(T)) b); } \
inline T &operator^= (T &a, const T &b) { return (T &) ((BITFLAG_TYPE(T) &) a ^= (BITFLAG_TYPE(T)) b); }