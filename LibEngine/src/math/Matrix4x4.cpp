#include "Private.h"


using namespace std;

QIMP_BEGIN_NAMESPACE
	
	namespace
	{
		Matrix4x4f CreateIdentityMatrix4x4f ()
		{
			Matrix4x4f temp; 
			temp.SetIdentity (); 
			return temp;
		}
	}

const Matrix4x4f Matrix4x4f::identity = CreateIdentityMatrix4x4f ();

Matrix4x4f::Matrix4x4f (const float data[16])
{
	for (int i=0; i<16; i++)
		m_Data[i] = data[i];
}

Matrix4x4f::Matrix4x4f (const Matrix3x3f &other)
{
	m_Data[0] = other.m_Data[0];
	m_Data[1] = other.m_Data[1];
	m_Data[2] = other.m_Data[2];
	m_Data[3] = 0.0F;

	m_Data[4] = other.m_Data[3];
	m_Data[5] = other.m_Data[4];
	m_Data[6] = other.m_Data[5];
	m_Data[7] = 0.0F;

	m_Data[8] = other.m_Data[6];
	m_Data[9] = other.m_Data[7];
	m_Data[10] = other.m_Data[8];
	m_Data[11] = 0.0F;

	m_Data[12] = 0.0F;
	m_Data[13] = 0.0F;
	m_Data[14] = 0.0F;
	m_Data[15] = 1.0F;
}

Matrix4x4f& Matrix4x4f::operator = (const Matrix3x3f& other)
{
	m_Data[0] = other.m_Data[0];
	m_Data[1] = other.m_Data[1];
	m_Data[2] = other.m_Data[2];
	m_Data[3] = 0.0F;

	m_Data[4] = other.m_Data[3];
	m_Data[5] = other.m_Data[4];
	m_Data[6] = other.m_Data[5];
	m_Data[7] = 0.0F;

	m_Data[8] = other.m_Data[6];
	m_Data[9] = other.m_Data[7];
	m_Data[10] = other.m_Data[8];
	m_Data[11] = 0.0F;

	m_Data[12] = 0.0F;
	m_Data[13] = 0.0F;
	m_Data[14] = 0.0F;
	m_Data[15] = 1.0F;
	return *this;
}

void Matrix4x4f::Set( float a00, float a01, float a02, float a03,
					float a10, float a11, float a12, float a13,
					float a20, float a21, float a22, float a23,
					float a30, float a31, float a32, float a33)
{
	SetRow(0, a00, a01, a02, a03 );
	SetRow(1, a10, a11, a12, a13 );
	SetRow(2, a20, a21, a22, a23 );
	SetRow(3, a30, a31, a32, a33 );
}

bool Matrix4x4f::IsIdentity (float threshold) const
{
	if (CompareApproximately (Get (0,0),1.0f, threshold) && CompareApproximately (Get (0,1),0.0f, threshold) && CompareApproximately (Get (0,2),0.0f, threshold) && CompareApproximately (Get (0,3),0.0f, threshold) &&
		CompareApproximately (Get (1,0),0.0f, threshold) && CompareApproximately (Get (1,1),1.0f, threshold) && CompareApproximately (Get (1,2),0.0f, threshold) && CompareApproximately (Get (1,3),0.0f, threshold) &&
		CompareApproximately (Get (2,0),0.0f, threshold) && CompareApproximately (Get (2,1),0.0f, threshold) && CompareApproximately (Get (2,2),1.0f, threshold) && CompareApproximately (Get (2,3),0.0f, threshold) &&
		CompareApproximately (Get (3,0),0.0f, threshold) && CompareApproximately (Get (3,1),0.0f, threshold) && CompareApproximately (Get (3,2),0.0f, threshold) && CompareApproximately (Get (3,3),1.0f, threshold))
		return true;
	return false;
}

double Matrix4x4f::GetDeterminant () const
{
	double m00 = Get(0, 0);  double m01 = Get(0, 1);  double m02 = Get(0, 2);  double m03 = Get(0, 3);
	double m10 = Get(1, 0);  double m11 = Get(1, 1);  double m12 = Get(1, 2);  double m13 = Get(1, 3);
	double m20 = Get(2, 0);  double m21 = Get(2, 1);  double m22 = Get(2, 2);  double m23 = Get(2, 3);
	double m30 = Get(3, 0);  double m31 = Get(3, 1);  double m32 = Get(3, 2);  double m33 = Get(3, 3);

	double result =
		m03 * m12 * m21 * m30 - m02 * m13 * m21 * m30 - m03 * m11 * m22 * m30 + m01 * m13 * m22 * m30 +
		m02 * m11 * m23 * m30 - m01 * m12 * m23 * m30 - m03 * m12 * m20 * m31 + m02 * m13 * m20 * m31 +
		m03 * m10 * m22 * m31 - m00 * m13 * m22 * m31 - m02 * m10 * m23 * m31 + m00 * m12 * m23 * m31 +
		m03 * m11 * m20 * m32 - m01 * m13 * m20 * m32 - m03 * m10 * m21 * m32 + m00 * m13 * m21 * m32 +
		m01 * m10 * m23 * m32 - m00 * m11 * m23 * m32 - m02 * m11 * m20 * m33 + m01 * m12 * m20 * m33 +
		m02 * m10 * m21 * m33 - m00 * m12 * m21 * m33 - m01 * m10 * m22 * m33 + m00 * m11 * m22 * m33;
	return result;
}

Matrix4x4f& Matrix4x4f::operator *= (const Matrix4x4f& inM1)
{
	Assert(&inM1 != this);
	Matrix4x4f tmp;
	MultiplyMatrix4x4(this, &inM1, &tmp);
	*this = tmp;
	return *this;
}

