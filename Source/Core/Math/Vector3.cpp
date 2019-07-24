#include "Core\Math\Vector3.h"
#include "Core\Math\Matrix3x3.h"

#define FPFIXES 1


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

inline float Vector3::SqrMagnitude(const Vector3 & inV)
{
	return Dot(inV, inV);
}

inline float Vector3::Magnitude(const Vector3 & inV)
{
	return sqrt(Dot(inV, inV));
}

inline float Vector3::Angle(const Vector3 & lhs, const Vector3 & rhs)
{
	return ::acos(std::min(1.0f, std::max(-1.0f, Dot(lhs, rhs) / (Magnitude(lhs) * Magnitude(rhs)))));
}

inline Vector3 Vector3::Normalize(const Vector3 & inV)
{
	return inV / Magnitude(inV);
}

inline Vector3 Vector3::NormalizeSafe(const Vector3 & inV, const Vector3 & defaultV)
{
	float mag = Magnitude(inV);
	if (mag > Vector3::epsilon)
		return inV / mag;
	else
		return defaultV;
}

// - Handles zero vector correclty
inline Vector3 Vector3::NormalizeFast(const Vector3 & inV)
{
	float m = SqrMagnitude(inV);
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
	float m = SqrMagnitude(inV);
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
	return SqrMagnitude(inV1 - inV0) <= inMaxDist * inMaxDist;
}

inline bool Vector3::IsNormalized(const Vector3 & vec, float epsilon)
{
	return ::CompareApproximately(SqrMagnitude(vec), 1.0f, epsilon);
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
	float mag = Magnitude(*inU);
	if (mag > Vector3::epsilon)
		*inU /= mag;
	else
		*inU = Vector3(1.0F, 0.0F, 0.0F);

	// compute u1
	float dot0 = Dot(*inU, *inV);
	*inV -= dot0 * *inU;
	mag = Magnitude(*inV);
	if (mag < Vector3::epsilon)
		*inV = OrthoNormalVectorFast(*inU);
	else
		*inV /= mag;
}

void Vector3::OrthoNormalize(Vector3* inU, Vector3* inV, Vector3* inW)
{
	// compute u0
	float mag = Magnitude(*inU);
	if (mag > Vector3::epsilon)
		*inU /= mag;
	else
		*inU = Vector3(1.0F, 0.0F, 0.0F);

	// compute u1
	float dot0 = Dot(*inU, *inV);
	*inV -= dot0 * *inU;
	mag = Magnitude(*inV);
	if (mag > Vector3::epsilon)
		*inV /= mag;
	else
		*inV = OrthoNormalVectorFast(*inU);

	// compute u2
	float dot1 = Dot(*inV, *inW);
	dot0 = Dot(*inU, *inW);
	*inW -= dot0 * *inU + dot1 * *inV;
	mag = Magnitude(*inW);
	if (mag > Vector3::epsilon)
		*inW /= mag;
	else
		*inW = Cross(*inU, *inV);
}

#define k1OverSqrt2 float(0.7071067811865475244008443621048490)

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

/* from chris hecker (Generates Orthonormal basis)
void
DextralBases(real32 const *XAxis, real32 *YAxis, real32 *ZAxis)
{
real32 CrossVector[3] = {1.0f, 1.0f, 1.0f};

real32 MaximumElement = 0.0f;

int MaximumElementIndex = 0;
{for(int ElementIndex = 0;
ElementIndex < 3;
++ElementIndex)
{
real32 ElementValue = AbsoluteValue(XAxis[ElementIndex]);
if(ElementValue > MaximumElement)
{
MaximumElement = ElementValue;
MaximumElementIndex = ElementIndex;
}
}}

CrossVector[MaximumElementIndex] = 0.0f;

VectorCrossProduct3(YAxis, CrossVector, XAxis);
Normalize3(YAxis);

VectorCrossProduct3(ZAxis, XAxis, YAxis);
Normalize3(ZAxis);
}

*/

/// Returns a Vector3 that moves lhs towards rhs by a maximum of clampedDistance
Vector3 Vector3::MoveTowards(const Vector3& lhs, const Vector3& rhs, float clampedDistance)
{
	Vector3 delta = rhs - lhs;
	float sqrDelta = SqrMagnitude(delta);
	float sqrClampedDistance = clampedDistance * clampedDistance;
	if (sqrDelta > sqrClampedDistance)
	{
		float deltaMag = sqrt(sqrDelta);
		if (deltaMag > Vector3::epsilon)
			return lhs + delta / deltaMag * clampedDistance;
		else
			return lhs;
	}
	else
		return rhs;
}

static inline float ClampedMove(float lhs, float rhs, float clampedDelta)
{
	float delta = rhs - lhs;
	if (delta > 0.0F)
		return lhs + std::min(delta, clampedDelta);
	else
		return lhs - std::min(-delta, clampedDelta);
}

