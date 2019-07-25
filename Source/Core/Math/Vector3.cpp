#include "Core\Math\Vector3.h"
#include "Core\Math\Matrix3x3.h"


const float     Vector3::epsilon = 0.00001F;
const float     Vector3::infinity = std::numeric_limits<float>::infinity();
const Vector3  Vector3::infinityVec = Vector3(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());

const Vector3  Vector3::zero = Vector3(0, 0, 0);
const Vector3  Vector3::one = Vector3(1.0F, 1.0F, 1.0F);
const Vector3  Vector3::xAxis = Vector3(1, 0, 0);
const Vector3  Vector3::yAxis = Vector3(0, 1, 0);
const Vector3  Vector3::zAxis = Vector3(0, 0, 1);

Vector3 Vector3::Scale(const Vector3& lhs, const Vector3& rhs)
{
	return Vector3(lhs.X * rhs.X, lhs.Y * rhs.Y, lhs.Z * rhs.Z);
}

inline float Vector3::Dot(const Vector3 & lhs, const Vector3 & rhs)
{
	return lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z;
}

inline float Vector3::Volume(const Vector3 & inV)
{
	return inV.X * inV.Y * inV.Z;
}

inline Vector3 Vector3::Inverse(const Vector3 & inVec)
{
	return Vector3(1.0F / inVec.X, 1.0F / inVec.Y, 1.0F / inVec.Z);
}

inline float Vector3::SqrLength(const Vector3 & inV)
{
	return Dot(inV, inV);
}

inline float Vector3::Length(const Vector3 & inV)
{
	return sqrt(Dot(inV, inV));
}

inline float Vector3::Angle(const Vector3 & lhs, const Vector3 & rhs)
{
	return ::acos(std::min(1.0f, std::max(-1.0f, Dot(lhs, rhs) / (Length(lhs) * Length(rhs)))));
}

inline Vector3 Vector3::Normalize(const Vector3 & inV)
{
	return inV / Length(inV);
}

inline Vector3 Vector3::NormalizeSafe(const Vector3 & inV, const Vector3 & defaultV)
{
	float mag = Length(inV);
	if (mag > Vector3::epsilon)
		return inV / mag;
	else
		return defaultV;
}

// - Handles zero vector correclty
inline Vector3 Vector3::NormalizeFast(const Vector3 & inV)
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

// - low precision normalize
// - nan for zero vector
inline Vector3 Vector3::NormalizeFastest(const Vector3 & inV)
{
	float m = SqrLength(inV);
	// GCC version of __frsqrte:
	//  static inline double __frsqrte (double x) {
	//      double y;
	//      asm ( "frsqrte %0, %1" : /*OUT*/ "=f" (y) : /*IN*/ "f" (x) );
	//      return y;
	//  }
	return inV * FastestInvSqrt(m);
}

inline Vector3 Vector3::Cross(const Vector3 & lhs, const Vector3 & rhs)
{
	return Vector3(
		lhs.Y * rhs.Z - lhs.Z * rhs.Y,
		lhs.Z * rhs.X - lhs.X * rhs.Z,
		lhs.X * rhs.Y - lhs.Y * rhs.X);
}

inline Vector3 Vector3::ReflectVector(const Vector3 & inDirection, const Vector3 & inNormal)
{
	return -2.0F * Dot(inNormal, inDirection) * inNormal + inDirection;
}

inline Vector3 Vector3::Lerp(const Vector3 & from, const Vector3 & to, float t)
{
	return to * t + from * (1.0F - t);
}

inline Vector3 Vector3::Min(const Vector3 & lhs, const Vector3 & rhs)
{
	return Vector3(std::min(lhs.X, rhs.X), std::min(lhs.Y, rhs.Y), std::min(lhs.Z, rhs.Z));
}

inline Vector3 Vector3::Max(const Vector3 & lhs, const Vector3 & rhs)
{
	return Vector3(std::max(lhs.X, rhs.X), std::max(lhs.Y, rhs.Y), std::max(lhs.Z, rhs.Z));
}