Matrix4x4f& Matrix4x4f::operator += (const Matrix4x4f& inM)
{
	for (int i = 0; i < 16; i++)
		m_Data[i] += inM.m_Data[i];
	return *this;
}

Quaternionf Matrix4x4f::GetRotation() const
{
	Quaternionf q;
	MatrixToQuaternion(*this, q);
	return q;
}

Vector3f Matrix4x4f::GetScale() const
{
	Vector3f Scale(1.0f, 1.0f, 1.0f);
	const float Sum0 = m_Data[0] * m_Data[0] + m_Data[1] * m_Data[1] + m_Data[2] * m_Data[2];
	const float Sum1 = m_Data[4] * m_Data[4] + m_Data[5] * m_Data[5] + m_Data[6] * m_Data[6];
	const float Sum2 = m_Data[8] * m_Data[8] + m_Data[9] * m_Data[9] + m_Data[10] * m_Data[10];

	if (fabs(Sum0) > EPSILON)
	{
		Scale.x = sqrt(Sum0);
	}
	else
	{
		Scale.x = 0.0f;
	}
	
	if (fabs(Sum1) > EPSILON)
	{
		Scale.y = sqrt(Sum1);
	}
	else
	{
		Scale.y = 0.0f;
	}

	if (fabs(Sum2) > EPSILON)
	{
		Scale.z = sqrt(Sum2);
	}
	else
	{
		Scale.z = 0.0f;
	}
	return Scale;
}

void MultiplyMatrices3x4( const Matrix4x4f& lhs, const Matrix4x4f& rhs, Matrix4x4f& res)
{
	for (int i=0;i<3;i++)
	{
		res.m_Data[i]    = lhs.m_Data[i] * rhs.m_Data[0]  + lhs.m_Data[i+4] * rhs.m_Data[1]  + lhs.m_Data[i+8] * rhs.m_Data[2];//  + lhs.m_Data[i+12] * rhs.m_Data[3];
		res.m_Data[i+4]  = lhs.m_Data[i] * rhs.m_Data[4]  + lhs.m_Data[i+4] * rhs.m_Data[5]  + lhs.m_Data[i+8] * rhs.m_Data[6];//  + lhs.m_Data[i+12] * rhs.m_Data[7];
		res.m_Data[i+8]  = lhs.m_Data[i] * rhs.m_Data[8]  + lhs.m_Data[i+4] * rhs.m_Data[9]  + lhs.m_Data[i+8] * rhs.m_Data[10];// + lhs.m_Data[i+12] * rhs.m_Data[11];
		res.m_Data[i+12] = lhs.m_Data[i] * rhs.m_Data[12] + lhs.m_Data[i+4] * rhs.m_Data[13] + lhs.m_Data[i+8] * rhs.m_Data[14] + lhs.m_Data[i+12];// * rhs.m_Data[15];
	}
	
	res.m_Data[3]  = 0.0f;
	res.m_Data[7]  = 0.0f;
	res.m_Data[11] = 0.0f;
	res.m_Data[15] = 1.0f;
}


Matrix4x4f& Matrix4x4f::SetIdentity ()
{
	Get (0, 0) = 1.0;	Get (0, 1) = 0.0;	Get (0, 2) = 0.0;	Get (0, 3) = 0.0;
	Get (1, 0) = 0.0;	Get (1, 1) = 1.0;	Get (1, 2) = 0.0;	Get (1, 3) = 0.0;
	Get (2, 0) = 0.0;	Get (2, 1) = 0.0;	Get (2, 2) = 1.0;	Get (2, 3) = 0.0;
	Get (3, 0) = 0.0;	Get (3, 1) = 0.0;	Get (3, 2) = 0.0;	Get (3, 3) = 1.0;
	return *this;
}

Matrix4x4f& Matrix4x4f::SetOrthoNormalBasis (const Vector3f& inX, const Vector3f& inY, const Vector3f& inZ)
{
	Get (0, 0) = inX[0];	Get (0, 1) = inY[0];	Get (0, 2) = inZ[0];	Get (0, 3) = 0.0;
	Get (1, 0) = inX[1];	Get (1, 1) = inY[1];	Get (1, 2) = inZ[1];	Get (1, 3) = 0.0;
	Get (2, 0) = inX[2];	Get (2, 1) = inY[2];	Get (2, 2) = inZ[2];	Get (2, 3) = 0.0;
	Get (3, 0) = 0.0;		Get (3, 1) = 0.0;		Get (3, 2) = 0.0;		Get (3, 3) = 1.0;
	return *this;
}

Matrix4x4f& Matrix4x4f::SetOrthoNormalBasisInverse (const Vector3f& inX, const Vector3f& inY, const Vector3f& inZ)
{
	Get (0, 0) = inX[0];	Get (1, 0) = inY[0];	Get (2, 0) = inZ[0];	Get (3, 0) = 0.0;
	Get (0, 1) = inX[1];	Get (1, 1) = inY[1];	Get (2, 1) = inZ[1];	Get (3, 1) = 0.0;
	Get (0, 2) = inX[2];	Get (1, 2) = inY[2];	Get (2, 2) = inZ[2];	Get (3, 2) = 0.0;
	Get (0, 3) = 0.0;		Get (1, 3) = 0.0;		Get (2, 3) = 0.0;		Get (3, 3) = 1.0;
	return *this;
}

