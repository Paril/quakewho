#pragma once

#include <condition_variable>

#define MAKE_BITFLAGS(T) \
inline T operator~ (const T &a) { return (T) ~(int32_t) a; } \
inline T operator| (const T &a, const T &b) { return (T) ((int32_t) a | (int32_t) b); } \
inline T operator& (const T &a, const T &b) { return (T) ((int32_t) a & (int32_t) b); } \
inline T operator^ (const T &a, const T &b) { return (T) ((int32_t) a ^ (int32_t) b); } \
inline T &operator|= (T &a, const T &b) { return (T &) ((int32_t &) a |= (int32_t) b); } \
inline T &operator&= (T &a, const T &b) { return (T &) ((int32_t &) a &= (int32_t) b); } \
inline T &operator^= (T &a, const T &b) { return (T &) ((int32_t &) a ^= (int32_t) b); }