inline Vector3 Vector3::Project(const Vector3 & v1, const Vector3 & v2)
{
	return v2 * Dot(v1, v2) / Dot(v2, v2);
}

inline Vector3 Vector3::Abs(const Vector3 & v)
{
	return Vector3(abs(v.X), abs(v.Y), abs(v.Z));
}

inline bool Vector3::IsFinite(const Vector3 & f)
{
	return isfinite(f.X) & isfinite(f.Y) & isfinite(f.Z);
}

inline Vector3 Vector3::Round(const Vector3 & a, float factor)
{
	return Vector3(roundf(a.X / factor) * factor, roundf(a.Y / factor) * factor, roundf(a.Z / factor) * factor);
}

inline bool Vector3::CompareApproximately(const Vector3 & inV0, const Vector3 & inV1, const float inMaxDist)
{
	return SqrLength(inV1 - inV0) <= inMaxDist * inMaxDist;
}

inline bool Vector3::IsNormalized(const Vector3 & vec, float epsilon)
{
	return ::CompareApproximately(SqrLength(vec), 1.0f, epsilon);
}

void Vector3::OrthoNormalizeFast(Vector3* inU, Vector3* inV, Vector3* inW)
{
	// compute u0
	*inU = Normalize(*inU);

	// compute u1
	float dot0 = Dot(*inU, *inV);
	*inV -= dot0 * *inU;
	*inV = Normalize(*inV);

	// compute u2
	float dot1 = Dot(*inV, *inW);
	dot0 = Dot(*inU, *inW);
	*inW -= dot0 * *inU + dot1 * *inV;
	*inW = Normalize(*inW);
}

void Vector3::OrthoNormalize(Vector3* inU, Vector3* inV)
{
	// compute u0
	float mag = Length(*inU);
	if (mag > Vector3::epsilon)
		*inU /= mag;
	else
		*inU = Vector3(1.0F, 0.0F, 0.0F);

	// compute u1
	float dot0 = Dot(*inU, *inV);
	*inV -= dot0 * *inU;
	mag = Length(*inV);
	if (mag < Vector3::epsilon)
		*inV = OrthoNormalVectorFast(*inU);
	else
		*inV /= mag;
}

void Vector3::OrthoNormalize(Vector3* inU, Vector3* inV, Vector3* inW)
{
	// compute u0
	float mag = Length(*inU);
	if (mag > Vector3::epsilon)
		*inU /= mag;
	else
		*inU = Vector3(1.0F, 0.0F, 0.0F);

	// compute u1
	float dot0 = Dot(*inU, *inV);
	*inV -= dot0 * *inU;
	mag = Length(*inV);
	if (mag > Vector3::epsilon)
		*inV /= mag;
	else
		*inV = OrthoNormalVectorFast(*inU);

	// compute u2
	float dot1 = Dot(*inV, *inW);
	dot0 = Dot(*inU, *inW);
	*inW -= dot0 * *inU + dot1 * *inV;
	mag = Length(*inW);
	if (mag > Vector3::epsilon)
		*inW /= mag;
	else
		*inW = Cross(*inU, *inV);
}

#define k1OverSqrt2 float(0.7071067811865475244008443621048490)
// Calculates a vector that is orthonormal to n, n is normalized
Vector3 Vector3::OrthoNormalVectorFast(const Vector3& n)
{
	Vector3 res;
	if (abs(n.Z) > k1OverSqrt2)
	{
		// choose p in y-z plane
		float a = n.Y * n.Y + n.Z * n.Z;
		float k = 1.0F / sqrt(a);
		res.X = 0;
		res.Y = -n.Z * k;
		res.Z = n.Y * k;
	}
	else
	{
		// choose p in x-y plane
		float a = n.X * n.X + n.Y * n.Y;
		float k = 1.0F / sqrt(a);
		res.X = -n.Y * k;
		res.Y = n.X * k;
		res.Z = 0;
	}
	return res;
}