Matrix4x4f& Matrix4x4f::SetPositionAndOrthoNormalBasis (const Vector3f& inPosition, const Vector3f& inX, const Vector3f& inY, const Vector3f& inZ)
{
	Get (0, 0) = inX[0];	Get (0, 1) = inY[0];	Get (0, 2) = inZ[0];	Get (0, 3) = inPosition[0];
	Get (1, 0) = inX[1];	Get (1, 1) = inY[1];	Get (1, 2) = inZ[1];	Get (1, 3) = inPosition[1];
	Get (2, 0) = inX[2];	Get (2, 1) = inY[2];	Get (2, 2) = inZ[2];	Get (2, 3) = inPosition[2];
	Get (3, 0) = 0.0;		Get (3, 1) = 0.0;		Get (3, 2) = 0.0;		Get (3, 3) = 1.0;
	return *this;
}

Matrix4x4f& Matrix4x4f::SetScale (const Vector3f& inScale)
{
	Get (0, 0) = inScale[0];	Get (0, 1) = 0.0;			Get (0, 2) = 0.0;			Get (0, 3) = 0.0;
	Get (1, 0) = 0.0;			Get (1, 1) = inScale[1];	Get (1, 2) = 0.0;			Get (1, 3) = 0.0;
	Get (2, 0) = 0.0;			Get (2, 1) = 0.0;			Get (2, 2) = inScale[2];	Get (2, 3) = 0.0;
	Get (3, 0) = 0.0;			Get (3, 1) = 0.0;			Get (3, 2) = 0.0;			Get (3, 3) = 1.0;
	return *this;
}

Matrix4x4f& Matrix4x4f::Scale (const Vector3f& inScale)
{
	Get (0, 0) *= inScale[0];
	Get (1, 0) *= inScale[0];
	Get (2, 0) *= inScale[0];
	Get (3, 0) *= inScale[0];

	Get (0, 1) *= inScale[1];
	Get (1, 1) *= inScale[1];
	Get (2, 1) *= inScale[1];
	Get (3, 1) *= inScale[1];

	Get (0, 2) *= inScale[2];
	Get (1, 2) *= inScale[2];
	Get (2, 2) *= inScale[2];
	Get (3, 2) *= inScale[2];
	return *this;
}

Matrix4x4f& Matrix4x4f::Scale(const Vector4f& inScale)
{
	Get(0, 0) *= inScale[0];
	Get(1, 0) *= inScale[0];
	Get(2, 0) *= inScale[0];
	Get(3, 0) *= inScale[0];

	Get(0, 1) *= inScale[1];
	Get(1, 1) *= inScale[1];
	Get(2, 1) *= inScale[1];
	Get(3, 1) *= inScale[1];

	Get(0, 2) *= inScale[2];
	Get(1, 2) *= inScale[2];
	Get(2, 2) *= inScale[2];
	Get(3, 2) *= inScale[2];

	Get(0, 3) *= inScale[3];
	Get(1, 3) *= inScale[3];
	Get(2, 3) *= inScale[3];
	Get(3, 3) *= inScale[3];
	return *this;
}

Matrix4x4f& Matrix4x4f::Translate (const Vector3f& inTrans)
{
	Get (0, 3) = Get (0, 0) * inTrans[0] + Get (0, 1) * inTrans[1] + Get (0, 2) * inTrans[2] + Get (0, 3);
	Get (1, 3) = Get (1, 0) * inTrans[0] + Get (1, 1) * inTrans[1] + Get (1, 2) * inTrans[2] + Get (1, 3);
	Get (2, 3) = Get (2, 0) * inTrans[0] + Get (2, 1) * inTrans[1] + Get (2, 2) * inTrans[2] + Get (2, 3);
	Get (3, 3) = Get (3, 0) * inTrans[0] + Get (3, 1) * inTrans[1] + Get (3, 2) * inTrans[2] + Get (3, 3);
	return *this;
}

Matrix4x4f& Matrix4x4f::SetTranslate (const Vector3f& inTrans)
{
	Get (0, 0) = 1.0;	Get (0, 1) = 0.0;	Get (0, 2) = 0.0;	Get (0, 3) = inTrans[0];
	Get (1, 0) = 0.0;	Get (1, 1) = 1.0;	Get (1, 2) = 0.0;	Get (1, 3) = inTrans[1];
	Get (2, 0) = 0.0;	Get (2, 1) = 0.0;	Get (2, 2) = 1.0;	Get (2, 3) = inTrans[2];
	Get (3, 0) = 0.0;	Get (3, 1) = 0.0;	Get (3, 2) = 0.0;	Get (3, 3) = 1.0;
	return *this;
}

Matrix4x4f& Matrix4x4f::SetPerspective(
	float fovy,
	float aspect,
	float zNear,
	float zFar )
{
	float cotangent, deltaZ;
	float radians = Deg2Rad (fovy / 2.0f);
	cotangent = cos (radians) / sin (radians);
	deltaZ = zNear - zFar;
	
	Get (0,0) = cotangent / aspect;	Get (0,1) = 0.0F;		Get (0,2) = 0.0F;						Get (0,3) = 0.0F;
	Get (1,0) = 0.0F;				Get (1,1) = cotangent;	Get (1,2) = 0.0F;						Get (1,3) = 0.0F;
	Get (2,0) = 0.0F;				Get (2,1) = 0.0F;		Get (2,2) = -(zFar + zNear) / deltaZ;	Get (2,3) = 2.0F * zNear * zFar / deltaZ;
	Get (3,0) = 0.0F;				Get (3,1) = 0.0F;		Get (3,2) = 1.0F;						Get (3,3) = 0.0F;

	return *this;
}

