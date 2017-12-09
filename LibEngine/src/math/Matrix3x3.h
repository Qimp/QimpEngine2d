#ifndef __MATH_MATRIX3X3_H__
#define __MATH_MATRIX3X3_H__

#pragma once

#include "Vector3.h"

QIMP_BEGIN_NAMESPACE

class Matrix3x3f
{
	public:

	float m_Data[9];
	
	///@todo: Can't be Transfer optimized because Transfer doesn't write the same as memory layout
	//DECLARE_SERIALIZE_NO_PPTR (Matrix3x3f)
	
	Matrix3x3f () {}
	Matrix3x3f (float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22) { Get (0,0) = m00; Get (1,0) = m10; Get (2,0) = m20; Get (0,1) = m01; Get (1,1) =m11; Get (2,1) = m21; Get (0,2) = m02; Get (1,2) = m12; Get (2,2) = m22; }
	explicit Matrix3x3f (const class Matrix4x4f& m);
	// The Get function accesses the matrix in std math convention
 	// m0,0 m0,1 m0,2
	// m1,0 m1,1 m1,2
	// m2,0 m2,1 m2,2

	// The floats are laid out:
	// m0   m3   m6	
	// m1   m4   m7	
	// m2   m5   m8	
	
	
	float& Get (int row, int column) 				{ return m_Data[row + (column * 3)]; }
	const float& Get (int row, int column)const 	{ return m_Data[row + (column * 3)]; }
	void Set(const Matrix4x4f& m);
	
	float& operator [] (int row) 					{ return m_Data[row]; }
	float operator [] (int row) const 				{ return m_Data[row]; }
	
	float* GetPtr ()								{ return m_Data; }
	const float* GetPtr () const					{ return m_Data; }
	
	Vector3f GetColumn (int col) const { return Vector3f (Get (0, col), Get (1, col), Get (2, col)); }
	Matrix3x3f& operator = (const class Matrix4x4f& m);
		
	Matrix3x3f& operator *= (const Matrix3x3f& inM);
	Matrix3x3f& operator *= (const class Matrix4x4f& inM);
	friend Matrix3x3f operator * (const Matrix3x3f& lhs, const Matrix3x3f& rhs)	{ Matrix3x3f temp (lhs); temp *= rhs; return temp; }
	Vector3f MultiplyVector3 (const Vector3f& inV) const;
	void MultiplyVector3 (const Vector3f& inV, Vector3f& output) const;

	Vector3f MultiplyPoint3 (const Vector3f& inV) const					{ return MultiplyVector3 (inV); }
	Vector3f MultiplyVector3Transpose (const Vector3f& inV) const;
	Vector3f MultiplyPoint3Transpose (const Vector3f& inV) const		{ return MultiplyVector3Transpose (inV); }

	Matrix3x3f& operator *= (float f);
	Matrix3x3f& operator /= (float f) { return *this *= (1.0F / f); }
	
	Vector3f GetAxisX() const;
	Vector3f GetAxisY() const;
	Vector3f GetAxisZ() const;

	float GetDeterminant () const;
	
//	Matrix3x3f& Transpose (const Matrix3x3f& inM);
	Matrix3x3f& Transpose ();
//	Matrix3x3f& Invert (const Matrix3x3f& inM)												{ return Transpose (inM); }
	bool Invert ();
	void InvertTranspose ();
	
	Matrix3x3f& SetIdentity ();
	Matrix3x3f& SetZero ();
	Matrix3x3f& SetFromToRotation (const Vector3f& from, const Vector3f& to);
	Matrix3x3f& SetAxisAngle (const Vector3f& rotationAxis, float radians);
	Matrix3x3f& SetOrthoNormalBasis (const Vector3f& inX, const Vector3f& inY, const Vector3f& inZ);
	Matrix3x3f& SetOrthoNormalBasisInverse (const Vector3f& inX, const Vector3f& inY, const Vector3f& inZ);
	Matrix3x3f& SetScale (const Vector3f& inScale);
	Matrix3x3f& Scale (const Vector3f& inScale);
	Vector3f GetScale() const;
	
	bool IsIdentity (float threshold = Vector3f::epsilon);
	
	static const Matrix3x3f zero;
	static const Matrix3x3f identity;
};

// Generates an orthornormal basis from a look at rotation, returns if it was successful
// (Righthanded)
bool LookRotationToMatrix (const Vector3f& viewVec, const Vector3f& upVec, Matrix3x3f* m);

bool MatrixToEuler (const Matrix3x3f& matrix, Vector3f& v);
void EulerToMatrix (const Vector3f& v, Matrix3x3f& matrix);

inline Vector3f Matrix3x3f::MultiplyVector3 (const Vector3f& v) const
{
	Vector3f res;
	res.x = m_Data[0] * v.x + m_Data[3] * v.y + m_Data[6] * v.z;
	res.y = m_Data[1] * v.x + m_Data[4] * v.y + m_Data[7] * v.z;
	res.z = m_Data[2] * v.x + m_Data[5] * v.y + m_Data[8] * v.z;
	return res;
}

inline void Matrix3x3f::MultiplyVector3 (const Vector3f& v, Vector3f& output) const
{
	output.x = m_Data[0] * v.x + m_Data[3] * v.y + m_Data[6] * v.z;
	output.y = m_Data[1] * v.x + m_Data[4] * v.y + m_Data[7] * v.z;
	output.z = m_Data[2] * v.x + m_Data[5] * v.y + m_Data[8] * v.z;
}


inline Vector3f Matrix3x3f::MultiplyVector3Transpose (const Vector3f& v) const
{
	Vector3f res;
	res.x = Get (0, 0) * v.x + Get (1, 0) * v.y + Get (2, 0) * v.z;
	res.y = Get (0, 1) * v.x + Get (1, 1) * v.y + Get (2, 1) * v.z;
	res.z = Get (0, 2) * v.x + Get (1, 2) * v.y + Get (2, 2) * v.z;
	return res;
}

inline Vector3f Matrix3x3f::GetAxisX() const
{
	return Vector3f(Get(0, 0), Get(1, 0), Get(2, 0));
}

inline Vector3f Matrix3x3f::GetAxisY() const
{
	return Vector3f(Get(0, 1), Get(1, 1), Get(2, 1));
}

inline Vector3f Matrix3x3f::GetAxisZ() const
{
	return Vector3f(Get(0, 2), Get(1, 2), Get(2, 2));
}

void OrthoNormalize (Matrix3x3f& matrix);

QIMP_END_NAMESPACE

#endif//__MATH_MATRIX3X3_H__