#pragma once

#include "Vector3.h"
#include "Vector4.h"
#include "TransformType.h"
//prefetch cache ?

//No SIMD

// Visual Studio generates bad code from x86 CopyMatrix4x4 SSE intrinsics. On x64 it uses SSE automatically.
// Only Neon benefits from custom copy constructors and assignment operators.
#define UNITY_USE_COPYMATRIX_4X4 (UNITY_SUPPORTS_NEON)

class Matrix3x3;
class Matrix4x4;
class Quaternionf;

struct FrustumPlanes
{
	float left;
	float right;
	float bottom;
	float top;
	float zNear;
	float zFar;
};

TransformType ComputeTransformType(const Matrix4x4& matrix, float epsilon = Vector3::epsilon);
float ComputeUniformScale(const Matrix4x4& matrix);

bool InvertMatrix4x4_Full(const float* m, float* out);
bool InvertMatrix4x4_General3D(const float* m, float* out);


/* Attention this! */
// Matrices in unity are column major
class Matrix4x4
{
public:
	float m_Data[16];

	enum InitIdentity { kIdentity };

	//performance reasons
	Matrix4x4() {}
	explicit Matrix4x4(InitIdentity) { SetIdentity(); }

#if UNITY_USE_COPYMATRIX_4X4
	Matrix4x4f(const Matrix4x4f &other);
#endif

	Matrix4x4(const Matrix3x3 &other);
	explicit Matrix4x4(const float data[16]);

	//column first
	float& Get(int row, int column) { return m_Data[row + (column * 4)]; }
	const float& Get(int row, int column) const { return m_Data[row + (column * 4)]; }
	float* GetPtr() { return m_Data; }
	const float* GetPtr() const { return m_Data; }

	float operator[](int index) const { return m_Data[index]; }
	float& operator[](int index) { return m_Data[index]; }

	Matrix4x4& operator*=(const Matrix4x4& inM);

#if UNITY_USE_COPYMATRIX_4X4
	Matrix4x4f& operator=(const Matrix4x4f& m);
#endif

	Matrix4x4& operator=(const Matrix3x3& m);

	Vector3 MultiplyVector3(const Vector3& inV) const;
	void MultiplyVector3(const Vector3& inV, Vector3& output) const;
	bool PerspectiveMultiplyVector3(const Vector3& inV, Vector3& output) const;
	Vector4 MultiplyVector4(const Vector4& inV) const;
	void MultiplyVector4(const Vector4& inV, Vector4& output) const;
	Vector3 MultiplyPoint3(const Vector3& inV) const;
	void MultiplyPoint3(const Vector3& inV, Vector3& output) const;
	bool PerspectiveMultiplyPoint3(const Vector3& inV, Vector3& output) const;
	Vector3 InverseMultiplyPoint3Affine(const Vector3& inV) const;
	Vector3 InverseMultiplyVector3Affine(const Vector3& inV) const;

	bool IsIdentity(float epsilon = Vector3::epsilon) const;
	// Returns whether a matrix is a perspective projection transform (i.e. doesn't have 0,0,0,1 in the last column).
	bool IsPerspective() const { return (m_Data[3] != 0.0f || m_Data[7] != 0.0f || m_Data[11] != 0.0f || m_Data[15] != 1.0f); }

	float GetDeterminant() const;

	Matrix4x4& Invert_Full()
	{
		InvertMatrix4x4_Full(m_Data, m_Data);
		return *this;
	}

	static bool Invert_Full(const Matrix4x4 &inM, Matrix4x4 &outM)
	{
		return InvertMatrix4x4_Full(inM.m_Data, outM.m_Data);
	}

	static bool Invert_General3D(const Matrix4x4 &inM, Matrix4x4 &outM)
	{
		return InvertMatrix4x4_General3D(inM.m_Data, outM.m_Data);
	}

	Matrix4x4& Transpose();

