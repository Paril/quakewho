#include "q_shared.h"
#include <random>

static std::mt19937 mt;
static std::uniform_int_distribution<uint32_t> idistrib;

int irandom()
{
	return mt();
}

int irandom(const int &min, const int &max)
{
	return std::uniform_int_distribution<int>(min, max)(mt);
}

float random()
{
	return (float) irandom() / mt.max();
}

float random(const float &min, const float &max)
{
	return std::uniform_real_distribution<float>(min, max)(mt);
}