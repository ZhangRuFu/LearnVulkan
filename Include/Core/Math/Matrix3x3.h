#pragma once

#include "Vector3.h"

class Matrix3x3
{
public:

	float m_Data[9];

	//performance reasons
	Matrix3x3() {}
	Matrix3x3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22) 
	{ 
		Get(0, 0) = m00; 
		Get(1, 0) = m10; 
		Get(2, 0) = m20; 
		Get(0, 1) = m01; 
		Get(1, 1) = m11; 
		Get(2, 1) = m21; 
		Get(0, 2) = m02; 
		Get(1, 2) = m12; 
		Get(2, 2) = m22; 
	}

	explicit Matrix3x3(const class Matrix4x4& m);
	// The Get function accesses the matrix in std math convention
	// m0,0 m0,1 m0,2
	// m1,0 m1,1 m1,2
	// m2,0 m2,1 m2,2

	// The floats are laid out:
	// m0   m3   m6
	// m1   m4   m7
	// m2   m5   m8


	float& Get(int row, int column) { return m_Data[row + (column * 3)]; }
	const float& Get(int row, int column) const { return m_Data[row + (column * 3)]; }

	float& operator[](int row) { return m_Data[row]; }
	float operator[](int row) const { return m_Data[row]; }

	float* GetPtr() { return m_Data; }
	const float* GetPtr() const { return m_Data; }

	Vector3 GetColumn(int col) const { return Vector3(Get(0, col), Get(1, col), Get(2, col)); }
	Matrix3x3& operator=(const class Matrix4x4& m);

	Matrix3x3& operator*=(const Matrix3x3& inM);
	Matrix3x3& operator*=(const class Matrix4x4& inM);
	friend Matrix3x3 operator*(const Matrix3x3& lhs, const Matrix3x3& rhs);
	Vector3 MultiplyVector3(const Vector3& inV) const;
	void MultiplyVector3(const Vector3& inV, Vector3& output) const;

	Vector3 MultiplyPoint3(const Vector3& inV) const { return MultiplyVector3(inV); }
	Vector3 MultiplyVector3Transpose(const Vector3& inV) const;
	Vector3 MultiplyPoint3Transpose(const Vector3& inV) const { return MultiplyVector3Transpose(inV); }

	Matrix3x3& operator*=(float f);
	Matrix3x3& operator/=(float f) { return *this *= (1.0F / f); }

	float GetDeterminant() const;

	//  Matrix3x3f& Transpose (const Matrix3x3f& inM);
	Matrix3x3& Transpose();
	//  Matrix3x3f& Invert (const Matrix3x3f& inM)                                              { return Transpose (inM); }
	bool Invert();
	void InvertTranspose();

	Matrix3x3& SetIdentity();
	Matrix3x3& SetZero();
	Matrix3x3& SetFromToRotation(const Vector3& from, const Vector3& to);
	Matrix3x3& SetAxisAngle(const Vector3& rotationAxis, float radians);
	Matrix3x3& SetBasis(const Vector3& inX, const Vector3& inY, const Vector3& inZ);
	Matrix3x3& SetBasisTransposed(const Vector3& inX, const Vector3& inY, const Vector3& inZ);
	Matrix3x3& SetScale(const Vector3& inScale);
	Matrix3x3& Scale(const Vector3& inScale);

	bool IsIdentity(float threshold = Vector3::epsilon);

	static const Matrix3x3 zero;
	static const Matrix3x3 identity;
};

// Generates an orthornormal basis from a look at rotation, returns if it was successful
// (Righthanded)
bool LookRotationToMatrix(const Vector3& viewVec, const Vector3& upVec, Matrix3x3* m);

void EulerToMatrix(const Vector3& v, Matrix3x3& matrix);

inline Vector3 Matrix3x3::MultiplyVector3(const Vector3& v) const
{
	Vector3 res;
	res.X = m_Data[0] * v.X + m_Data[3] * v.Y + m_Data[6] * v.Z;
	res.Y = m_Data[1] * v.X + m_Data[4] * v.Y + m_Data[7] * v.Z;
	res.Z = m_Data[2] * v.X + m_Data[5] * v.Y + m_Data[8] * v.Z;
	return res;
}

inline void Matrix3x3::MultiplyVector3(const Vector3& v, Vector3& output) const
{
	output.X = m_Data[0] * v.X + m_Data[3] * v.Y + m_Data[6] * v.Z;
	output.Y = m_Data[1] * v.X + m_Data[4] * v.Y + m_Data[7] * v.Z;
	output.Z = m_Data[2] * v.X + m_Data[5] * v.Y + m_Data[8] * v.Z;
}

inline void MultiplyMatrices3x3(const Matrix3x3* __restrict lhs, const Matrix3x3* __restrict rhs, Matrix3x3* __restrict res)
{
	/* TODO : Assert */
	/* Assert(lhs != rhs && lhs != res && rhs != res); */
	for (int i = 0; i < 3; ++i)
	{
		res->m_Data[i] = lhs->m_Data[i] * rhs->m_Data[0] + lhs->m_Data[i + 3] * rhs->m_Data[1] + lhs->m_Data[i + 6] * rhs->m_Data[2];
		res->m_Data[i + 3] = lhs->m_Data[i] * rhs->m_Data[3] + lhs->m_Data[i + 3] * rhs->m_Data[4] + lhs->m_Data[i + 6] * rhs->m_Data[5];
		res->m_Data[i + 6] = lhs->m_Data[i] * rhs->m_Data[6] + lhs->m_Data[i + 3] * rhs->m_Data[7] + lhs->m_Data[i + 6] * rhs->m_Data[8];
	}
}

inline Matrix3x3 operator*(const Matrix3x3& lhs, const Matrix3x3& rhs)
{
	Matrix3x3 temp;
	MultiplyMatrices3x3(&lhs, &rhs, &temp);
	return temp;
}

inline Vector3 Matrix3x3::MultiplyVector3Transpose(const Vector3& v) const
{
	Vector3 res;
	res.X = Get(0, 0) * v.X + Get(1, 0) * v.Y + Get(2, 0) * v.Z;
	res.Y = Get(0, 1) * v.X + Get(1, 1) * v.Y + Get(2, 1) * v.Z;
	res.Z = Get(0, 2) * v.X + Get(1, 2) * v.Y + Get(2, 2) * v.Z;
	return res;
}

template<class TransferFunction>
inline void Matrix3x3::Transfer(TransferFunction& t)
{
	t.Transfer(Get(0, 0), "e00"); t.Transfer(Get(0, 1), "e01"); t.Transfer(Get(0, 2), "e02");
	t.Transfer(Get(1, 0), "e10"); t.Transfer(Get(1, 1), "e11"); t.Transfer(Get(1, 2), "e12");
	t.Transfer(Get(2, 0), "e20"); t.Transfer(Get(2, 1), "e21"); t.Transfer(Get(2, 2), "e22");
}
