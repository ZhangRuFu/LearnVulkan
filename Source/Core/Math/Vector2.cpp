#include "Core\Math\Vector2.h"

const float     Vector2::epsilon = 0.00001F;
const float     Vector2::infinity = std::numeric_limits<float>::infinity();
const Vector2  Vector2::infinityVec = Vector2(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());

const Vector2  Vector2::zero = Vector2(0, 0);
const Vector2  Vector2::one = Vector2(1, 1);
const Vector2  Vector2::xAxis = Vector2(1, 0);
const Vector2  Vector2::yAxis = Vector2(0, 1);

inline Vector2 operator+(const Vector2 & lhs, const Vector2 & rhs)
{
	return Vector2(lhs.X + rhs.X, lhs.Y + rhs.Y);
}

inline Vector2 operator-(const Vector2 & lhs, const Vector2 & rhs)
{
	return Vector2(lhs.X - rhs.X, lhs.Y - rhs.Y);
}

inline Vector2 operator*(const Vector2 & lhs, const Vector2 & rhs)
{
	return Vector2(lhs.X * rhs.X, lhs.Y * rhs.Y);
}

inline Vector2 operator*(const Vector2 & inV, float s)
{
	return Vector2(inV.X * s, inV.Y * s);
}

inline Vector2 operator*(const float s, const Vector2 & inV)
{
	return Vector2(inV.X * s, inV.Y * s);
}

inline Vector2 operator/(const Vector2 & inV, float s)
{
	Vector2 temp(inV); temp /= s; return temp;
}

inline Vector2 Vector2::Scale(const Vector2 & lhs, const Vector2 & rhs)
{
	return Vector2(lhs.X * rhs.X, lhs.Y * rhs.Y);
}

inline float Vector2::Dot(const Vector2 & lhs, const Vector2 & rhs)
{
	return lhs.X * rhs.X + lhs.Y * rhs.Y;
}

inline float Vector2::SqrLength(const Vector2 & inV)
{
	return Dot(inV, inV);
}

inline float Vector2::Length(const Vector2 & inV)
{
	return sqrt(Dot(inV, inV));
}

inline float Vector2::Angle(const Vector2 & lhs, const Vector2 & rhs)
{
	return acos(std::min(1.0f, std::max(-1.0f, Dot(lhs, rhs) / (Length(lhs) * Length(rhs)))));
}

inline Vector2 Vector2::Inverse(const Vector2 & inVec)
{
	return Vector2(1.0F / inVec.X, 1.0F / inVec.Y);
}

inline Vector2 Vector2::Lerp(const Vector2 & from, const Vector2 & to, float t)
{
	return to * t + from * (1.0f - t);
}

inline Vector2 Vector2::Min(const Vector2 & lhs, const Vector2 & rhs)
{
	return Vector2(std::min(lhs.X, rhs.X), std::min(lhs.Y, rhs.Y));
}

inline Vector2 Vector2::Max(const Vector2 & lhs, const Vector2 & rhs)
{
	return Vector2(std::max(lhs.X, rhs.X), std::max(lhs.Y, rhs.Y));
}

inline bool Vector2::CompareApproximately(const Vector2 & inV0, const Vector2 & inV1, float inMaxDist)
{
	return SqrLength(inV1 - inV0) <= inMaxDist * inMaxDist;
}

inline bool Vector2::IsNormalized(const Vector2 & vec, float epsilon)
{
	return ::CompareApproximately(SqrLength(vec), 1.0f, epsilon);
}

inline Vector2 Vector2::Abs(const Vector2 & v)
{
	return Vector2(abs(v.X), abs(v.Y));
}

inline bool Vector2::IsFinite(const Vector2 & f)
{
	return std::isfinite(f.X) & std::isfinite(f.Y);
}

inline Vector2 Vector2::Normalize(const Vector2 & inV)
{
	//Length == 0 ???
	return inV / Length(inV);
}

inline Vector2 Vector2::NormalizeFast(const Vector2 & inV)
{
	float m = SqrLength(inV);
	// GCC version of __frsqrte:
	//  static inline double __frsqrte (double x) {
	//      double y;
	//      asm ( "frsqrte %0, %1" : /*OUT*/ "=f" (y) : /*IN*/ "f" (x) );
	//      return y;
	//  }
	return inV * FastInvSqrt(m);
}

inline Vector2 Vector2::NormalizeSafe(const Vector2 & inV, const Vector2 & defaultV)
{
	float len = Length(inV);
	if (len > Vector2::epsilon)
		return inV / len;
	else
		return defaultV;
}
