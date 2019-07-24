#include "Core\Math\Vector4.h"

const float Vector4::infinity = std::numeric_limits<float>::infinity();
const Vector4 Vector4::infinityVec = Vector4(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());

const Vector4 Vector4::zero = Vector4(0, 0, 0, 0);
const Vector4 Vector4::one = Vector4(1, 1, 1, 1);

inline float Vector4::Dot(const Vector4 & lhs, const Vector4 & rhs)
{
	return lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z + lhs.W * rhs.W;
}

inline float Vector4::SqrMagnitude(const Vector4 & inV)
{
	return Dot(inV, inV);
}

inline float Vector4::Magnitude(const Vector4 & inV)
{
	return sqrt(Dot(inV, inV));
}

inline bool Vector4::IsFinite(const Vector4 & f)
{
	return isfinite(f.X) & isfinite(f.Y) & isfinite(f.Z) && isfinite(f.W);
}

inline bool Vector4::CompareApproximately(const Vector4 & inV0, const Vector4 & inV1, const float inMaxDist)
{
	return SqrMagnitude(inV1 - inV0) <= inMaxDist * inMaxDist;
}

inline Vector4 Vector4::Lerp(const Vector4 & from, const Vector4 & to, float t)
{
	return to * t + from * (1.0F - t);
}
