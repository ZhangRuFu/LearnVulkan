#pragma once

#include <cmath>

#ifndef kPI
	#define kPI 3.14159265358979323846264338327950288419716939937510F
#endif

template<class T>
inline T clamp(const T&t, const T& t0, const T& t1)
{
	if (t < t0)
		return t0;
	else if (t > t1)
		return t1;
	else
		return t;
}

template<>
inline float clamp(const float&t, const float& t0, const float& t1)
{
	if (t < t0)
		return t0;
	else if (t > t1)
		return t1;
	else
		return t;
}

template<class T>
inline T clamp01(const T& t)
{
	if (t < 0)
		return 0;
	else if (t > 1)
		return 1;
	else
		return t;
}

template<>
inline float clamp01<float>(const float& t)
{
	if (t < 0.0F)
		return 0.0F;
	else if (t > 1.0F)
		return 1.0F;
	else
		return t;
}

// Returns the t^2
template<class T>
T Sqr(const T& t)
{
	return t * t;
}

inline float FastInvSqrt(float f)
{
	// The Newton iteration trick used in FastestInvSqrt is a bit faster on
	// Pentium4 / Windows, but lower precision. Doing two iterations is precise enough,
	// but actually a bit slower.
	if (fabs(f) == 0.0F)
		return f;
	return 1.0F / sqrtf(f);
}

//No to webgl, loading floats as ints is too slow in webgl
inline float FastestInvSqrt(float f)
{
	union
	{
		float f;
		int i;
	} u;
	float fhalf = 0.5f * f;
	u.f = f;
	int i = u.i;
	i = 0x5f3759df - (i >> 1);
	u.i = i;
	f = u.f;
	f = f * (1.5f - fhalf * f * f);
	// f = f*(1.5f - fhalf*f*f); // uncommenting this would be two iterations
	return f;
}

inline bool CompareApproximately(float f0, float f1, float epsilon = 0.000001F)
{
	float dist = (f0 - f1);
	dist = abs(dist);
	return dist <= epsilon;
}

inline float Sign(float f)
{
	if (f < 0.0F)
		return -1.0F;
	else
		return 1.0;
}

/// CopySignf () returns x with its sign changed to y's.
inline float CopySignf(float x, float y)
{

	// Loading floats as integers will not produce correct results in WebGL
	// return x * Sign(x) * Sign(y);
	
	// 这能快多少???

	union
	{
		float f;
		uint32_t i;
	} u, u0, u1;
	u0.f = x; 
	u1.f = y;
	uint32_t a = u0.i;
	uint32_t b = u1.i;

	int32_t mask = 1 << 31;
	uint32_t sign = b & mask;
	a &= ~mask;
	a |= sign;

	u.i = a;
	return u.f;
}

inline float InvSqrt(float p) { return 1.0F / sqrt(p); }

inline float Lerp(float from, float to, float t)
{
	return to * t + from * (1.0F - t);
}

//angle and radian

#define kDeg2Rad (2.0F * kPI / 360.0F)
#define kRad2Deg (1.F / kDeg2Rad)

inline float Deg2Rad(float deg)
{
	// TODO : should be deg * kDeg2Rad, but can't be changed,
	// because it changes the order of operations and that affects a replay in some RegressionTests
	return deg / 360.0F * 2.0F * kPI;
}

inline float Rad2Deg(float rad)
{
	// TODO : should be rad * kRad2Deg, but can't be changed,
	// because it changes the order of operations and that affects a replay in some RegressionTests
	return rad / 2.0F / kPI * 360.0F;
}

inline float Radians(float deg)
{
	return deg * kDeg2Rad;
}

inline float Degrees(float rad)
{
	return rad * kRad2Deg;
}