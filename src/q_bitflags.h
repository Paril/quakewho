#pragma once

template<typename T>
inline constexpr T bit(const T &place) { return 1u << place; }

#include <type_traits>

#define BITFLAG_TYPE(T) std::underlying_type_t<T>
#define MAKE_BITFLAGS(T) \
constexpr T operator~ (const T &a) { return (T) ~(BITFLAG_TYPE(T)) a; } \
constexpr T operator| (const T &a, const T &b) { return (T) ((BITFLAG_TYPE(T)) a | (BITFLAG_TYPE(T)) b); } \
constexpr T operator& (const T &a, const T &b) { return (T) ((BITFLAG_TYPE(T)) a & (BITFLAG_TYPE(T)) b); } \
constexpr T operator^ (const T &a, const T &b) { return (T) ((BITFLAG_TYPE(T)) a ^ (BITFLAG_TYPE(T)) b); } \
inline T &operator|= (T &a, const T &b) { return (T &) ((BITFLAG_TYPE(T) &) a |= (BITFLAG_TYPE(T)) b); } \
inline T &operator&= (T &a, const T &b) { return (T &) ((BITFLAG_TYPE(T) &) a &= (BITFLAG_TYPE(T)) b); } \
inline T &operator^= (T &a, const T &b) { return (T &) ((BITFLAG_TYPE(T) &) a ^= (BITFLAG_TYPE(T)) b); }