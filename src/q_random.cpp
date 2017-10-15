#include "q_shared.h"
#include <ctime>
#include <random>

static std::mt19937 mt(time(NULL));
static std::uniform_int_distribution<uint32_t> idistrib;

int32_t irandom()
{
	return mt();
}

int32_t irandom(const int32_t &min, const int32_t &max)
{
	return std::uniform_int_distribution<int32_t>(min, max)(mt);
}

vec_t random()
{
	return (vec_t) irandom() / mt.max();
}

vec_t random(const vec_t &min, const vec_t &max)
{
	return std::uniform_real_distribution<vec_t>(min, max)(mt);
}