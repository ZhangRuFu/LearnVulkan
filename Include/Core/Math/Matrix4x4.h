#pragma once

#include "Vector3.h"
#include "Vector4.h"
#include "TransformType.h"

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

//column major
class Matrix4x4
{
public:
	float m_data[16];

	enum InitFlag { Identity };

	Matrix4x4() {}
	explicit Matrix4x4(InitFlag) { SetIdentity(); }		//construct to identity

	Matrix4x4(const Matrix4x4 &other);
	Matrix4x4(const Matrix3x3 &other);
	explicit Matrix4x4(const float data[16]);

	//column first
	float& Get(int row, int column) { return m_data[row + (column * 4)]; }
	const float& Get(int row, int column) const { return m_data[row + (column * 4)]; }
	float* GetPtr() { return m_data; }
	const float* GetPtr() const { return m_data; }

	float operator[](int index) const { return m_data[index]; }
	float& operator[](int index) { return m_data[index]; }

	Matrix4x4& operator*=(const Matrix4x4& inM);
	
	Matrix4x4& operator=(const Matrix4x4& m);

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
	bool IsPerspective() const { return (m_data[3] != 0.0f || m_data[7] != 0.0f || m_data[11] != 0.0f || m_data[15] != 1.0f); }

	float GetDeterminant() const;

	Matrix4x4& Invert_Full()
	{
		InvertMatrix4x4_Full(m_data, m_data);
		return *this;
	}

	static bool Invert_Full(const Matrix4x4 &inM, Matrix4x4 &outM)
	{
		return InvertMatrix4x4_Full(inM.m_data, outM.m_data);
	}

	static bool Invert_General3D(const Matrix4x4 &inM, Matrix4x4 &outM)
	{
		return InvertMatrix4x4_General3D(inM.m_data, outM.m_data);
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

void MultiplyMatrices4x4(const Matrix4x4* __restrict lhs, const Matrix4x4* __restrict rhs, Matrix4x4* __restrict res);
void CopyMatrix4x4(const float* __restrict lhs, float* __restrict res);
void TransposeMatrix4x4(const Matrix4x4* __restrict lhs, Matrix4x4* __restrict res);

// foreach R[i] = A[i] * B[i]
void MultiplyMatrixArray4x4(const Matrix4x4* __restrict arrayA, const Matrix4x4* __restrict arrayB,
	Matrix4x4* __restrict arrayRes, size_t count);
// foreach R[i] = BASE * A[i] * B[i]
void MultiplyMatrixArrayWithBase4x4(const Matrix4x4* __restrict base,
	const Matrix4x4* __restrict arrayA, const Matrix4x4* __restrict arrayB,
	Matrix4x4* __restrict arrayRes, size_t count);