	Matrix4x4& SetIdentity();
	Matrix4x4& SetZero();
	Matrix4x4& SetPerspective(float fovy, float aspect, float zNear, float zFar);
	// rad = Deg2Rad(fovy/2), contanHalfFOV = cos(rad)/sin(rad)
	Matrix4x4& SetPerspectiveCotan(float cotanHalfFOV, float zNear, float zFar);
	Matrix4x4& SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
	Matrix4x4& SetFrustum(float left, float right, float bottom, float top, float nearval, float farval);
	Matrix4x4& AdjustDepthRange(float newNear, float newFar);

	Vector3 GetAxisX() const;
	Vector3 GetAxisY() const;
	Vector3 GetAxisZ() const;
	Vector3 GetAxis(int axis) const;
	Vector3 GetPosition() const;
	Quaternionf GetRotation() const;
	Vector3 GetLossyScale() const;
	Vector4 GetRow(int row) const;
	Vector4 GetColumn(int col) const;
	// these set only these components of the matrix, everything else is untouched!
	void SetAxisX(const Vector3& v);
	void SetAxisY(const Vector3& v);
	void SetAxisZ(const Vector3& v);
	void SetAxis(int axis, const Vector3& v);
	void SetPosition(const Vector3& v);
	void SetRow(int row, const Vector4& v);
	void SetColumn(int col, const Vector4& v);

	Matrix4x4& SetTranslate(const Vector3& inTrans);
	Matrix4x4& SetBasis(const Vector3& inX, const Vector3& inY, const Vector3& inZ);
	Matrix4x4& SetBasisTransposed(const Vector3& inX, const Vector3& inY, const Vector3& inZ);
	Matrix4x4& SetScale(const Vector3& inScale);
	Matrix4x4& SetPositionAndOrthoNormalBasis(const Vector3& inPosition, const Vector3& inX, const Vector3& inY, const Vector3& inZ);

	Matrix4x4& Translate(const Vector3& inTrans);
	Matrix4x4& Scale(const Vector3& inScale);

	Matrix4x4& SetFromToRotation(const Vector3& from, const Vector3& to);

	void SetTR(const Vector3& pos, const Quaternionf& q);
	void SetTRS(const Vector3& pos, const Quaternionf& q, const Vector3& s);
	void SetTRInverse(const Vector3& pos, const Quaternionf& q);
	FrustumPlanes DecomposeProjection() const;
	static const Matrix4x4 identity;
	bool ValidTRS() const;
};

bool CompareApproximately(const Matrix4x4& lhs, const Matrix4x4& rhs, float dist = Vector3::epsilon);

/// Transforms an array of vertices. input may be the same as output.
void TransformPoints3x3(const Matrix4x4 &matrix, const Vector3* input, Vector3* ouput, int count);
void TransformPoints3x4(const Matrix4x4 &matrix, const Vector3* input, Vector3* ouput, int count);
void TransformPoints3x3(const Matrix4x4 &matrix, const Vector3* input, size_t inStride, Vector3* ouput, size_t outStride, int count);
void TransformPoints3x4(const Matrix4x4 &matrix, const Vector3* input, size_t inStride, Vector3* ouput, size_t outStride, int count);

void MultiplyMatrices3x4(const Matrix4x4& lhs, const Matrix4x4& rhs, Matrix4x4& res);

void MultiplyMatrices4x4REF(const Matrix4x4* __restrict lhs, const Matrix4x4* __restrict rhs, Matrix4x4* __restrict res);
void CopyMatrix4x4REF(const float* __restrict lhs, float* __restrict res);
void TransposeMatrix4x4REF(const Matrix4x4* __restrict lhs, Matrix4x4* __restrict res);

// foreach R[i] = A[i] * B[i]
void MultiplyMatrixArray4x4REF(const Matrix4x4* __restrict arrayA, const Matrix4x4* __restrict arrayB,
	Matrix4x4* __restrict arrayRes, size_t count);
// foreach R[i] = BASE * A[i] * B[i]
void MultiplyMatrixArrayWithBase4x4REF(const Matrix4x4* __restrict base,
	const Matrix4x4* __restrict arrayA, const Matrix4x4* __restrict arrayB,
	Matrix4x4* __restrict arrayRes, size_t count);