Matrix4x4f& Matrix4x4f::SetPerspectiveCotan(
	float cotangent,
	float zNear,
	float zFar )
{
	float deltaZ = zNear - zFar;
	
	Get (0,0) = cotangent;			Get (0,1) = 0.0F;		Get (0,2) = 0.0F;						Get (0,3) = 0.0F;
	Get (1,0) = 0.0F;				Get (1,1) = cotangent;	Get (1,2) = 0.0F;						Get (1,3) = 0.0F;
	Get (2,0) = 0.0F;				Get (2,1) = 0.0F;		Get (2,2) = -(zFar + zNear) / deltaZ;	Get (2,3) = 2.0F * zNear * zFar / deltaZ;
	Get (3,0) = 0.0F;				Get (3,1) = 0.0F;		Get (3,2) = 1.0F;						Get (3,3) = 0.0F;

	return *this;
}

Matrix4x4f& Matrix4x4f::SetOrtho (
	float left,
	float right,
	float bottom,
	float top,
	float zNear,
	float zFar )
{
	SetIdentity ();

	float deltax = right - left;
	float deltay = top - bottom;
	float deltaz = zFar - zNear;

	Get(0,0) = 2.0F / deltax;
	Get(0,3) = -(right + left) / deltax;
	Get(1,1) = 2.0F / deltay;
	Get(1,3) = -(top + bottom) / deltay;
	Get(2,2) = -2.0F / deltaz;
	Get(2,3) = -(zFar + zNear) / deltaz;

	// ×óÓÒÊÖÇÐ»»
	Get(2, 2) *= -1;
	Get(3, 2) *= -1;
	return *this;
}

Matrix4x4f& Matrix4x4f::SetFrustum (
	float left,
	float right,
	float bottom,
	float top,
	float nearval,
	float farval )
{
	float x, y, a, b, c, d, e;
	    
	x =  (2.0F * nearval) 		/ (right - left);
	y =  (2.0F * nearval) 		/ (top - bottom);
	a =  (right + left)			/ (right - left);
	b =  (top + bottom)			/ (top - bottom);
	c = -(farval + nearval)		   / (farval - nearval);
	d = -(2.0f * farval * nearval) / (farval - nearval);
	e = -1.0f;

	Get (0,0) = x;    Get (0,1) = 0.0;  Get (0,2) = a;   Get (0,3) = 0.0;
	Get (1,0) = 0.0;  Get (1,1) = y;    Get (1,2) = b;   Get (1,3) = 0.0;
	Get (2,0) = 0.0;  Get (2,1) = 0.0;  Get (2,2) = c;   Get (2,3) = d;
	Get (3,0) = 0.0;  Get (3,1) = 0.0;  Get (3,2) = e;	Get (3,3) = 0.0;
	return *this;
}



//TransformType ComputeTransformType (const Matrix4x4f& matrix, float& outUniformScale, float epsilon)
//{
//	float lengthX = Magnitude(matrix.GetAxisX());
//	float lengthY = Magnitude(matrix.GetAxisY());
//	float lengthZ = Magnitude(matrix.GetAxisZ());
//	float minAxis = std::min(std::min(lengthX, lengthY), lengthZ);
//	float maxAxis = std::max(std::max(lengthX, lengthY), lengthZ);
//	TransformType transType = kNoScaleTransform;
//	outUniformScale = 1.0f;
//	if (minAxis < 1.0 - epsilon || maxAxis > 1.0 + epsilon)
//	{
//		if (minAxis != 0.0f && maxAxis / minAxis < 1.0 + epsilon)
//		{
//			transType = kUniformScaleTransform;
//			outUniformScale = minAxis;
//		}
//		else
//			transType = kNonUniformScaleTransform;
//	}
//	return transType;
//}


#define MAT(m,r,c) (m)[(c)*4+(r)]

#define RETURN_ZERO \
{ \
	for (int i=0;i<16;i++) \
		out[i] = 0.0F; \
	return false; \
}

// 4x4 matrix inversion by Gaussian reduction with partial pivoting followed by back/substitution;
// with loops manually unrolled.

