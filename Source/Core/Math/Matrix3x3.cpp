#include "Core\Math\Matrix3x3.h"
#include "Core\Math\Matrix4x4.h"

namespace
{
	Matrix3x3 CreateIdentityMatrix3x3()
	{
		Matrix3x3 temp;
		temp.SetIdentity();
		return temp;
	}

	Matrix3x3 CreateZeroMatrix3x3()
	{
		Matrix3x3 temp;
		temp.SetZero();
		return temp;
	}
}

const Matrix3x3 Matrix3x3::identity = CreateIdentityMatrix3x3();
const Matrix3x3 Matrix3x3::zero = CreateZeroMatrix3x3();

void GetRotMatrixNormVec(float* out, const float* inVec, float radians);

inline Vector3 Matrix3x3::MultiplyVector3(const Vector3& v) const
{
	Vector3 res;
	res.X = m_data[0] * v.X + m_data[3] * v.Y + m_data[6] * v.Z;
	res.Y = m_data[1] * v.X + m_data[4] * v.Y + m_data[7] * v.Z;
	res.Z = m_data[2] * v.X + m_data[5] * v.Y + m_data[8] * v.Z;
	return res;
}

inline void Matrix3x3::MultiplyVector3(const Vector3& v, Vector3& output) const
{
	output.X = m_data[0] * v.X + m_data[3] * v.Y + m_data[6] * v.Z;
	output.Y = m_data[1] * v.X + m_data[4] * v.Y + m_data[7] * v.Z;
	output.Z = m_data[2] * v.X + m_data[5] * v.Y + m_data[8] * v.Z;
}

inline Vector3 Matrix3x3::MultiplyVector3Transpose(const Vector3& v) const
{
	Vector3 res;
	res.X = Get(0, 0) * v.X + Get(1, 0) * v.Y + Get(2, 0) * v.Z;
	res.Y = Get(0, 1) * v.X + Get(1, 1) * v.Y + Get(2, 1) * v.Z;
	res.Z = Get(0, 2) * v.X + Get(1, 2) * v.Y + Get(2, 2) * v.Z;
	return res;
}


Matrix3x3& Matrix3x3::operator=(const Matrix4x4& mat4)
{
	m_data[0] = mat4.m_Data[0];
	m_data[1] = mat4.m_Data[1];
	m_data[2] = mat4.m_Data[2];

	m_data[3] = mat4.m_Data[4];
	m_data[4] = mat4.m_Data[5];
	m_data[5] = mat4.m_Data[6];

	m_data[6] = mat4.m_Data[8];
	m_data[7] = mat4.m_Data[9];
	m_data[8] = mat4.m_Data[10];
	return *this;
}

Matrix3x3::Matrix3x3(const Matrix4x4& mat4)
{
	m_data[0] = mat4.m_Data[0];
	m_data[1] = mat4.m_Data[1];
	m_data[2] = mat4.m_Data[2];

	m_data[3] = mat4.m_Data[4];
	m_data[4] = mat4.m_Data[5];
	m_data[5] = mat4.m_Data[6];

	m_data[6] = mat4.m_Data[8];
	m_data[7] = mat4.m_Data[9];
	m_data[8] = mat4.m_Data[10];
}


Matrix3x3& Matrix3x3::SetIdentity()
{
	Get(0, 0) = 1.0F;  Get(0, 1) = 0.0F;  Get(0, 2) = 0.0F;
	Get(1, 0) = 0.0F;  Get(1, 1) = 1.0F;  Get(1, 2) = 0.0F;
	Get(2, 0) = 0.0F;  Get(2, 1) = 0.0F;  Get(2, 2) = 1.0F;
	return *this;
}

Matrix3x3& Matrix3x3::SetZero()
{
	Get(0, 0) = 0.0F;  Get(0, 1) = 0.0F;  Get(0, 2) = 0.0F;
	Get(1, 0) = 0.0F;  Get(1, 1) = 0.0F;  Get(1, 2) = 0.0F;
	Get(2, 0) = 0.0F;  Get(2, 1) = 0.0F;  Get(2, 2) = 0.0F;
	return *this;
}


Matrix3x3& Matrix3x3::SetBasis(const Vector3& inX, const Vector3& inY, const Vector3& inZ)
{
	Get(0, 0) = inX[0];    Get(0, 1) = inY[0];    Get(0, 2) = inZ[0];
	Get(1, 0) = inX[1];    Get(1, 1) = inY[1];    Get(1, 2) = inZ[1];
	Get(2, 0) = inX[2];    Get(2, 1) = inY[2];    Get(2, 2) = inZ[2];
	return *this;
}

