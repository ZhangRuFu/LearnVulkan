#pragma once

#include "Vector3.h"

/*
	Column First
	// The Get function in std math convention
	// m00 m01 m02
	// m10 m11 m12
	// m20 m21 m22

	// The floats are laid out in mem: m00 m10 m20 m01 m11 m21 m02 m12 m22
*/

class Matrix3x3
{
public:
	float m_data[9];

public:
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

	//convert from mat4
	explicit Matrix3x3(const class Matrix4x4& m);
	Matrix3x3& operator*=(const class Matrix4x4& inM);

	float& Get(int row, int column) { return m_data[row + (column * 3)]; }
	const float& Get(int row, int column) const { return m_data[row + (column * 3)]; }

	float& operator[](int row) { return m_data[row]; }
	float operator[](int row) const { return m_data[row]; }

	float* GetPtr() { return m_data; }
	const float* GetPtr() const { return m_data; }

	//think this can be optimized
	Vector3 GetColumn(int col) const { return Vector3(Get(0, col), Get(1, col), Get(2, col)); }
	Matrix3x3& operator=(const class Matrix4x4& m);

	Matrix3x3& operator*=(const Matrix3x3& inM);
	
	friend Matrix3x3 operator*(const Matrix3x3& lhs, const Matrix3x3& rhs);
	
	Vector3 MultiplyVector3(const Vector3& inV) const;
	void MultiplyVector3(const Vector3& inV, Vector3& output) const;

	Vector3 MultiplyPoint3(const Vector3& inV) const;
	Vector3 MultiplyVector3Transpose(const Vector3& inV) const;
	Vector3 MultiplyPoint3Transpose(const Vector3& inV) const { return MultiplyVector3Transpose(inV); }

	Matrix3x3& operator*=(float f);
	Matrix3x3& operator/=(float f) { return *this *= (1.0F / f); }

	float GetDeterminant() const;

	Matrix3x3& Transpose();

	bool Invert();
	void InvertTranspose();

	Matrix3x3& SetIdentity();
	Matrix3x3& SetZero();
	
	//Rotation
	Matrix3x3& SetFromToRotation(const Vector3& from, const Vector3& to);
	Matrix3x3& SetAxisAngle(const Vector3& rotationAxis, float radians);
	
	//Basis vector to coordinate
	Matrix3x3& SetBasis(const Vector3& inX, const Vector3& inY, const Vector3& inZ);
	Matrix3x3& SetBasisTransposed(const Vector3& inX, const Vector3& inY, const Vector3& inZ);
	
	//Scale
	Matrix3x3& SetScale(const Vector3& inScale);	//scale, other to 0
	Matrix3x3& Scale(const Vector3& inScale);		//scale by vector

	bool IsIdentity(float threshold = Vector3::epsilon);

	static const Matrix3x3 zero;
	static const Matrix3x3 identity;
};

// Generates an orthornormal basis from a look at rotation, returns if it was successful
// (Righthanded)
bool LookRotationToMatrix(const Vector3& viewVec, const Vector3& upVec, Matrix3x3* m);

void EulerToMatrix(const Vector3& v, Matrix3x3& matrix);

inline void MultiplyMatrices3x3(const Matrix3x3* __restrict lhs, const Matrix3x3* __restrict rhs, Matrix3x3* __restrict res)
{
	/* TODO : Assert */
	/* Assert(lhs != rhs && lhs != res && rhs != res); */

	for (int i = 0; i < 3; ++i)
	{
		res->m_data[i] = lhs->m_data[i] * rhs->m_data[0] + lhs->m_data[i + 3] * rhs->m_data[1] + lhs->m_data[i + 6] * rhs->m_data[2];
		res->m_data[i + 3] = lhs->m_data[i] * rhs->m_data[3] + lhs->m_data[i + 3] * rhs->m_data[4] + lhs->m_data[i + 6] * rhs->m_data[5];
		res->m_data[i + 6] = lhs->m_data[i] * rhs->m_data[6] + lhs->m_data[i + 3] * rhs->m_data[7] + lhs->m_data[i + 6] * rhs->m_data[8];
	}
}

inline Matrix3x3 operator*(const Matrix3x3& lhs, const Matrix3x3& rhs)
{
	Matrix3x3 temp;
	MultiplyMatrices3x3(&lhs, &rhs, &temp);
	return temp;
}