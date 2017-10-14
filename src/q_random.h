#pragma once

int irandom();
int irandom(const int &min, const int &max);

inline int irandom(const int &max) { return irandom(0, max); }

float random();
float random(const float &min, const float &max);

inline float random(const float &max) { return random(0, max); }

inline float crandom() { return random(-1.0, 1.0); }
inline float crandom(const float &max) { return random(-max, max); }

inline bool prandom(const int &pct) { return irandom(100) < pct; }
inline bool prandom(const float &pct) { return random(100) < pct; }