Matrix3x3& Matrix3x3::SetBasisTransposed(const Vector3& inX, const Vector3& inY, const Vector3& inZ)
{
	Get(0, 0) = inX[0];    Get(1, 0) = inY[0];    Get(2, 0) = inZ[0];
	Get(0, 1) = inX[1];    Get(1, 1) = inY[1];    Get(2, 1) = inZ[1];
	Get(0, 2) = inX[2];    Get(1, 2) = inY[2];    Get(2, 2) = inZ[2];
	return *this;
}


Matrix3x3& Matrix3x3::SetScale(const Vector3& inScale)
{
	Get(0, 0) = inScale[0];    Get(0, 1) = 0.0F;          Get(0, 2) = 0.0F;
	Get(1, 0) = 0.0F;          Get(1, 1) = inScale[1];    Get(1, 2) = 0.0F;
	Get(2, 0) = 0.0F;          Get(2, 1) = 0.0F;          Get(2, 2) = inScale[2];
	return *this;
}

Matrix3x3& Matrix3x3::Scale(const Vector3& inScale)
{
	Get(0, 0) *= inScale[0];
	Get(1, 0) *= inScale[0];
	Get(2, 0) *= inScale[0];

	Get(0, 1) *= inScale[1];
	Get(1, 1) *= inScale[1];
	Get(2, 1) *= inScale[1];

	Get(0, 2) *= inScale[2];
	Get(1, 2) *= inScale[2];
	Get(2, 2) *= inScale[2];
	return *this;
}


bool Matrix3x3::IsIdentity(float threshold)
{
	if (CompareApproximately(Get(0, 0), 1.0f, threshold) && CompareApproximately(Get(0, 1), 0.0f, threshold) && CompareApproximately(Get(0, 2), 0.0f, threshold) &&
		CompareApproximately(Get(1, 0), 0.0f, threshold) && CompareApproximately(Get(1, 1), 1.0f, threshold) && CompareApproximately(Get(1, 2), 0.0f, threshold) &&
		CompareApproximately(Get(2, 0), 0.0f, threshold) && CompareApproximately(Get(2, 1), 0.0f, threshold) && CompareApproximately(Get(2, 2), 1.0f, threshold))
		return true;
	return false;
}


float Matrix3x3::GetDeterminant() const
{
	float fCofactor0 = Get(0, 0) * Get(1, 1) * Get(2, 2);
	float fCofactor1 = Get(0, 1) * Get(1, 2) * Get(2, 0);
	float fCofactor2 = Get(0, 2) * Get(1, 0) * Get(2, 1);

	float fCofactor3 = Get(0, 2) * Get(1, 1) * Get(2, 0);
	float fCofactor4 = Get(0, 1) * Get(1, 0) * Get(2, 2);
	float fCofactor5 = Get(0, 0) * Get(1, 2) * Get(2, 1);

	return fCofactor0 + fCofactor1 + fCofactor2 - fCofactor3 - fCofactor4 - fCofactor5;
}

Matrix3x3& Matrix3x3::Transpose()
{
	std::swap(Get(0, 1), Get(1, 0));
	std::swap(Get(0, 2), Get(2, 0));
	std::swap(Get(2, 1), Get(1, 2));
	return *this;
}

bool Matrix3x3::Invert()
{
	///@TODO make a fast but robust inverse matrix 3x3
	Matrix4x4 m = *this;
	bool success = InvertMatrix4x4_Full(m.GetPtr(), m.GetPtr());
	*this = m;
	return success;

#if 0
	////// THIS METHOD IS NUMERICALLY LESS ROBUST
	// Invert a 3x3 using cofactors.  This is faster than using a generic
	// Gaussian elimination because of the loop overhead of such a method.

	Matrix3x3f kInverse;

	kInverse.Get(0, 0) = Get(1, 1) * Get(2, 2) - Get(1, 2) * Get(2, 1);
	kInverse.Get(0, 1) = Get(0, 2) * Get(2, 1) - Get(0, 1) * Get(2, 2);
	kInverse.Get(0, 2) = Get(0, 1) * Get(1, 2) - Get(0, 2) * Get(1, 1);
	kInverse.Get(1, 0) = Get(1, 2) * Get(2, 0) - Get(1, 0) * Get(2, 2);
	kInverse.Get(1, 1) = Get(0, 0) * Get(2, 2) - Get(0, 2) * Get(2, 0);
	kInverse.Get(1, 2) = Get(0, 2) * Get(1, 0) - Get(0, 0) * Get(1, 2);
	kInverse.Get(2, 0) = Get(1, 0) * Get(2, 1) - Get(1, 1) * Get(2, 0);
	kInverse.Get(2, 1) = Get(0, 1) * Get(2, 0) - Get(0, 0) * Get(2, 1);
	kInverse.Get(2, 2) = Get(0, 0) * Get(1, 1) - Get(0, 1) * Get(1, 0);

	float fDet = Get(0, 0) * kInverse.Get(0, 0) + Get(0, 1) * kInverse.Get(1, 0) + Get(0, 2) * kInverse.Get(2, 0);

	if (Abs(fDet) > Vector3f::epsilon)
	{
		kInverse /= fDet;
		*this = kInverse;
		return true;
	}
	else
	{
		SetZero();
		return false;
	}
#endif
}

