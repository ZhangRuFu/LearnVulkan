#pragma once

#include "Vector3.h"

class Vector4
{
public:
	float X;
	float Y;
	float Z;
	float W;

public:
	//performace reason ?
	Vector4() {}
	Vector4(const Vector4& v) : X(v.X), Y(v.Y), Z(v.Z), W(v.W) {}
	Vector4(float inX, float inY, float inZ, float inW) : X(inX), Y(inY), Z(inZ), W(inW) {}
	explicit Vector4(const Vector3& v, float inW) : X(v.X), Y(v.Y), Z(v.Z), W(inW) {}
	explicit Vector4(const float* v) : X(v[0]), Y(v[1]), Z(v[2]), W(v[3]) {}

	void Set(float inX, float inY, float inZ, float inW) { X = inX; Y = inY; Z = inZ; W = inW; }
	void Set(const float* array) { X = array[0]; Y = array[1]; Z = array[2]; W = array[3]; }
	void SetZero() { X = 0.0f; Y = 0.0f; Z = 0.0f; W = 0.0f; }

	float* GetPtr() { return &X; }
	const float* GetPtr() const { return &X; }

	float& operator[](int i) { /* assert (i >= 0 && i <= 3); */ return (&X)[i]; }
	const float& operator[](int i) const { /* assert (i >= 0 && i <= 3); */ return (&X)[i]; }

	bool operator==(const Vector4& v) const { return X == v.X && Y == v.Y && Z == v.Z && W == v.W; }
	bool operator!=(const Vector4& v) const { return X != v.X || Y != v.Y || Z != v.Z || W != v.W; }
	bool operator==(const float v[4]) const { return X == v[0] && Y == v[1] && Z == v[2] && W == v[3]; }
	bool operator!=(const float v[4]) const { return X != v[0] || Y != v[1] || Z != v[2] || W != v[3]; }

	Vector4 operator-() const { return Vector4(-X, -Y, -Z, -W); }

public:
	//static
	inline float Dot(const Vector4& lhs, const Vector4& rhs);
	inline float SqrMagnitude(const Vector4& inV);
	inline float Magnitude(const Vector4& inV);
	inline bool IsFinite(const Vector4& f);
	inline bool CompareApproximately(const Vector4& inV0, const Vector4& inV1, const float inMaxDist = Vector3::epsilon);
	inline Vector4 Lerp(const Vector4& from, const Vector4& to, float t);

public:
	static const float infinity;
	static const Vector4 infinityVec;
	static const Vector4 zero;
	static const Vector4 one;
};

inline Vector4 operator*(const Vector4& lhs, const Vector4& rhs) { return Vector4(lhs.X * rhs.X, lhs.Y * rhs.Y, lhs.Z * rhs.Z, lhs.W * rhs.W); }
inline Vector4 operator*(const Vector4& inV, const float s) { return Vector4(inV.X * s, inV.Y * s, inV.Z * s, inV.W * s); }
inline Vector4 operator+(const Vector4& lhs, const Vector4& rhs) { return Vector4(lhs.X + rhs.X, lhs.Y + rhs.Y, lhs.Z + rhs.Z, lhs.W + rhs.W); }
inline Vector4 operator-(const Vector4& lhs, const Vector4& rhs) { return Vector4(lhs.X - rhs.X, lhs.Y - rhs.Y, lhs.Z - rhs.Z, lhs.W - rhs.W); }