#define SWAP_ROWS(a, b) { float *_tmp = a; (a)=(b); (b)=_tmp; }
bool InvertMatrix4x4_Full(const float* m, float* out)
{
	float wtmp[4][8];
	float m0, m1, m2, m3, s;
	float *r0, *r1, *r2, *r3;
	
	r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];
	
	r0[0] = MAT(m,0,0), r0[1] = MAT(m,0,1),
	r0[2] = MAT(m,0,2), r0[3] = MAT(m,0,3),
	r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,
	
	r1[0] = MAT(m,1,0), r1[1] = MAT(m,1,1),
	r1[2] = MAT(m,1,2), r1[3] = MAT(m,1,3),
	r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,
	
	r2[0] = MAT(m,2,0), r2[1] = MAT(m,2,1),
	r2[2] = MAT(m,2,2), r2[3] = MAT(m,2,3),
	r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,
	
	r3[0] = MAT(m,3,0), r3[1] = MAT(m,3,1),
	r3[2] = MAT(m,3,2), r3[3] = MAT(m,3,3),
	r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;
	
	/* choose pivot - or die */
	if (Abs(r3[0])>Abs(r2[0])) SWAP_ROWS(r3, r2);
	if (Abs(r2[0])>Abs(r1[0])) SWAP_ROWS(r2, r1);
	if (Abs(r1[0])>Abs(r0[0])) SWAP_ROWS(r1, r0);
	if (0.0F == r0[0]) RETURN_ZERO
	
	/* eliminate first variable     */
	m1 = r1[0]/r0[0]; m2 = r2[0]/r0[0]; m3 = r3[0]/r0[0];
	s = r0[1]; r1[1] -= m1 * s; r2[1] -= m2 * s; r3[1] -= m3 * s;
	s = r0[2]; r1[2] -= m1 * s; r2[2] -= m2 * s; r3[2] -= m3 * s;
	s = r0[3]; r1[3] -= m1 * s; r2[3] -= m2 * s; r3[3] -= m3 * s;
	s = r0[4];
	if (s != 0.0F) { r1[4] -= m1 * s; r2[4] -= m2 * s; r3[4] -= m3 * s; }
	s = r0[5];
	if (s != 0.0F) { r1[5] -= m1 * s; r2[5] -= m2 * s; r3[5] -= m3 * s; }
	s = r0[6];
	if (s != 0.0F) { r1[6] -= m1 * s; r2[6] -= m2 * s; r3[6] -= m3 * s; }
	s = r0[7];
	if (s != 0.0F) { r1[7] -= m1 * s; r2[7] -= m2 * s; r3[7] -= m3 * s; }
	
	/* choose pivot - or die */
	if (Abs(r3[1])>Abs(r2[1])) SWAP_ROWS(r3, r2);
	if (Abs(r2[1])>Abs(r1[1])) SWAP_ROWS(r2, r1);
	if (0.0F == r1[1]) RETURN_ZERO;
	
	/* eliminate second variable */
	m2 = r2[1]/r1[1]; m3 = r3[1]/r1[1];
	r2[2] -= m2 * r1[2]; r3[2] -= m3 * r1[2];
	r2[3] -= m2 * r1[3]; r3[3] -= m3 * r1[3];
	s = r1[4]; if (0.0F != s) { r2[4] -= m2 * s; r3[4] -= m3 * s; }
	s = r1[5]; if (0.0F != s) { r2[5] -= m2 * s; r3[5] -= m3 * s; }
	s = r1[6]; if (0.0F != s) { r2[6] -= m2 * s; r3[6] -= m3 * s; }
	s = r1[7]; if (0.0F != s) { r2[7] -= m2 * s; r3[7] -= m3 * s; }
	
	/* choose pivot - or die */
	if (Abs(r3[2])>Abs(r2[2])) SWAP_ROWS(r3, r2);
	if (0.0F == r2[2]) RETURN_ZERO;
	
	/* eliminate third variable */
	m3 = r3[2]/r2[2];
	r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
	r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6],
	r3[7] -= m3 * r2[7];
	
	/* last check */
	if (0.0F == r3[3]) RETURN_ZERO;
	
	s = 1.0F/r3[3];				/* now back substitute row 3 */
	r3[4] *= s; r3[5] *= s; r3[6] *= s; r3[7] *= s;
	
	m2 = r2[3];					/* now back substitute row 2 */
	s  = 1.0F/r2[2];
	r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
	r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
	m1 = r1[3];
	r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
	r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
	m0 = r0[3];
	r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
	r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;
	
	m1 = r1[2];					/* now back substitute row 1 */
	s  = 1.0F/r1[1];
	r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
	r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
	m0 = r0[2];
	r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
	r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;
	
	m0 = r0[1];					/* now back substitute row 0 */
	s  = 1.0F/r0[0];
	r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
	r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);
	
	MAT(out,0,0) = r0[4]; MAT(out,0,1) = r0[5], MAT(out,0,2) = r0[6]; MAT(out,0,3) = r0[7],
	MAT(out,1,0) = r1[4]; MAT(out,1,1) = r1[5], MAT(out,1,2) = r1[6]; MAT(out,1,3) = r1[7],
	MAT(out,2,0) = r2[4]; MAT(out,2,1) = r2[5], MAT(out,2,2) = r2[6]; MAT(out,2,3) = r2[7],
	MAT(out,3,0) = r3[4]; MAT(out,3,1) = r3[5], MAT(out,3,2) = r3[6]; MAT(out,3,3) = r3[7];
	
	return true;
}

#undef SWAP_ROWS

// Invert 3D transformation matrix (not perspective). Adapted from graphics gems 2.
// Inverts upper left by calculating its determinant and multiplying it to the symmetric
// adjust matrix of each element. Finally deals with the translation by transforming the
// original translation using by the calculated inverse.
bool InvertMatrix4x4_General3D( const float* in, float* out )
{
	float pos, neg, t;
	float det;

	// Calculate the determinant of upper left 3x3 sub-matrix and
	// determine if the matrix is singular.
	pos = neg = 0.0;
	t =  MAT(in,0,0) * MAT(in,1,1) * MAT(in,2,2);
	if (t >= 0.0) pos += t; else neg += t;

	t =  MAT(in,1,0) * MAT(in,2,1) * MAT(in,0,2);
	if (t >= 0.0) pos += t; else neg += t;

	t =  MAT(in,2,0) * MAT(in,0,1) * MAT(in,1,2);
	if (t >= 0.0) pos += t; else neg += t;

	t = -MAT(in,2,0) * MAT(in,1,1) * MAT(in,0,2);
	if (t >= 0.0) pos += t; else neg += t;

	t = -MAT(in,1,0) * MAT(in,0,1) * MAT(in,2,2);
	if (t >= 0.0) pos += t; else neg += t;

	t = -MAT(in,0,0) * MAT(in,2,1) * MAT(in,1,2);
	if (t >= 0.0) pos += t; else neg += t;

	det = pos + neg;

	if (det*det < 1e-25)
		RETURN_ZERO;

	det = 1.0F / det;
	MAT(out,0,0) = (  (MAT(in,1,1)*MAT(in,2,2) - MAT(in,2,1)*MAT(in,1,2) )*det);
	MAT(out,0,1) = (- (MAT(in,0,1)*MAT(in,2,2) - MAT(in,2,1)*MAT(in,0,2) )*det);
	MAT(out,0,2) = (  (MAT(in,0,1)*MAT(in,1,2) - MAT(in,1,1)*MAT(in,0,2) )*det);
	MAT(out,1,0) = (- (MAT(in,1,0)*MAT(in,2,2) - MAT(in,2,0)*MAT(in,1,2) )*det);
	MAT(out,1,1) = (  (MAT(in,0,0)*MAT(in,2,2) - MAT(in,2,0)*MAT(in,0,2) )*det);
	MAT(out,1,2) = (- (MAT(in,0,0)*MAT(in,1,2) - MAT(in,1,0)*MAT(in,0,2) )*det);
	MAT(out,2,0) = (  (MAT(in,1,0)*MAT(in,2,1) - MAT(in,2,0)*MAT(in,1,1) )*det);
	MAT(out,2,1) = (- (MAT(in,0,0)*MAT(in,2,1) - MAT(in,2,0)*MAT(in,0,1) )*det);
	MAT(out,2,2) = (  (MAT(in,0,0)*MAT(in,1,1) - MAT(in,1,0)*MAT(in,0,1) )*det);

	// Do the translation part
	MAT(out,0,3) = - (MAT(in,0,3) * MAT(out,0,0) +
		MAT(in,1,3) * MAT(out,0,1) +
		MAT(in,2,3) * MAT(out,0,2) );
	MAT(out,1,3) = - (MAT(in,0,3) * MAT(out,1,0) +
		MAT(in,1,3) * MAT(out,1,1) +
		MAT(in,2,3) * MAT(out,1,2) );
	MAT(out,2,3) = - (MAT(in,0,3) * MAT(out,2,0) +
		MAT(in,1,3) * MAT(out,2,1) +
		MAT(in,2,3) * MAT(out,2,2) );
	
	MAT(out,3,0) = 0.0f;
	MAT(out,3,1) = 0.0f;
	MAT(out,3,2) = 0.0f;
	MAT(out,3,3) = 1.0f;

	return true;
}