// Returns a Vector3 that moves lhs towards rhs by a maximum of clampedDistance
Vector3 Vector3::MoveTowards(const Vector3& current, const Vector3& target, float maxDistanceDelta)
{
	Vector3 delta = target - current;
	float sqrDelta = SqrLength(delta);
	float sqrMaxDistanceDelta = maxDistanceDelta * maxDistanceDelta;
	if (sqrDelta > sqrMaxDistanceDelta)
	{
		float deltaMag = sqrt(sqrDelta);
		if (deltaMag > Vector3::epsilon)
			return current + delta / deltaMag * maxDistanceDelta;
		else
			return current;	//too short
	}
	else
		return target;
}

static inline float ClampedMove(float lhs, float rhs, float clampedDelta)
{
	float delta = rhs - lhs;
	if (delta > 0.0F)
		return lhs + std::min(delta, clampedDelta);
	else
		return lhs - std::min(-delta, clampedDelta);
}

Vector3 Vector3::RotateTowards(const Vector3& current, const Vector3& target, float angleMove, float lengthMove)
{
	float curLength = Length(current);
	float targetLength = Length(target);

	// both vectors are non-zero
	if (curLength > Vector3::epsilon && targetLength > Vector3::epsilon)
	{
		Vector3 curNorm = current / curLength;
		Vector3 targetNorm = target / targetLength;

		float dot = Dot(curNorm, targetNorm);
		// angle is almost the same
		if (dot > 1.0F - Vector3::epsilon)
			return MoveTowards(current, target, lengthMove);	//fall back
		// directions are almost opposite
		else if (dot < -1.0F + Vector3::epsilon)
		{
			//cross will get small result to error
			Vector3 axis = OrthoNormalVectorFast(curNorm);
			Matrix3x3 m;
			m.SetAxisAngle(axis, angleMove);
			Vector3 rotated = m.MultiplyPoint3(curNorm);
			rotated *= ClampedMove(curLength, targetLength, lengthMove);
			return rotated;
		}
		// normal case
		else
		{
			float angle = std::acos(dot);
			Vector3 axis = Normalize(Cross(curNorm, targetNorm));
			Matrix3x3 m;
			m.SetAxisAngle(axis, std::min(angleMove, angle));
			Vector3 rotated = m.MultiplyPoint3(curNorm);
			rotated *= ClampedMove(curLength, targetLength, lengthMove);
			return rotated;
		}
	}
	else
	{
		//fall back
		return MoveTowards(current, target, lengthMove);
	}
}

// angle : linear
// len : linear
//final vector = angle - len
Vector3 Vector3::Slerp(const Vector3& lhs, const Vector3& rhs, float t)
{
	float lhsMag = Length(lhs);
	float rhsMag = Length(rhs);

	if (lhsMag < Vector3::epsilon || rhsMag < Vector3::epsilon)
		return Lerp(lhs, rhs, t);

	float lerpedMagnitude = ::Lerp(lhsMag, rhsMag, t);

	float dot = Dot(lhs, rhs) / (lhsMag * rhsMag);
	// direction is almost the same
	if (dot > 1.0F - Vector3::epsilon)
	{
		return Lerp(lhs, rhs, t);
	}
	// directions are almost opposite
	else if (dot < -1.0F + Vector3::epsilon)
	{
		Vector3 lhsNorm = lhs / lhsMag;
		Vector3 axis = OrthoNormalVectorFast(lhsNorm);
		Matrix3x3 m;
		m.SetAxisAngle(axis, kPI * t);
		Vector3 slerped = m.MultiplyPoint3(lhsNorm);
		slerped *= lerpedMagnitude;
		return slerped;
	}
	// normal case
	else
	{
		Vector3 axis = Cross(lhs, rhs);
		Vector3 lhsNorm = lhs / lhsMag;
		axis = Normalize(axis);
		float angle = std::acos(dot) * t;

		Matrix3x3 m;
		m.SetAxisAngle(axis, angle);
		Vector3 slerped = m.MultiplyPoint3(lhsNorm);
		slerped *= lerpedMagnitude;
		return slerped;
	}
}