void Matrix3x3::InvertTranspose()
{
	Invert();
	Transpose();
}

Matrix3x3& Matrix3x3::operator*=(float f)
{
	for (int i = 0; i < 9; i++)
		m_data[i] *= f;
	return *this;
}

Matrix3x3& Matrix3x3::operator*=(const Matrix3x3& inM)
{
	int i;
	for (i = 0; i < 3; i++)
	{
		float v[3] = { Get(i, 0), Get(i, 1), Get(i, 2) };
		Get(i, 0) = v[0] * inM.Get(0, 0) + v[1] * inM.Get(1, 0) + v[2] * inM.Get(2, 0);
		Get(i, 1) = v[0] * inM.Get(0, 1) + v[1] * inM.Get(1, 1) + v[2] * inM.Get(2, 1);
		Get(i, 2) = v[0] * inM.Get(0, 2) + v[1] * inM.Get(1, 2) + v[2] * inM.Get(2, 2);
	}
	return *this;
}

Matrix3x3& Matrix3x3::operator*=(const Matrix4x4& inM)
{
	int i;
	for (i = 0; i < 3; i++)
	{
		float v[3] = { Get(i, 0), Get(i, 1), Get(i, 2) };
		Get(i, 0) = v[0] * inM.Get(0, 0) + v[1] * inM.Get(1, 0) + v[2] * inM.Get(2, 0);
		Get(i, 1) = v[0] * inM.Get(0, 1) + v[1] * inM.Get(1, 1) + v[2] * inM.Get(2, 1);
		Get(i, 2) = v[0] * inM.Get(0, 2) + v[1] * inM.Get(1, 2) + v[2] * inM.Get(2, 2);
	}
	return *this;
}

Matrix3x3& Matrix3x3::SetAxisAngle(const Vector3& rotationAxis, float radians)
{
	GetRotMatrixNormVec(m_data, rotationAxis.GetPtr(), radians);
	return *this;
}

void EulerToMatrix(const Vector3& v, Matrix3x3& matrix)
{
	float cx = cos(v.X);
	float sx = sin(v.X);
	float cy = cos(v.Y);
	float sy = sin(v.Y);
	float cz = cos(v.Z);
	float sz = sin(v.Z);

	matrix.Get(0, 0) = cy * cz + sx * sy * sz;
	matrix.Get(0, 1) = cz * sx * sy - cy * sz;
	matrix.Get(0, 2) = cx * sy;

	matrix.Get(1, 0) = cx * sz;
	matrix.Get(1, 1) = cx * cz;
	matrix.Get(1, 2) = -sx;

	matrix.Get(2, 0) = -cz * sy + cy * sx * sz;
	matrix.Get(2, 1) = cy * cz * sx + sy * sz;
	matrix.Get(2, 2) = cx * cy;
}