#undef MAT
#undef RETURN_ZERO


/*
4x4 matrix inverse based on Cramer's rule. From Intel's "Streaming SIMD Extensions - Inverse of 4x4 Matrix" paper.
Seems to be about the same speed as our current one, maybe slightly faster. Less numerically robust though,
at very small numbers.

bool InvertMatrix4x4_Cramer (const float* mat, float* dst)
{
	float    tmp[12]; // temp array for pairs
	float    src[16]; // array of transpose source matrix
	float    det;     // determinant
	// transpose matrix
	for (int i = 0; i < 4; i++) {
		src[i]        = mat[i*4];
		src[i + 4]    = mat[i*4 + 1];
		src[i + 8]    = mat[i*4 + 2];
		src[i + 12]   = mat[i*4 + 3];
	}
	// calculate pairs for first 8 elements (cofactors)
	tmp[0]  = src[10] * src[15];
	tmp[1]  = src[11] * src[14];
	tmp[2]  = src[9]  * src[15];
	tmp[3]  = src[11] * src[13];
	tmp[4]  = src[9]  * src[14];
	tmp[5]  = src[10] * src[13];
	tmp[6]  = src[8]  * src[15];
	tmp[7]  = src[11] * src[12];
	tmp[8]  = src[8]  * src[14];
	tmp[9]  = src[10] * src[12];
	tmp[10] = src[8]  * src[13];
	tmp[11] = src[9]  * src[12];
	// calculate first 8 elements (cofactors)
	dst[0]  = tmp[0]*src[5] + tmp[3]*src[6] + tmp[4]*src[7];
	dst[0] -= tmp[1]*src[5] + tmp[2]*src[6] + tmp[5]*src[7];
	dst[1]  = tmp[1]*src[4] + tmp[6]*src[6] + tmp[9]*src[7];
	dst[1] -= tmp[0]*src[4] + tmp[7]*src[6] + tmp[8]*src[7];
	dst[2]  = tmp[2]*src[4] + tmp[7]*src[5] + tmp[10]*src[7];
	dst[2] -= tmp[3]*src[4] + tmp[6]*src[5] + tmp[11]*src[7];
	dst[3]  = tmp[5]*src[4] + tmp[8]*src[5] + tmp[11]*src[6];
	dst[3] -= tmp[4]*src[4] + tmp[9]*src[5] + tmp[10]*src[6];
	dst[4]  = tmp[1]*src[1] + tmp[2]*src[2] + tmp[5]*src[3];
	dst[4] -= tmp[0]*src[1] + tmp[3]*src[2] + tmp[4]*src[3];
	dst[5]  = tmp[0]*src[0] + tmp[7]*src[2] + tmp[8]*src[3];
	dst[5] -= tmp[1]*src[0] + tmp[6]*src[2] + tmp[9]*src[3];
	dst[6]  = tmp[3]*src[0] + tmp[6]*src[1] + tmp[11]*src[3];
	dst[6] -= tmp[2]*src[0] + tmp[7]*src[1] + tmp[10]*src[3];
	dst[7]  = tmp[4]*src[0] + tmp[9]*src[1] + tmp[10]*src[2];
	dst[7] -= tmp[5]*src[0] + tmp[8]*src[1] + tmp[11]*src[2];
	// calculate pairs for second 8 elements (cofactors)
	tmp[0]  = src[2]*src[7];
	tmp[1]  = src[3]*src[6];
	tmp[2]  = src[1]*src[7];
	tmp[3]  = src[3]*src[5];
	tmp[4]  = src[1]*src[6];
	tmp[5]  = src[2]*src[5];
	tmp[6]  = src[0]*src[7];
	tmp[7]  = src[3]*src[4];
	tmp[8]  = src[0]*src[6];
	tmp[9]  = src[2]*src[4];
	tmp[10] = src[0]*src[5];
	tmp[11] = src[1]*src[4];
	// calculate second 8 elements (cofactors)
	dst[8]  = tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15];
	dst[8] -= tmp[1]*src[13] + tmp[2]*src[14] + tmp[5]*src[15];
	dst[9]  = tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15];
	dst[9] -= tmp[0]*src[12] + tmp[7]*src[14] + tmp[8]*src[15];
	dst[10] = tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15];
	dst[10]-= tmp[3]*src[12] + tmp[6]*src[13] + tmp[11]*src[15];
	dst[11] = tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14];
	dst[11]-= tmp[4]*src[12] + tmp[9]*src[13] + tmp[10]*src[14];
	dst[12] = tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9];
	dst[12]-= tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10];
	dst[13] = tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10];
	dst[13]-= tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8];
	dst[14] = tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8];
	dst[14]-= tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9];
	dst[15] = tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9];
	dst[15]-= tmp[8]*src[9] + tmp[11]*src[10] + tmp[5]*src[8];
	// calculate determinant
	det=src[0]*dst[0]+src[1]*dst[1]+src[2]*dst[2]+src[3]*dst[3];
	// calculate matrix inverse
	if( CompareApproximately(det,0.0f) )
	{
		for (int i=0;i<16;i++)
			dst[i] = 0.0F;
		return false;
	}

	det = 1.0f/det;
	for (int j = 0; j < 16; j++)
		dst[j] *= det;

	return true;
}
*/