#if UNITY_SUPPORTS_SSE
#include "Simd/Matrix4x4Simd.h"
#elif UNITY_SUPPORTS_NEON

#if UNITY_USE_PREFIX_EXTERN_SYMBOLS
#define MultiplyMatrices4x4_NEON                _MultiplyMatrices4x4_NEON
#define CopyMatrix4x4_NEON                      _CopyMatrix4x4_NEON
#define TransposeMatrix4x4_NEON                 _TransposeMatrix4x4_NEON
#define MultiplyMatrixArray4x4_NEON             _MultiplyMatrixArray4x4_NEON
#define MultiplyMatrixArrayWithBase4x4_NEON     _MultiplyMatrixArrayWithBase4x4_NEON
#endif

extern "C"
{
	void CopyMatrix4x4_NEON(const float* __restrict lhs, float* __restrict res);
	void TransposeMatrix4x4_NEON(const Matrix4x4f* __restrict lhs, Matrix4x4f* __restrict res);

	void MultiplyMatrices4x4_NEON(const Matrix4x4f* __restrict lhs, const Matrix4x4f* __restrict rhs, Matrix4x4f* __restrict res);
	void MultiplyMatrixArray4x4_NEON(const Matrix4x4f* __restrict arrayA, const Matrix4x4f* __restrict arrayB,
		Matrix4x4f* __restrict arrayRes, size_t count);
	void MultiplyMatrixArrayWithBase4x4_NEON(const Matrix4x4f* __restrict base,
		const Matrix4x4f* __restrict arrayA, const Matrix4x4f* __restrict arrayB,
		Matrix4x4f* __restrict arrayRes, size_t count);
}

#define CopyMatrix4x4       CopyMatrix4x4_NEON
#define TransposeMatrix4x4  TransposeMatrix4x4_NEON
#define MultiplyMatrices4x4 MultiplyMatrices4x4_NEON
#define MultiplyMatrixArray4x4          MultiplyMatrixArray4x4_NEON
#define MultiplyMatrixArrayWithBase4x4  MultiplyMatrixArrayWithBase4x4_NEON

#else

#define TransposeMatrix4x4              TransposeMatrix4x4REF
#define MultiplyMatrices4x4             MultiplyMatrices4x4REF
#define MultiplyMatrixArray4x4          MultiplyMatrixArray4x4REF
#define MultiplyMatrixArrayWithBase4x4  MultiplyMatrixArrayWithBase4x4REF

#endif

#if UNITY_USE_COPYMATRIX_4X4
inline Matrix4x4f::Matrix4x4f(const Matrix4x4f &other)
{
	CopyMatrix4x4(other.GetPtr(), GetPtr());
}

inline Matrix4x4f& Matrix4x4f::operator=(const Matrix4x4f& m)
{
	CopyMatrix4x4(m.GetPtr(), GetPtr());
	return *this;
}

#endif

inline Vector3 Matrix4x4::GetAxisX() const
{
	return Vector3(Get(0, 0), Get(1, 0), Get(2, 0));
}

inline Vector3 Matrix4x4::GetAxisY() const
{
	return Vector3(Get(0, 1), Get(1, 1), Get(2, 1));
}

inline Vector3 Matrix4x4::GetAxisZ() const
{
	return Vector3(Get(0, 2), Get(1, 2), Get(2, 2));
}

inline Vector3 Matrix4x4::GetAxis(int axis) const
{
	return Vector3(Get(0, axis), Get(1, axis), Get(2, axis));
}

inline Vector3 Matrix4x4::GetPosition() const
{
	return Vector3(Get(0, 3), Get(1, 3), Get(2, 3));
}

inline Vector4 Matrix4x4::GetRow(int row) const
{
	return Vector4(Get(row, 0), Get(row, 1), Get(row, 2), Get(row, 3));
}

inline Vector4 Matrix4x4::GetColumn(int col) const
{
	return Vector4(Get(0, col), Get(1, col), Get(2, col), Get(3, col));
}

