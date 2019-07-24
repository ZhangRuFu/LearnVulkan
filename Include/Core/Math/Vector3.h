#pragma once

#include <algorithm>
#include "MathTrick.h"

class Vector3
{
public:
	float X, Y, Z;

public:
	//performance reason ?
	Vector3() {}
	Vector3(const Vector3& v) : X(v.X), Y(v.Y), Z(v.Z) {}
	Vector3(float inX, float inY, float inZ) { X = inX; Y = inY; Z = inZ; }
	explicit Vector3(const float* array) { X = array[0]; Y = array[1]; Z = array[2]; }
	void Set(float inX, float inY, float inZ) { X = inX; Y = inY; Z = inZ; }
	void Set(const float* array) { X = array[0]; Y = array[1]; Z = array[2]; }
	void SetZero() { X = 0.0f; Y = 0.0f; Z = 0.0f; }

	float* GetPtr() { return &X; }
	const float* GetPtr() const { return &X; }
	float& operator[](int i) { /* assert (i >= 0 && i <= 2); */ return (&X)[i]; }
	const float& operator[](int i) const { /* DebugAssert(i >= 0 && i <= 2); */ return (&X)[i]; }

	bool operator==(const Vector3& v) const { return X == v.X && Y == v.Y && Z == v.Z; }
	bool operator!=(const Vector3& v) const { return X != v.X || Y != v.Y || Z != v.Z; }

	Vector3& operator+=(const Vector3& inV) { X += inV.X; Y += inV.Y; Z += inV.Z; return *this; }
	Vector3& operator-=(const Vector3& inV) { X -= inV.X; Y -= inV.Y; Z -= inV.Z; return *this; }
	Vector3& operator*=(float s) { X *= s; Y *= s; Z *= s; return *this; }
	Vector3& operator/=(float s);

	Vector3 operator-() const { return Vector3(-X, -Y, -Z); }

	Vector3& Scale(const Vector3& inV) { X *= inV.X; Y *= inV.Y; Z *= inV.Z; return *this; }

public:
	static inline Vector3 Scale(const Vector3& lhs, const Vector3& rhs);
	static inline float Dot(const Vector3& lhs, const Vector3& rhs);
	static inline float Volume(const Vector3& inV);
	static inline Vector3 Inverse(const Vector3& inVec);
	static inline float SqrMagnitude(const Vector3& inV);
	static inline float Magnitude(const Vector3& inV);
	static inline float Angle(const Vector3& lhs, const Vector3& rhs);
	static inline Vector3 Normalize(const Vector3& inV);
	static inline Vector3 NormalizeSafe(const Vector3& inV, const Vector3& defaultV = Vector3::zero);
	static inline Vector3 NormalizeFast(const Vector3& inV);
	static inline Vector3 NormalizeFastest(const Vector3& inV);
	static inline Vector3 Cross(const Vector3& lhs, const Vector3& rhs);
	static inline Vector3 ReflectVector(const Vector3& inDirection, const Vector3& inNormal);
	static inline Vector3 Lerp(const Vector3& from, const Vector3& to, float t);
	static inline Vector3 Min(const Vector3& lhs, const Vector3& rhs);
	static inline Vector3 Max(const Vector3& lhs, const Vector3& rhs);
	static inline Vector3 Project(const Vector3& v1, const Vector3& v2);
	static inline Vector3 Abs(const Vector3& v);
	static inline bool IsFinite(const Vector3& f);
	static inline Vector3 Round(const Vector3& a, float factor);

	static inline bool CompareApproximately(const Vector3& inV0, const Vector3& inV1, const float inMaxDist = Vector3::epsilon);
	static inline bool IsNormalized(const Vector3& vec, float epsilon = Vector3::epsilon);

	// Orthonormalizes the three vectors, assuming that a orthonormal basis can be formed
	static void OrthoNormalizeFast(Vector3* inU, Vector3* inV, Vector3* inW);
	// Orthonormalizes the three vectors, returns false if no orthonormal basis could be formed.
	static void OrthoNormalize(Vector3* inU, Vector3* inV, Vector3* inW);
	// Orthonormalizes the two vectors. inV is taken as a hint and will try to be as close as possible to inV.
	static void OrthoNormalize(Vector3* inU, Vector3* inV);

	// Calculates a vector that is orthonormal to n.
	// Assumes that n is normalized
	static Vector3 OrthoNormalVectorFast(const Vector3& n);

	// Rotates lhs towards rhs by no more than max Angle
	// Moves the magnitude of lhs towards rhs by no more than maxMagnitude
	static Vector3 RotateTowards(const Vector3& lhs, const Vector3& rhs, float maxAngle, float maxMagnitude);

	// Spherically interpolates the direction of two vectors
	// and interpolates the magnitude of the two vectors
	static Vector3 Slerp(const Vector3& lhs, const Vector3& rhs, float t);

	// Returns a Vector3 that moves lhs towards rhs by a maximum of clampedDistance
	static Vector3 MoveTowards(const Vector3& lhs, const Vector3& rhs, float clampedDistance);


	// this may be called for vectors `a' with extremely small magnitude, for
	// example the result of a cross product on two nearly perpendicular vectors.
	// we must be robust to these small vectors. to prevent numerical error,
	// first find the component a[i] with the largest magnitude and then scale
	// all the components by 1/a[i]. then we can compute the length of `a' and
	// scale the components by 1/l. this has been verified to work with vectors
	// containing the smallest representable numbers.
	static Vector3 NormalizeRobust(const Vector3& a);
	// This also returns vector's inverse original length, to avoid duplicate
	// invSqrt calculations when needed. If a is a zero vector, invOriginalLength will be 0.
	static Vector3 NormalizeRobust(const Vector3& a, float &invOriginalLength, float eps = Vector3::epsilon);

public:
	static const float epsilon;
	static const float infinity;
	static const Vector3 infinityVec;
	static const Vector3 zero;
	static const Vector3 one;
	static const Vector3 xAxis;
	static const Vector3 yAxis;
	static const Vector3 zAxis;
};

inline Vector3 operator+(const Vector3& lhs, const Vector3& rhs) { return Vector3(lhs.X + rhs.X, lhs.Y + rhs.Y, lhs.Z + rhs.Z); }
inline Vector3 operator-(const Vector3& lhs, const Vector3& rhs) { return Vector3(lhs.X - rhs.X, lhs.Y - rhs.Y, lhs.Z - rhs.Z); }
inline Vector3 operator*(const Vector3& inV, const float s) { return Vector3(inV.X * s, inV.Y * s, inV.Z * s); }
inline Vector3 operator*(const float s, const Vector3& inV) { return Vector3(inV.X * s, inV.Y * s, inV.Z * s); }
inline Vector3 operator*(const Vector3& lhs, const Vector3& rhs) { return Vector3(lhs.X * rhs.X, lhs.Y * rhs.Y, lhs.Z * rhs.Z); }
inline Vector3 operator/(const Vector3& inV, const float s) { Vector3 temp(inV); temp /= s; return temp; }
inline Vector3& Vector3::operator/=(float s) { /*assert (!CompareApproximately(s, 0.0F)); */ X /= s; Y /= s; Z /= s; return *this; }