/*
// SSE based matrix inverse from Intel's "Streaming SIMD Extensions - Inverse of 4x4 Matrix" paper.
// Does not seem to be much faster on Core 2 Duo. Keeping it here just in case.

#include <emmintrin.h>

bool InvertMatrix4x4( const float* src, float* dst )
{
	__m128 minor0, minor1, minor2, minor3;
	__m128 row0,   row1,   row2,   row3;
	__m128 det,    tmp1;
	tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(src)), (__m64*)(src+ 4));
	row1 = _mm_loadh_pi(_mm_loadl_pi(row1, (__m64*)(src+8)), (__m64*)(src+12));
	row0 = _mm_shuffle_ps(tmp1, row1, 0x88);
	row1 = _mm_shuffle_ps(row1, tmp1, 0xDD);
	tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(src+ 2)), (__m64*)(src+ 6));
	row3 = _mm_loadh_pi(_mm_loadl_pi(row3, (__m64*)(src+10)), (__m64*)(src+14));
	row2 = _mm_shuffle_ps(tmp1, row3, 0x88);
	row3 = _mm_shuffle_ps(row3, tmp1, 0xDD);
	// -----------------------------------------------
	tmp1 = _mm_mul_ps(row2, row3);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
	minor0 = _mm_mul_ps(row1, tmp1);
	minor1 = _mm_mul_ps(row0, tmp1);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
	minor0 = _mm_sub_ps(_mm_mul_ps(row1, tmp1), minor0);
	minor1 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor1);
	minor1 = _mm_shuffle_ps(minor1, minor1, 0x4E);
	// -----------------------------------------------
	tmp1 = _mm_mul_ps(row1, row2);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
	minor0 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor0);
	minor3 = _mm_mul_ps(row0, tmp1);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
	minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp1));
	minor3 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor3);
	minor3 = _mm_shuffle_ps(minor3, minor3, 0x4E);
	// -----------------------------------------------
	tmp1 = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
	row2 = _mm_shuffle_ps(row2, row2, 0x4E);
	minor0 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor0);
	minor2 = _mm_mul_ps(row0, tmp1);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
	minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp1));
	minor2 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor2);
	minor2 = _mm_shuffle_ps(minor2, minor2, 0x4E);
	// -----------------------------------------------
	tmp1 = _mm_mul_ps(row0, row1);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
	minor2 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor2);
	minor3 = _mm_sub_ps(_mm_mul_ps(row2, tmp1), minor3);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
	minor2 = _mm_sub_ps(_mm_mul_ps(row3, tmp1), minor2);
	minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row2, tmp1));
	// -----------------------------------------------
	tmp1 = _mm_mul_ps(row0, row3);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
	minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row2, tmp1));
	minor2 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor2);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
	minor1 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor1);
	minor2 = _mm_sub_ps(minor2, _mm_mul_ps(row1, tmp1));
	// -----------------------------------------------
	tmp1 = _mm_mul_ps(row0, row2);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
	minor1 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor1);
	minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row1, tmp1));
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
	minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row3, tmp1));
	minor3 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor3);
	// -----------------------------------------------
	det = _mm_mul_ps(row0, minor0);
	det = _mm_add_ps(_mm_shuffle_ps(det, det, 0x4E), det);
	det = _mm_add_ss(_mm_shuffle_ps(det, det, 0xB1), det);
	// TODO: detect zero determinant
	tmp1 = _mm_rcp_ss(det);
	det = _mm_sub_ss(_mm_add_ss(tmp1, tmp1), _mm_mul_ss(det, _mm_mul_ss(tmp1, tmp1)));
	det = _mm_shuffle_ps(det, det, 0x00);
	minor0 = _mm_mul_ps(det, minor0);
	_mm_storel_pi((__m64*)(dst), minor0);
	_mm_storeh_pi((__m64*)(dst+2), minor0);
	minor1 = _mm_mul_ps(det, minor1);
	_mm_storel_pi((__m64*)(dst+4), minor1);
	_mm_storeh_pi((__m64*)(dst+6), minor1);
	minor2 = _mm_mul_ps(det, minor2);
	_mm_storel_pi((__m64*)(dst+ 8), minor2);
	_mm_storeh_pi((__m64*)(dst+10), minor2);
	minor3 = _mm_mul_ps(det, minor3);
	_mm_storel_pi((__m64*)(dst+12), minor3);
	_mm_storeh_pi((__m64*)(dst+14), minor3);

	return true;
}
*/


