#pragma once

#include <algorithm>
#include <cmath>

#include "MathTrick.h"

class Vector2
{
public:
	float X, Y;

public:
	//performance reasons ?
	Vector2() {}
	Vector2(const Vector2& v) : X(v.X), Y(v.Y) {}
	Vector2(float inX, float inY) { X = inX; Y = inY; }
	explicit Vector2(const float* p) { X = p[0]; Y = p[1]; }

	void Set(float inX, float inY) { X = inX; Y = inY; }
	void SetZero() { X = 0.0f; Y = 0.0f; }

	float* GetPtr() { return &X; }
	const float* GetPtr() const { return &X; }
	float& operator[](int i) { /*assert (i >= 0 && i <= 1);*/ return (&X)[i]; }
	const float& operator[](int i) const { /*assert (i >= 0 && i <= 1);*/; return (&X)[i]; }

	Vector2& operator+=(const Vector2& v) { X += v.X; Y += v.Y; return *this; }
	Vector2& operator-=(const Vector2& v) { X -= v.X; Y -= v.Y; return *this; }
	Vector2& operator*=(const float s) { X *= s; Y *= s; return *this; }
	Vector2& operator/=(const float s) { /*(!CompareApproximately(s, 0.0F)); */ X /= s; Y /= s; return *this; }

	bool operator==(const Vector2& v) const { return X == v.X && Y == v.Y; }
	bool operator!=(const Vector2& v) const { return X != v.X || Y != v.Y; }

	Vector2 operator-() const { return Vector2(-X, -Y); }
	Vector2& Scale(const Vector2& v) { X *= v.X; Y *= v.Y; return *this; }

public:
	//static
	static inline Vector2 Scale(const Vector2& lhs, const Vector2& rhs);
	static inline float Dot(const Vector2& lhs, const Vector2& rhs);
	static inline float SqrLength(const Vector2& inV);
	static inline float Length(const Vector2& inV);
	static inline float Angle(const Vector2& lhs, const Vector2& rhs);
	static inline Vector2 Inverse(const Vector2& inVec);
	static inline Vector2 Lerp(const Vector2& from, const Vector2& to, float t);
	static inline Vector2 Min(const Vector2& lhs, const Vector2& rhs);
	static inline Vector2 Max(const Vector2& lhs, const Vector2& rhs);
	static inline bool CompareApproximately(const Vector2& inV0, const Vector2& inV1, float inMaxDist = Vector2::epsilon);
	static inline bool IsNormalized(const Vector2& vec, float epsilon = Vector2::epsilon);
	static inline Vector2 Abs(const Vector2& v);
	static inline bool IsFinite(const Vector2& f);
	
	static inline Vector2 Normalize(const Vector2& inV);
	static inline Vector2 NormalizeFast(const Vector2& inV);
	static inline Vector2 NormalizeSafe(const Vector2& inV, const Vector2& defaultV = Vector2::zero);

public:
	static const float epsilon;
	static const float infinity;
	
	static const Vector2 infinityVec;
	static const Vector2 zero;
	static const Vector2 one;
	
	static const Vector2 xAxis;
	static const Vector2 yAxis;
};

inline Vector2 operator+(const Vector2& lhs, const Vector2& rhs);
inline Vector2 operator-(const Vector2& lhs, const Vector2& rhs);
inline Vector2 operator*(const Vector2& lhs, const Vector2& rhs);
inline Vector2 operator*(const Vector2& inV, float s);
inline Vector2 operator*(const float s, const Vector2& inV);
inline Vector2 operator/(const Vector2& inV, float s);