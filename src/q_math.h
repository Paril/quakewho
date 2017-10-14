#pragma once

template <size_t size, typename T>
inline constexpr size_t lengthof(T (&arr)[size]) { return size; }

typedef float vec_t;
typedef vec_t vec3_t[3];

typedef double dvec_t;
typedef dvec_t dvec3_t[3];

template<typename T>
constexpr T pi = 3.141592653589793238462643383279502884e+00;

#ifndef M_PI
 constexpr vec_t M_PI = pi<float>;
#endif

constexpr vec3_t vec3_origin = { 0, 0, 0 };

template<typename Ta, typename Tb>
constexpr auto DotProduct(const Ta *x, const Tb *y)
{
	return x[0] * y[0] + x[1] * y[1] + x[2] * y[2];
}

template<typename Ta, typename Tb, typename Tc>
inline void VectorSubtract(const Ta *a, const Tb *b, Tc *c)
{
	for (int32_t i = 0; i < 3; i++)
		c[i] = a[i] - b[i];
}

template<typename Ta, typename Tb, typename Tc>
inline void VectorAdd(const Ta *a, const Tb *b, Tc *c)
{
	for (int32_t i = 0; i < 3; i++)
		c[i] = a[i] + b[i];
}

template<typename Ta, typename Tb>
inline void VectorCopy(const Ta *a, Tb *b)
{
	for (int32_t i = 0; i < 3; i++)
		b[i] = a[i];
}

template<typename Ta, typename Tx, typename Ty, typename Tz>
inline void VectorSet(Ta *a, const Tx &x, const Ty &y, const Tz &z)
{
	a[0] = x;
	a[1] = y;
	a[2] = z;
}

template<typename T>
inline void VectorClear(T *a)
{
	VectorCopy(vec3_origin, a);
}

template<typename Ta, typename Tb>
inline void VectorNegate(const Ta *a, Tb *b)
{
	for (int32_t i = 0; i < 3; i++)
		b[i] = -a[i];
}

template<typename Ta, typename Ts, typename Tb>
inline void VectorScale(const Ta *a, const Ts &s, Tb *b)
{
	for (int32_t i = 0; i < 3; i++)
		b[i] = a[i] * s;
}

template<typename Ta, typename Ts, typename Tb, typename Tc>
inline void VectorMA (const Ta *a, const Ts &s, const Tb *b, Tc *c)
{
	for (int32_t i = 0; i < 3; i++)
		c[i] = a[i] + s * b[i];
}

template<typename Ta, typename Tb>
constexpr bool VectorCompare (const Ta *v1, const Tb *v2)
{
	return (v1[0] == v2[0] && v1[1] == v2[1] && v1[2] == v2[2]);
}

template<typename T>
constexpr auto VectorLengthSquared(const T *v)
{
	return DotProduct(v, v);
}

template<typename T>
constexpr auto VectorLength(const T *v)
{
	return sqrt(VectorLengthSquared(v));
}

template<typename Ta, typename Tb>
inline auto VectorNormalize (const Ta *v, Tb *o)
{
	auto length = VectorLength(v);

	if (length)
		VectorScale(v, 1.0 / length, o);
	else
		VectorClear(o);
		
	return length;
}

template<typename T>
inline auto VectorNormalize (T *v)
{
	return VectorNormalize(v, v);
}

template<typename Ta, typename Tb, typename Tc>
inline void CrossProduct (const Ta *v1, const Tb *v2, Tc *cross)
{
	cross[0] = v1[1] * v2[2] - v1[2] * v2[1];
	cross[1] = v1[2] * v2[0] - v1[0] * v2[2];
	cross[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

template<typename T>
inline void VectorInverse (T *v)
{
	for (int32_t i = 0; i < 3; i++)
		v[0] = -v[0];
}

template<typename T>
inline void AngleVectors (const T *angles, vec_t *forward, vec_t *right, vec_t *up)
{
	T angle = angles[YAW] * (M_PI*2 / 360);
	T sy = sin(angle);
	T cy = cos(angle);

	angle = angles[PITCH] * (M_PI*2 / 360);
	T sp = sin(angle);
	T cp = cos(angle);
	
	angle = angles[ROLL] * (M_PI*2 / 360);
	T sr = sin(angle);
	T cr = cos(angle);

	if (forward)
	{
		forward[0] = cp * cy;
		forward[1] = cp * sy;
		forward[2] = -sp;
	}
	
	if (right)
	{
		right[0] = (-1 * sr * sp * cy + -1 * cr * -sy);
		right[1] = (-1 * sr * sp * sy + -1 * cr * cy);
		right[2] = -1 * sr * cp;
	}

	if (up)
	{
		up[0] = (cr * sp * cy + -sr * -sy);
		up[1] = (cr * sp * sy + -sr * cy);
		up[2] = cr * cp;
	}
}

template<typename T>
inline void ClearBounds (T *mins, T *maxs)
{
	mins[0] = mins[1] = mins[2] = 99999;
	maxs[0] = maxs[1] = maxs[2] = -99999;
}

template<typename Tv, typename Tm>
inline void AddPointToBounds (const Tv *v, Tm *mins, Tm *maxs)
{
	for (int32_t i = 0; i < 3; i++)
	{
		const Tv &val = v[i];

		if (val < mins[i])
			mins[i] = val;
		
		if (val > maxs[i])
			maxs[i] = val;
	}
}

template<typename T>
constexpr T DEG2RAD(const T &a) { return (a * pi<T>) / 180.0; }

template<typename T>
constexpr T anglemod(const T &x)
{
    T angle = fmod(x, 360);

	if (angle < 0)
        angle += 360;
    
	return angle;
}

template<typename T>
constexpr auto LerpAngle (const T &a2, const T &a1, const vec_t &frac)
{
	if (a1 - a2 > 180)
		a1 -= 360;

	if (a1 - a2 < -180)
		a1 += 360;

	return a2 + frac * (a1 - a2);
}