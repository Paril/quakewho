#pragma once

int32_t irandom();
int32_t irandom(const int32_t &min, const int32_t &max);

inline int32_t irandom(const int32_t &max) { return irandom(0, max); }

vec_t random();
vec_t random(const vec_t &min, const vec_t &max);

inline vec_t random(const vec_t &max) { return random(0, max); }

inline vec_t crandom() { return random(-1.0, 1.0); }
inline vec_t crandom(const vec_t &max) { return random(-max, max); }

inline bool prandom(const int32_t &pct) { return irandom(100) < pct; }
inline bool prandom(const vec_t &pct) { return random(100) < pct; }