/*
* A function for creating a rotation matrix that rotates a vector called
* "from" into another vector called "to".
* Input: from, to which both must be *normalized* non-zero vectors
* Output: a 3x3 matrix in colum-major form
* Author: Tomas Möller, 1999
*/
Matrix3x3& Matrix3x3::SetFromToRotation(const Vector3& from, const Vector3& to)
{
	Vector3 v = Vector3::Cross(from, to);
	float e = Vector3::Dot(from, to);
	const float kEpsilon = 0.000001f;
	if (e > 1.0 - kEpsilon)     /* "from" almost or equal to "to"-vector? */
	{
		/* return identity */
		Get(0, 0) = 1.0; Get(0, 1) = 0.0; Get(0, 2) = 0.0;
		Get(1, 0) = 0.0; Get(1, 1) = 1.0; Get(1, 2) = 0.0;
		Get(2, 0) = 0.0; Get(2, 1) = 0.0; Get(2, 2) = 1.0;
	}
	else if (e < -1.0 + kEpsilon) /* "from" almost or equal to negated "to"? */
	{
		float invlen;
		float fxx, fyy, fzz, fxy, fxz, fyz;
		float uxx, uyy, uzz, uxy, uxz, uyz;
		float lxx, lyy, lzz, lxy, lxz, lyz;
		/* left=CROSS(from, (1,0,0)) */
		Vector3 left(0.0f, from[2], -from[1]);
		if (Vector3::Dot(left, left) < kEpsilon) /* was left=CROSS(from,(1,0,0)) a good choice? */
		{
			/* here we now that left = CROSS(from, (1,0,0)) will be a good choice */
			left[0] = -from[2]; left[1] = 0.0; left[2] = from[0];
		}
		/* normalize "left" */
		invlen = 1.0f / std::sqrt(Vector3::Dot(left, left));
		left[0] *= invlen;
		left[1] *= invlen;
		left[2] *= invlen;
		Vector3 up = Vector3::Cross(left, from);
		/* now we have a coordinate system, i.e., a basis;    */
		/* M=(from, up, left), and we want to rotate to:      */
		/* N=(-from, up, -left). This is done with the matrix:*/
		/* N*M^T where M^T is the transpose of M              */
		fxx = -from[0] * from[0]; fyy = -from[1] * from[1]; fzz = -from[2] * from[2];
		fxy = -from[0] * from[1]; fxz = -from[0] * from[2]; fyz = -from[1] * from[2];

		uxx = up[0] * up[0]; uyy = up[1] * up[1]; uzz = up[2] * up[2];
		uxy = up[0] * up[1]; uxz = up[0] * up[2]; uyz = up[1] * up[2];

		lxx = -left[0] * left[0]; lyy = -left[1] * left[1]; lzz = -left[2] * left[2];
		lxy = -left[0] * left[1]; lxz = -left[0] * left[2]; lyz = -left[1] * left[2];
		/* symmetric matrix */
		Get(0, 0) = fxx + uxx + lxx; Get(0, 1) = fxy + uxy + lxy; Get(0, 2) = fxz + uxz + lxz;
		Get(1, 0) = Get(0, 1);   Get(1, 1) = fyy + uyy + lyy; Get(1, 2) = fyz + uyz + lyz;
		Get(2, 0) = Get(0, 2);   Get(2, 1) = Get(1, 2);   Get(2, 2) = fzz + uzz + lzz;
	}
	else  /* the most common case, unless "from"="to", or "from"=-"to" */
	{
		/* ...otherwise use this hand optimized version (9 mults less) */
		float hvx, hvz, hvxy, hvxz, hvyz;
		float h = (1.0f - e) / Vector3::Dot(v, v);
		hvx = h * v[0];
		hvz = h * v[2];
		hvxy = hvx * v[1];
		hvxz = hvx * v[2];
		hvyz = hvz * v[1];
		Get(0, 0) = e + hvx * v[0]; Get(0, 1) = hvxy - v[2];     Get(0, 2) = hvxz + v[1];
		Get(1, 0) = hvxy + v[2];  Get(1, 1) = e + h * v[1] * v[1]; Get(1, 2) = hvyz - v[0];
		Get(2, 0) = hvxz - v[1];  Get(2, 1) = hvyz + v[0];     Get(2, 2) = e + hvz * v[2];
	}
	return *this;
}

// Right handed
bool LookRotationToMatrix(const Vector3& viewVec, const Vector3& upVec, Matrix3x3* m)
{
	Vector3 z = viewVec;
	// compute u0
	float mag = Vector3::Length(z);
	if (mag < Vector3::epsilon)
	{
		m->SetIdentity();
		return false;
	}
	z /= mag;

	Vector3 x = Vector3::Cross(upVec, z);
	mag = Vector3::Length(x);
	if (mag < Vector3::epsilon)
	{
		m->SetIdentity();
		return false;
	}
	x /= mag;

	Vector3 y(Vector3::Cross(z, x));
	if (!CompareApproximately(Vector3::SqrLength(y), 1.0F))
		return false;

	m->SetBasis(x, y, z);
	return true;
}