inline void Matrix4x4::SetAxisX(const Vector3& v)
{
	Get(0, 0) = v.X; Get(1, 0) = v.Y; Get(2, 0) = v.Z;
}

inline void Matrix4x4::SetAxisY(const Vector3& v)
{
	Get(0, 1) = v.X; Get(1, 1) = v.Y; Get(2, 1) = v.Z;
}

inline void Matrix4x4::SetAxisZ(const Vector3& v)
{
	Get(0, 2) = v.X; Get(1, 2) = v.Y; Get(2, 2) = v.Z;
}

inline void Matrix4x4::SetAxis(int axis, const Vector3& v)
{
	Get(0, axis) = v.X; Get(1, axis) = v.Y; Get(2, axis) = v.Z;
}

inline void Matrix4x4::SetPosition(const Vector3& v)
{
	Get(0, 3) = v.X; Get(1, 3) = v.Y; Get(2, 3) = v.Z;
}

inline void Matrix4x4::SetRow(int row, const Vector4& v)
{
	Get(row, 0) = v.X; Get(row, 1) = v.Y; Get(row, 2) = v.Z; Get(row, 3) = v.W;
}

inline void Matrix4x4::SetColumn(int col, const Vector4& v)
{
	Get(0, col) = v.X; Get(1, col) = v.Y; Get(2, col) = v.Z; Get(3, col) = v.W;
}

inline Vector3 Matrix4x4::MultiplyPoint3(const Vector3& v) const
{
	Vector3 res;
	res.X = m_Data[0] * v.X + m_Data[4] * v.Y + m_Data[8] * v.Z + m_Data[12];
	res.Y = m_Data[1] * v.X + m_Data[5] * v.Y + m_Data[9] * v.Z + m_Data[13];
	res.Z = m_Data[2] * v.X + m_Data[6] * v.Y + m_Data[10] * v.Z + m_Data[14];
	return res;
}

inline void Matrix4x4::MultiplyPoint3(const Vector3& v, Vector3& output) const
{
	output.X = m_Data[0] * v.X + m_Data[4] * v.Y + m_Data[8] * v.Z + m_Data[12];
	output.Y = m_Data[1] * v.X + m_Data[5] * v.Y + m_Data[9] * v.Z + m_Data[13];
	output.Z = m_Data[2] * v.X + m_Data[6] * v.Y + m_Data[10] * v.Z + m_Data[14];
}

inline Vector3 Matrix4x4::MultiplyVector3(const Vector3& v) const
{
	Vector3 res;
	res.X = m_Data[0] * v.X + m_Data[4] * v.Y + m_Data[8] * v.Z;
	res.Y = m_Data[1] * v.X + m_Data[5] * v.Y + m_Data[9] * v.Z;
	res.Z = m_Data[2] * v.X + m_Data[6] * v.Y + m_Data[10] * v.Z;
	return res;
}

inline void Matrix4x4::MultiplyVector3(const Vector3& v, Vector3& output) const
{
	output.X = m_Data[0] * v.X + m_Data[4] * v.Y + m_Data[8] * v.Z;
	output.Y = m_Data[1] * v.X + m_Data[5] * v.Y + m_Data[9] * v.Z;
	output.Z = m_Data[2] * v.X + m_Data[6] * v.Y + m_Data[10] * v.Z;
}

inline bool Matrix4x4::PerspectiveMultiplyPoint3(const Vector3& v, Vector3& output) const
{
	Vector3 res;
	float w;
	res.X = Get(0, 0) * v.X + Get(0, 1) * v.Y + Get(0, 2) * v.Z + Get(0, 3);
	res.Y = Get(1, 0) * v.X + Get(1, 1) * v.Y + Get(1, 2) * v.Z + Get(1, 3);
	res.Z = Get(2, 0) * v.X + Get(2, 1) * v.Y + Get(2, 2) * v.Z + Get(2, 3);
	w = Get(3, 0) * v.X + Get(3, 1) * v.Y + Get(3, 2) * v.Z + Get(3, 3);
	if (abs(w) > 1.0e-7f)
	{
		float invW = 1.0f / w;
		output.X = res.X * invW;
		output.Y = res.Y * invW;
		output.Z = res.Z * invW;
		return true;
	}
	else
	{
		output.X = 0.0f;
		output.Y = 0.0f;
		output.Z = 0.0f;
		return false;
	}
}