Matrix4x4f& Matrix4x4f::Transpose ()
{
	swap(Get (0,1),Get (1,0));
	swap(Get (0,2),Get (2,0));
	swap(Get (0,3),Get (3,0));
	swap(Get (1,2),Get (2,1));
	swap(Get (1,3),Get (3,1));
	swap(Get (2,3),Get (3,2));
	return *this;
}

Matrix4x4f& Matrix4x4f::Copy (const Matrix4x4f& inM) 
{
	CopyMatrix(inM.m_Data, m_Data);
	return *this;
}

void fromToRotation(const float from[3], const float to[3],float mtx[3][3]);

Matrix4x4f& Matrix4x4f::SetFromToRotation (const Vector3f& from, const Vector3f& to)
{
	float mtx[3][3];
	fromToRotation (from.GetPtr (), to.GetPtr (), mtx);
	Get (0, 0) = mtx[0][0];	Get (0, 1) = mtx[0][1];	Get (0, 2) = mtx[0][2];	Get (0, 3) = 0.0;
	Get (1, 0) = mtx[1][0];	Get (1, 1) = mtx[1][1];	Get (1, 2) = mtx[1][2];	Get (1, 3) = 0.0;
	Get (2, 0) = mtx[2][0];	Get (2, 1) = mtx[2][1];	Get (2, 2) = mtx[2][2];	Get (2, 3) = 0.0;
	Get (3, 0) = 0.0;		Get (3, 1) = 0.0;		Get (3, 2) = 0.0;		Get (3, 3) = 1.0;
	return *this;
}

bool CompareApproximately (const Matrix4x4f& lhs, const Matrix4x4f& rhs, float dist)
{
	for (int i=0;i<16;i++)
	{
		if (!CompareApproximately (lhs[i], rhs[i], dist))
			return false;
	}
	return true;
}

void Matrix4x4f::SetTR (const Vector4f& pos, const Quaternionf& q)
{
	QuaternionToMatrix (q, *this);
	m_Data[12] = pos[0];
	m_Data[13] = pos[1];
	m_Data[14] = pos[2];
}

void Matrix4x4f::SetTR (const Vector3f& pos, const Quaternionf& q)
{
	QuaternionToMatrix (q, *this);
	m_Data[12] = pos[0];
	m_Data[13] = pos[1];
	m_Data[14] = pos[2];
}

void Matrix4x4f::SetTRS (const Vector3f& pos, const Quaternionf& q, const Vector3f& s)
{
	QuaternionToMatrix (q, *this);

	m_Data[0] *= s[0];
	m_Data[1] *= s[0];
	m_Data[2] *= s[0];

	m_Data[4] *= s[1];
	m_Data[5] *= s[1];
	m_Data[6] *= s[1];

	m_Data[8] *= s[2];
	m_Data[9] *= s[2];
	m_Data[10] *= s[2];

	m_Data[12] = pos[0];
	m_Data[13] = pos[1];
	m_Data[14] = pos[2];
}

void Matrix4x4f::SetTRInverse (const Vector3f& pos, const Quaternionf& q)
{
	QuaternionToMatrix (Inverse (q), *this);
	Translate (Vector3f (-pos[0], -pos[1], -pos[2]));
}


void EulerToMatrix(const Vector3f& v, Matrix4x4f& matrix)
{
	matrix.SetIdentity();
	float cx = cos(v.x);
	float sx = sin(v.x);
	float cy = cos(v.y);
	float sy = sin(v.y);
	float cz = cos(v.z);
	float sz = sin(v.z);

	matrix.Get(0, 0) = cy*cz + sx*sy*sz;
	matrix.Get(0, 1) = cz*sx*sy - cy*sz;
	matrix.Get(0, 2) = cx*sy;

	matrix.Get(1, 0) = cx*sz;
	matrix.Get(1, 1) = cx*cz;
	matrix.Get(1, 2) = -sx;

	matrix.Get(2, 0) = -cz*sy + cy*sx*sz;
	matrix.Get(2, 1) = cy*cz*sx + sy*sz;
	matrix.Get(2, 2) = cx*cy;
}

void TransformPoints3x3 (const Matrix4x4f& matrix, const Vector3f* in, Vector3f* out, int count)
{
	Matrix3x3f m = Matrix3x3f(matrix);
	for (int i=0;i<count;i++)
		out[i] = m.MultiplyPoint3 (in[i]);
}

void TransformPoints3x4 (const Matrix4x4f& matrix, const Vector3f* in, Vector3f* out, int count)
{
	for (int i=0;i<count;i++)
		out[i] = matrix.MultiplyPoint3 (in[i]);
}

void TransformPoints3x3 (const Matrix4x4f& matrix, const Vector3f* in, size_t inStride, Vector3f* out, size_t outStride, int count)
{
	Matrix3x3f m = Matrix3x3f(matrix);
	for (int i=0;i<count; ++i, in = Stride (in, inStride), out = Stride (out, outStride))
	{
		*out = m.MultiplyPoint3 (*in);
	}
}

void TransformPoints3x4 (const Matrix4x4f& matrix, const Vector3f* in, size_t inStride, Vector3f* out, size_t outStride, int count)
{
	for (int i=0;i<count; ++i, in = Stride (in, inStride), out = Stride (out, outStride))
	{
		*out = matrix.MultiplyPoint3 (*in);
	}
}


	#include "Matrix4x4_REF.cpp"

QIMP_END_NAMESPACE