/*
//Left handed
bool LookRotationToMatrixLeftHanded (const Vector3f& viewVec, const Vector3f& upVec, Matrix3x3f* m)
{
Vector3f z = viewVec;
// compute u0
float mag = Magnitude (z);
if (mag < Vector3f::epsilon)
return false;
z /= mag;

Vector3f x = Cross (z, upVec);
mag = Magnitude (x);
if (mag < Vector3f::epsilon)
return false;
x /= mag;

Vector3f y (Cross (x, z));
if (!CompareApproximately (SqrMagnitude (y), 1.0F))
return false;

m->SetBasis (x, y, z);
return true;
}
*/

void GetRotMatrixNormVec(float* out, const float* inVec, float radians)
{
	/* This function contributed by Erich Boleyn (erich@uruk.org) */
	/* This function used from the Mesa OpenGL code (matrix.c)  */
	float s, c;
	float vx, vy, vz, xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;

	s = sin(radians);
	c = cos(radians);

	vx = inVec[0];
	vy = inVec[1];
	vz = inVec[2];

#define M(row, col)  out[row*3 + col]
	/*
	*     Arbitrary axis rotation matrix.
	*
	*  This is composed of 5 matrices, Rz, Ry, T, Ry', Rz', multiplied
	*  like so:  Rz * Ry * T * Ry' * Rz'.  T is the final rotation
	*  (which is about the X-axis), and the two composite transforms
	*  Ry' * Rz' and Rz * Ry are (respectively) the rotations necessary
	*  from the arbitrary axis to the X-axis then back.  They are
	*  all elementary rotations.
	*
	*  Rz' is a rotation about the Z-axis, to bring the axis vector
	*  into the x-z plane.  Then Ry' is applied, rotating about the
	*  Y-axis to bring the axis vector parallel with the X-axis.  The
	*  rotation about the X-axis is then performed.  Ry and Rz are
	*  simply the respective inverse transforms to bring the arbitrary
	*  axis back to its original orientation.  The first transforms
	*  Rz' and Ry' are considered inverses, since the data from the
	*  arbitrary axis gives you info on how to get to it, not how
	*  to get away from it, and an inverse must be applied.
	*
	*  The basic calculation used is to recognize that the arbitrary
	*  axis vector (x, y, z), since it is of unit length, actually
	*  represents the sines and cosines of the angles to rotate the
	*  X-axis to the same orientation, with theta being the angle about
	*  Z and phi the angle about Y (in the order described above)
	*  as follows:
	*
	*  cos ( theta ) = x / sqrt ( 1 - z^2 )
	*  sin ( theta ) = y / sqrt ( 1 - z^2 )
	*
	*  cos ( phi ) = sqrt ( 1 - z^2 )
	*  sin ( phi ) = z
	*
	*  Note that cos ( phi ) can further be inserted to the above
	*  formulas:
	*
	*  cos ( theta ) = x / cos ( phi )
	*  sin ( theta ) = y / cos ( phi )
	*
	*  ...etc.  Because of those relations and the standard trigonometric
	*  relations, it is pssible to reduce the transforms down to what
	*  is used below.  It may be that any primary axis chosen will give the
	*  same results (modulo a sign convention) using thie method.
	*
	*  Particularly nice is to notice that all divisions that might
	*  have caused trouble when parallel to certain planes or
	*  axis go away with care paid to reducing the expressions.
	*  After checking, it does perform correctly under all cases, since
	*  in all the cases of division where the denominator would have
	*  been zero, the numerator would have been zero as well, giving
	*  the expected result.
	*/

	xx = vx * vx;
	yy = vy * vy;
	zz = vz * vz;
	xy = vx * vy;
	yz = vy * vz;
	zx = vz * vx;
	xs = vx * s;
	ys = vy * s;
	zs = vz * s;
	one_c = 1.0F - c;

	M(0, 0) = (one_c * xx) + c;
	M(1, 0) = (one_c * xy) - zs;
	M(2, 0) = (one_c * zx) + ys;

	M(0, 1) = (one_c * xy) + zs;
	M(1, 1) = (one_c * yy) + c;
	M(2, 1) = (one_c * yz) - xs;

	M(0, 2) = (one_c * zx) - ys;
	M(1, 2) = (one_c * yz) + xs;
	M(2, 2) = (one_c * zz) + c;

#undef M
}

void OrthoNormalize(Matrix3x3& matrix)
{
	Vector3* c0 = (Vector3*)matrix.GetPtr() + 0;
	Vector3* c1 = (Vector3*)matrix.GetPtr() + 3;
	Vector3* c2 = (Vector3*)matrix.GetPtr() + 6;
	Vector3::OrthoNormalize(c0, c1, c2);
}