inline Vector4 Matrix4x4::MultiplyVector4(const Vector4& v) const
{
	Vector4 res;
	MultiplyVector4(v, res);
	return res;
}

inline void Matrix4x4::MultiplyVector4(const Vector4& v, Vector4& output) const
{
	output.X = m_Data[0] * v.X + m_Data[4] * v.Y + m_Data[8] * v.Z + m_Data[12] * v.W;
	output.Y = m_Data[1] * v.X + m_Data[5] * v.Y + m_Data[9] * v.Z + m_Data[13] * v.W;
	output.Z = m_Data[2] * v.X + m_Data[6] * v.Y + m_Data[10] * v.Z + m_Data[14] * v.W;
	output.W = m_Data[3] * v.X + m_Data[7] * v.Y + m_Data[11] * v.Z + m_Data[15] * v.W;
}

inline bool Matrix4x4::PerspectiveMultiplyVector3(const Vector3& v, Vector3& output) const
{
	Vector3 res;
	float w;
	res.X = Get(0, 0) * v.X + Get(0, 1) * v.Y + Get(0, 2) * v.Z;
	res.Y = Get(1, 0) * v.X + Get(1, 1) * v.Y + Get(1, 2) * v.Z;
	res.Z = Get(2, 0) * v.X + Get(2, 1) * v.Y + Get(2, 2) * v.Z;
	w = Get(3, 0) * v.X + Get(3, 1) * v.Y + Get(3, 2) * v.Z;
	if (abs(w) > 1.0e-7f)
	{
		float invW = 1.0f / w;
		output.X = res.X * invW;
		output.Y = res.Y * invW;
		output.Z = res.Z * invW;
		return true;
	}
	else
	{
		output.X = 0.0f;
		output.Y = 0.0f;
		output.Z = 0.0f;
		return false;
	}
}

inline Vector3 Matrix4x4::InverseMultiplyPoint3Affine(const Vector3& inV) const
{
	Vector3 v(inV.X - Get(0, 3), inV.Y - Get(1, 3), inV.Z - Get(2, 3));
	Vector3 res;
	res.X = Get(0, 0) * v.X + Get(1, 0) * v.Y + Get(2, 0) * v.Z;
	res.Y = Get(0, 1) * v.X + Get(1, 1) * v.Y + Get(2, 1) * v.Z;
	res.Z = Get(0, 2) * v.X + Get(1, 2) * v.Y + Get(2, 2) * v.Z;
	return res;
}

inline Vector3 Matrix4x4::InverseMultiplyVector3Affine(const Vector3& v) const
{
	Vector3 res;
	res.X = Get(0, 0) * v.X + Get(1, 0) * v.Y + Get(2, 0) * v.Z;
	res.Y = Get(0, 1) * v.X + Get(1, 1) * v.Y + Get(2, 1) * v.Z;
	res.Z = Get(0, 2) * v.X + Get(1, 2) * v.Y + Get(2, 2) * v.Z;
	return res;
}

inline bool IsFinite(const Matrix4x4& f)
{
	return
		isfinite(f.m_Data[0]) & isfinite(f.m_Data[1]) & isfinite(f.m_Data[2]) &
		isfinite(f.m_Data[4]) & isfinite(f.m_Data[5]) & isfinite(f.m_Data[6]) &
		isfinite(f.m_Data[8]) & isfinite(f.m_Data[9]) & isfinite(f.m_Data[10]) &
		isfinite(f.m_Data[12]) & isfinite(f.m_Data[13]) & isfinite(f.m_Data[14]) & isfinite(f.m_Data[15]);
}