Vector3 Vector3::RotateTowards(const Vector3& lhs, const Vector3& rhs, float angleMove, float magnitudeMove)
{
	float lhsMag = Magnitude(lhs);
	float rhsMag = Magnitude(rhs);

	// both vectors are non-zero
	if (lhsMag > Vector3::epsilon && rhsMag > Vector3::epsilon)
	{
		Vector3 lhsNorm = lhs / lhsMag;
		Vector3 rhsNorm = rhs / rhsMag;

		float dot = Dot(lhsNorm, rhsNorm);
		// direction is almost the same
		if (dot > 1.0F - Vector3::epsilon)
		{
			return MoveTowards(lhs, rhs, magnitudeMove);
		}
		// directions are almost opposite
		else if (dot < -1.0F + Vector3::epsilon)
		{
			Vector3 axis = OrthoNormalVectorFast(lhsNorm);
			Matrix3x3 m;
			m.SetAxisAngle(axis, angleMove);
			Vector3 rotated = m.MultiplyPoint3(lhsNorm);
			rotated *= ClampedMove(lhsMag, rhsMag, magnitudeMove);
			return rotated;
		}
		// normal case
		else
		{
			float angle = std::acos(dot);
			Vector3 axis = Normalize(Cross(lhsNorm, rhsNorm));
			Matrix3x3 m;
			m.SetAxisAngle(axis, std::min(angleMove, angle));
			Vector3 rotated = m.MultiplyPoint3(lhsNorm);
			rotated *= ClampedMove(lhsMag, rhsMag, magnitudeMove);
			return rotated;
		}
	}
	// at least one of the vectors is almost zero
	else
	{
		return MoveTowards(lhs, rhs, magnitudeMove);
	}
}

Vector3 Vector3::Slerp(const Vector3& lhs, const Vector3& rhs, float t)
{
	float lhsMag = Magnitude(lhs);
	float rhsMag = Magnitude(rhs);

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

inline static Vector3 NormalizeRobustInternal(const Vector3& a, float &l, float &div, float eps)
{
	float a0, a1, a2, aa0, aa1, aa2;
	a0 = a[0];
	a1 = a[1];
	a2 = a[2];

#if FPFIXES
	if (CompareApproximately(a0, 0.0F, eps))
		a0 = aa0 = 0;
	else
#endif
	{
		aa0 = abs(a0);
	}

#if FPFIXES
	if (CompareApproximately(a1, 0.0F, eps))
		a1 = aa1 = 0;
	else
#endif
	{
		aa1 = abs(a1);
	}

#if FPFIXES
	if (CompareApproximately(a2, 0.0F, eps))
		a2 = aa2 = 0;
	else
#endif
	{
		aa2 = abs(a2);
	}

	if (aa1 > aa0)
	{
		if (aa2 > aa1)
		{
			a0 /= aa2;
			a1 /= aa2;
			l = InvSqrt(a0 * a0 + a1 * a1 + 1.0F);
			div = aa2;
			return Vector3(a0 * l, a1 * l, CopySignf(l, a2));
		}
		else
		{
			// aa1 is largest
			a0 /= aa1;
			a2 /= aa1;
			l = InvSqrt(a0 * a0 + a2 * a2 + 1.0F);
			div = aa1;
			return Vector3(a0 * l, CopySignf(l, a1), a2 * l);
		}
	}
	else
	{
		if (aa2 > aa0)
		{
			// aa2 is largest
			a0 /= aa2;
			a1 /= aa2;
			l = InvSqrt(a0 * a0 + a1 * a1 + 1.0F);
			div = aa2;
			return Vector3(a0 * l, a1 * l, CopySignf(l, a2));
		}
		else
		{
			// aa0 is largest
			if (aa0 <= 0)
			{
				l = 0;
				div = 1;
				return Vector3(0.0F, 1.0F, 0.0F);
			}

			a1 /= aa0;
			a2 /= aa0;
			l = InvSqrt(a1 * a1 + a2 * a2 + 1.0F);
			div = aa0;
			return Vector3(CopySignf(l, a0), a1 * l, a2 * l);
		}
	}
}

Vector3 Vector3::NormalizeRobust(const Vector3& a)
{
	float l, div;
	return NormalizeRobustInternal(a, l, div, Vector3::epsilon);
}

Vector3 Vector3::NormalizeRobust(const Vector3& a, float &invOriginalLength, float eps)
{
	float l, div;
	const Vector3 &n = NormalizeRobustInternal(a, l, div, eps);
	invOriginalLength = l / div;
	// guard for NaNs
	/* TODO : Assert */
	/*
	Assert(n == n);
	Assert(invOriginalLength == invOriginalLength);
	Assert(IsNormalized(n));
	*/

	return n;
}