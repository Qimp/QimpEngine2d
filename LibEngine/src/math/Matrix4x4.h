#ifndef __MATH_MATRIX4X4_H__
#define __MATH_MATRIX4X4_H__

#include "Vector3.h"
#include "Vector4.h"

QIMP_BEGIN_NAMESPACE

#if (SUPPORTS_SSE)
#	include "Simd/SimdMath.h"
#endif


class Matrix3x3f;
class Matrix4x4f;
class Quaternionf;

#if 0
/// Uniform transform scales x, y, z in the same amount,
/// NonUniform transform scales x, y, z differently and might contain skew.
/// kOddNegativeScaleTransform means that FrontFace(CCW) should be used (An odd number of scale axes is negative)
enum TransformType
{ 
	kNoScaleTransform = 0,
	kUniformScaleTransform = 1 << 0, 
	kNonUniformScaleTransform = 1 << 1,
	kOddNegativeScaleTransform = 1 << 2
};
//ENUM_FLAGS(TransformType);

inline bool IsNoScaleTransform (TransformType type) { return type == kNoScaleTransform; }
inline bool IsNonUniformScaleTransform (TransformType type) { return (type & kNonUniformScaleTransform) != 0; }

TransformType ComputeTransformType (const Matrix4x4f& matrix, float& outUniformScale, float epsilon = Vector3f::epsilon);
#endif

bool InvertMatrix4x4_Full( const float* m, float* out );
bool InvertMatrix4x4_General3D( const float* m, float* out );

/// Matrix are column major.
/*	matrix	内存数据如下保存
0	4	8	12		x

1	5	9	13		y
				*
2	6	10	14		z

3	7	11	15		1
*/

class Matrix4x4f
{
	public:
	float m_Data[16];


	///@todo: Can't be Transfer optimized because Transfer doesn't write the same as memory layout
	//DECLARE_SERIALIZE_NO_PPTR (Matrix4x4f)
	
	Matrix4x4f () {}
	Matrix4x4f (const Matrix3x3f &other);
	explicit Matrix4x4f (const float data[16]);

	void Set(float a00, float a01, float a02, float a03,
			float a10, float a11, float a12, float a13,
			float a20, float a21, float a22, float a23,
			float a30, float a31, float a32, float a33);

	float& Get (int row, int column)            { return m_Data[row + (column*4)]; }
	const float& Get (int row, int column)const { return m_Data[row + (column*4)]; }
	float* GetPtr ()                            { return m_Data; }
	const float* GetPtr ()const                 { return m_Data; }

	float operator [] (int index) const         { return m_Data[index]; }
	float& operator [] (int index)              { return m_Data[index]; }

	Matrix4x4f& operator *= (const Matrix4x4f& inM);
	Matrix4x4f& operator += (const Matrix4x4f& inM);

	Matrix4x4f& operator = (const Matrix3x3f& m);


	inline Vector3f PostMult( const Vector3f& v ) const;
	inline Vector3f operator* (const Vector3f& v) const {
		return PostMult(v);
	}

	inline Vector4f PostMult( const Vector4f& v ) const;
	inline Vector4f operator* (const Vector4f& v) const {
		return PostMult(v);
	}

	inline Vector3f PreMult( const Vector3f& v ) const;
	inline Vector4f PreMult( const Vector4f& v ) const;

	Vector3f MultiplyVector3 (const Vector3f& inV) const;
	void MultiplyVector3 (const Vector3f& inV, Vector3f& output) const;
	bool PerspectiveMultiplyVector3( const Vector3f& inV, Vector3f& output ) const;
	Vector3f MultiplyPoint3 (const Vector3f& inV) const;
	void MultiplyPoint3 (const Vector3f& inV, Vector3f& output) const;
	bool PerspectiveMultiplyPoint3( const Vector3f& inV, Vector3f& output ) const;
	Vector3f InverseMultiplyPoint3Affine (const Vector3f& inV) const;
	Vector3f InverseMultiplyVector3Affine (const Vector3f& inV) const;

	bool IsIdentity (float epsilon = Vector3f::epsilon) const;

	double GetDeterminant() const;
	
	Matrix4x4f& Invert_Full() {
		InvertMatrix4x4_Full(m_Data, m_Data);
		return *this;
	}

	Matrix4x4f GetInvert_Full()  const {
		Matrix4x4f outM;
		InvertMatrix4x4_Full( m_Data, outM.m_Data );
		return outM;
	}

	Matrix4x4f& Invert_General3D() {
		Matrix4x4f matrix(*this);
		InvertMatrix4x4_General3D(matrix.m_Data, m_Data);
		return *this;
	}

	Matrix4x4f GetInvert_General3D() const {
		Matrix4x4f outM;
		InvertMatrix4x4_General3D( m_Data, outM.m_Data );
		return outM;
	}

	Matrix4x4f& Transpose ();

	Matrix4x4f& Copy (const Matrix4x4f& inM);
	
	Matrix4x4f& SetIdentity ();
	Matrix4x4f& SetPerspective( float fovy, float aspect, float zNear, float zFar );
	// rad = Deg2Rad(fovy/2), contanHalfFOV = cos(rad)/sin(rad)
	Matrix4x4f& SetPerspectiveCotan( float cotanHalfFOV, float zNear, float zFar );
	Matrix4x4f& SetOrtho( float left, float right, float bottom, float top, float zNear, float zFar );
	Matrix4x4f& SetFrustum( float left, float right, float bottom, float top, float nearval, float farval );

	Vector3f GetAxisX() const;
	Vector3f GetAxisY() const;
	Vector3f GetAxisZ() const;
	Vector3f GetPosition() const;
	Quaternionf GetRotation() const;
	Vector3f GetScale() const;
	Vector4f GetRow(int row) const;
	Vector4f GetColumn(int col) const;
	// these set only these components of the matrix, everything else is untouched!
	void SetAxisX( const Vector3f& v );
	void SetAxisY( const Vector3f& v );
	void SetAxisZ( const Vector3f& v );
	void SetPosition( const Vector3f& v );
	void SetRow( int row, const Vector4f& v );
	void SetRow( int row, float inX, float inY, float inZ, float inW);
	void SetColumn( int col, const Vector4f& v );
	void SetColumn( int col, float inX, float inY, float inZ, float inW);

	Vector3f GetPosition();
	Matrix4x4f& SetTranslate (const Vector3f& inTrans);
	Matrix4x4f& SetOrthoNormalBasis (const Vector3f& inX, const Vector3f& inY, const Vector3f& inZ);
	Matrix4x4f& SetOrthoNormalBasisInverse (const Vector3f& inX, const Vector3f& inY, const Vector3f& inZ);
	Matrix4x4f& SetScale (const Vector3f& inScale);
	Matrix4x4f& SetPositionAndOrthoNormalBasis (const Vector3f& inPosition, const Vector3f& inX, const Vector3f& inY, const Vector3f& inZ);

	Matrix4x4f& Translate (const Vector3f& inTrans);
	Matrix4x4f& Scale (const Vector3f& inScale);
	Matrix4x4f& Scale(const Vector4f& inScale);
	
	Matrix4x4f& SetFromToRotation (const Vector3f& from, const Vector3f& to);
	
	void SetTR (const Vector4f& pos, const Quaternionf& q);
	void SetTR (const Vector3f& pos, const Quaternionf& q);
	void SetTRS (const Vector3f& pos, const Quaternionf& q, const Vector3f& s);
	void SetTRInverse (const Vector3f& pos, const Quaternionf& q);
	
	static const Matrix4x4f identity;
};

bool CompareApproximately (const Matrix4x4f& lhs, const Matrix4x4f& rhs, float dist = Vector3f::epsilon);

void EulerToMatrix(const Vector3f& v, Matrix4x4f& matrix);

/// Transforms an array of vertices. input may be the same as output.
void TransformPoints3x3 (const Matrix4x4f &matrix, const Vector3f* input, Vector3f* ouput, int count);
void TransformPoints3x4 (const Matrix4x4f &matrix, const Vector3f* input, Vector3f* ouput, int count);
void TransformPoints3x3 (const Matrix4x4f &matrix, const Vector3f* input, size_t inStride, Vector3f* ouput, size_t outStride, int count);
void TransformPoints3x4 (const Matrix4x4f &matrix, const Vector3f* input, size_t inStride, Vector3f* ouput, size_t outStride, int count);

void MultiplyMatrix3x4( const Matrix4x4f& lhs, const Matrix4x4f& rhs, Matrix4x4f& res);

void MultiplyMatrix4x4REF(const Matrix4x4f* __restrict lhs, const Matrix4x4f* __restrict rhs, Matrix4x4f* __restrict res);
void CopyMatrixREF( const float* __restrict lhs, float* __restrict res);
void TransposeMatrix4x4REF (const Matrix4x4f* __restrict lhs, Matrix4x4f* __restrict res);

// foreach R[i] = A[i] * B[i]
void MultiplyMatrixArray4x4REF(const Matrix4x4f* __restrict arrayA, const Matrix4x4f* __restrict arrayB,
							   Matrix4x4f* __restrict arrayRes, size_t count);
// foreach R[i] = BASE * A[i] * B[i]
void MultiplyMatrixArrayWithBase4x4REF (const Matrix4x4f* __restrict base,
										const Matrix4x4f* __restrict arrayA, const Matrix4x4f* __restrict arrayB,
										Matrix4x4f* __restrict arrayRes, size_t count);

#if (AUTO_DETECT_VECTOR_UNIT && SUPPORTS_SSE)
#	define DECLARE_SIMD_FUNC(f) f##Simd
#else
#	define DECLARE_SIMD_FUNC(f) f
#endif

#if (SUPPORTS_SSE)
#	include "Simd/Matrix4x4Simd.h"
#elif SUPPORTS_NEON

#if PLATFORM_ANDROID || PLATFORM_WINRT
	#define MultiplyMatrix4x4_NEON				_MultiplyMatrix4x4_NEON
	#define CopyMatrix_NEON							_CopyMatrix_NEON
	#define TransposeMatrix4x4_NEON					_TransposeMatrix4x4_NEON

	#define	MultiplyMatrixArray4x4_NEON				_MultiplyMatrixArray4x4_NEON
	#define	MultiplyMatrixArrayWithBase4x4_NEON		_MultiplyMatrixArrayWithBase4x4_NEON
#endif

extern "C" 
{
	void CopyMatrix_NEON(const float* __restrict lhs, float* __restrict res);
	void TransposeMatrix4x4_NEON(const Matrix4x4f* __restrict lhs, Matrix4x4f* __restrict res);
	
	void MultiplyMatrix4x4_NEON(const Matrix4x4f* __restrict lhs, const Matrix4x4f* __restrict rhs, Matrix4x4f* __restrict res);
	void MultiplyMatrixArray4x4_NEON(const Matrix4x4f* __restrict arrayA, const Matrix4x4f* __restrict arrayB,
									 Matrix4x4f* __restrict arrayRes, size_t count);
	void MultiplyMatrixArrayWithBase4x4_NEON(const Matrix4x4f* __restrict base,
											 const Matrix4x4f* __restrict arrayA, const Matrix4x4f* __restrict arrayB,
											 Matrix4x4f* __restrict arrayRes, size_t count);
}

#if PLATFORM_ANDROID && SUPPORTS_NEON && SUPPORTS_VFP

	#define MultiplyMatrix4x4_VFP			_MultiplyMatrix4x4_VFP
	#define	MultiplyMatrixArray4x4_VFP		_MultiplyMatrixArray4x4_VFP

	extern "C"
	{
		void MultiplyMatrix4x4_VFP(const Matrix4x4f* __restrict lhs, const Matrix4x4f* __restrict rhs, Matrix4x4f* __restrict res);
		void MultiplyMatrixArray4x4_VFP(const Matrix4x4f* __restrict arrayA, const Matrix4x4f* __restrict arrayB,
										Matrix4x4f* __restrict arrayRes, size_t count);
	}

	#define CopyMatrix(a,b)								CPUInfo::HasNEONSupport() ? CopyMatrix_NEON(a,b) : CopyMatrixREF(a,b)
	#define TransposeMatrix4x4(a,b)						CPUInfo::HasNEONSupport() ? TransposeMatrix4x4_NEON(a,b) : TransposeMatrix4x4REF(a,b)

	#define MultiplyMatrix4x4(a,b,c)					CPUInfo::HasNEONSupport() ? MultiplyMatrix4x4_NEON(a,b,c) : MultiplyMatrix4x4_VFP(a,b,c)
	#define MultiplyMatrixArray4x4(a,b,c,d)				CPUInfo::HasNEONSupport() ? MultiplyMatrixArray4x4_NEON(a,b,c,d) : MultiplyMatrixArray4x4_VFP(a,b,c,d)
	#define MultiplyMatrixArrayWithBase4x4(a,b,c,d,e)	CPUInfo::HasNEONSupport() ? MultiplyMatrixArrayWithBase4x4_NEON(a,b,c,d,e) : MultiplyMatrixArrayWithBase4x4REF(a,b,c,d,e)

#else
	
	#define CopyMatrix			CopyMatrix_NEON
	#define TransposeMatrix4x4	TransposeMatrix4x4_NEON

	#define MultiplyMatrix4x4	MultiplyMatrix4x4_NEON
	#define MultiplyMatrixArray4x4			MultiplyMatrixArray4x4_NEON
	#define MultiplyMatrixArrayWithBase4x4	MultiplyMatrixArrayWithBase4x4_NEON

#endif
	
#elif SUPPORTS_VFP

#if PLATFORM_ANDROID
	#define MultiplyMatrix4x4_VFP			_MultiplyMatrix4x4_VFP
	#define	MultiplyMatrixArray4x4_VFP		_MultiplyMatrixArray4x4_VFP
#endif

extern "C" 
{
	void MultiplyMatrix4x4_VFP(const Matrix4x4f* __restrict lhs, const Matrix4x4f* __restrict rhs, Matrix4x4f* __restrict res);
	void MultiplyMatrixArray4x4_VFP(const Matrix4x4f* __restrict arrayA, const Matrix4x4f* __restrict arrayB,
									Matrix4x4f* __restrict arrayRes, size_t count);
}

	#define CopyMatrix						CopyMatrixREF
	#define TransposeMatrix4x4				TransposeMatrix4x4REF
	
	#define MultiplyMatrix4x4				MultiplyMatrix4x4_VFP
	#define MultiplyMatrixArray4x4			MultiplyMatrixArray4x4_VFP
	#define MultiplyMatrixArrayWithBase4x4	MultiplyMatrixArrayWithBase4x4REF


#else

	#define CopyMatrix						CopyMatrixREF
	#define TransposeMatrix4x4				TransposeMatrix4x4REF

	#define MultiplyMatrix4x4				MultiplyMatrix4x4REF
	#define MultiplyMatrixArray4x4			MultiplyMatrixArray4x4REF
	#define MultiplyMatrixArrayWithBase4x4	MultiplyMatrixArrayWithBase4x4REF

#endif


inline Vector3f Matrix4x4f::GetAxisX() const {
	return Vector3f( Get(0,0), Get(1,0), Get(2,0) );
}
inline Vector3f Matrix4x4f::GetAxisY() const {
	return Vector3f( Get(0,1), Get(1,1), Get(2,1) );
}
inline Vector3f Matrix4x4f::GetAxisZ() const {
	return Vector3f( Get(0,2), Get(1,2), Get(2,2) );
}
inline Vector3f Matrix4x4f::GetPosition() const {
	return Vector3f( Get(0,3), Get(1,3), Get(2,3) );
}
inline Vector4f Matrix4x4f::GetRow(int row) const {
	return Vector4f( Get(row,0), Get(row,1), Get(row,2), Get(row,3) );
}
inline Vector4f Matrix4x4f::GetColumn(int col) const {
	return Vector4f( Get(0,col), Get(1,col), Get(2,col), Get(3,col) );
}
inline void Matrix4x4f::SetAxisX( const Vector3f& v ) {
	Get(0,0) = v.x; Get(1,0) = v.y; Get(2,0) = v.z;
}
inline void Matrix4x4f::SetAxisY( const Vector3f& v ) {
	Get(0,1) = v.x; Get(1,1) = v.y; Get(2,1) = v.z;
}
inline void Matrix4x4f::SetAxisZ( const Vector3f& v ) {
	Get(0,2) = v.x; Get(1,2) = v.y; Get(2,2) = v.z;
}
inline void Matrix4x4f::SetPosition( const Vector3f& v ) {
	Get(0,3) = v.x; Get(1,3) = v.y; Get(2,3) = v.z;
}
inline void Matrix4x4f::SetRow( int row, const Vector4f& v ) {
	Get(row,0) = v.x; Get(row,1) = v.y; Get(row,2) = v.z; Get(row,3) = v.w;
}
inline void Matrix4x4f::SetRow( int row, float inX, float inY, float inZ, float inW) {
	Get(row,0) = inX; Get(row,1) = inY; Get(row,2) = inZ; Get(row,3) = inW;
}
inline void Matrix4x4f::SetColumn( int col, const Vector4f& v ) {
	Get(0,col) = v.x; Get(1,col) = v.y; Get(2,col) = v.z; Get(3,col) = v.w;
}
inline void Matrix4x4f::SetColumn( int col, float inX, float inY, float inZ, float inW) {
	Get(0,col) = inX; Get(1,col) = inY; Get(2,col) = inZ; Get(3,col) = inW;
}
inline Vector3f Matrix4x4f::GetPosition() {
	return Vector3f(Get(0,3), Get(1,3), Get(2,3));
}

inline Vector3f Matrix4x4f::PostMult( const Vector3f& v ) const {
	float d = 1.0f / (Get(3, 0) * v.x + Get(3, 1) * v.y + Get(3, 2) * v.z + Get(3, 3)) ;
	return Vector3f( (Get(0, 0)*v.x + Get(0, 1)*v.y + Get(0, 2)*v.z + Get(0, 3))*d,
		(Get(1, 0)*v.x + Get(1, 1)*v.y + Get(1, 2)*v.z + Get(1, 3))*d,
		(Get(2, 0)*v.x + Get(2, 1)*v.y + Get(2, 2)*v.z + Get(2, 3))*d) ;
}

inline Vector3f Matrix4x4f::PreMult( const Vector3f& v ) const {
	float d = 1.0f / (Get(0, 3) * v.x + Get(1, 3) * v.y + Get(2, 3) * v.z + Get(3, 3)) ;
	return Vector3f( (Get(0, 0)*v.x + Get(1, 0)*v.y + Get(2, 0)*v.z + Get(3, 0))*d,
		(Get(0, 1)*v.x + Get(1, 1)*v.y + Get(2, 1)*v.z + Get(3, 1))*d,
		(Get(0, 2)*v.x + Get(1, 2)*v.y + Get(2, 2)*v.z + Get(3, 2))*d);
}

inline Vector4f Matrix4x4f::PostMult( const Vector4f& v ) const {
	return Vector4f( (Get(0, 0)*v.x + Get(0, 1)*v.y + Get(0, 2)*v.z + Get(0, 3)*v.w),
		(Get(1, 0)*v.x + Get(1, 1)*v.y + Get(1, 2)*v.z + Get(1, 3)*v.w),
		(Get(2, 0)*v.x + Get(2, 1)*v.y + Get(2, 2)*v.z + Get(2, 3)*v.w),
		(Get(3, 0)*v.x + Get(3, 1)*v.y + Get(3, 2)*v.z + Get(3, 3)*v.w)) ;
}
inline Vector4f Matrix4x4f::PreMult( const Vector4f& v ) const {
	return Vector4f( (Get(0, 0)*v.x + Get(1, 0)*v.y + Get(2, 0)*v.z + Get(3, 0)*v.w),
		(Get(0, 1)*v.x + Get(1, 1)*v.y + Get(2, 1)*v.z + Get(3, 1)*v.w),
		(Get(0, 2)*v.x + Get(1, 2)*v.y + Get(2, 2)*v.z + Get(3, 2)*v.w),
		(Get(0, 3)*v.x + Get(1, 3)*v.y + Get(2, 3)*v.z + Get(3, 3)*v.w));
}
inline Vector3f Matrix4x4f::MultiplyPoint3 (const Vector3f& v) const
{
	Vector3f res;
	res.x = m_Data[0] * v.x + m_Data[4] * v.y + m_Data[ 8] * v.z + m_Data[12];
	res.y = m_Data[1] * v.x + m_Data[5] * v.y + m_Data[ 9] * v.z + m_Data[13];
	res.z = m_Data[2] * v.x + m_Data[6] * v.y + m_Data[10] * v.z + m_Data[14];
	return res;
}

inline void Matrix4x4f::MultiplyPoint3 (const Vector3f& v, Vector3f& output) const
{
	output.x = m_Data[0] * v.x + m_Data[4] * v.y + m_Data[ 8] * v.z + m_Data[12];
	output.y = m_Data[1] * v.x + m_Data[5] * v.y + m_Data[ 9] * v.z + m_Data[13];
	output.z = m_Data[2] * v.x + m_Data[6] * v.y + m_Data[10] * v.z + m_Data[14];
}


inline Vector3f Matrix4x4f::MultiplyVector3 (const Vector3f& v) const
{
	Vector3f res;
	res.x = m_Data[0] * v.x + m_Data[4] * v.y + m_Data[ 8] * v.z;
	res.y = m_Data[1] * v.x + m_Data[5] * v.y + m_Data[ 9] * v.z;
	res.z = m_Data[2] * v.x + m_Data[6] * v.y + m_Data[10] * v.z;
	return res;
}

inline void Matrix4x4f::MultiplyVector3 (const Vector3f& v, Vector3f& output) const
{
	output.x = m_Data[0] * v.x + m_Data[4] * v.y + m_Data[ 8] * v.z;
	output.y = m_Data[1] * v.x + m_Data[5] * v.y + m_Data[ 9] * v.z;
	output.z = m_Data[2] * v.x + m_Data[6] * v.y + m_Data[10] * v.z;
}


inline bool Matrix4x4f::PerspectiveMultiplyPoint3( const Vector3f& v, Vector3f& output ) const
{
	Vector3f res;
	float w;
	res.x = Get (0, 0) * v.x + Get (0, 1) * v.y + Get (0, 2) * v.z + Get (0, 3); 
	res.y = Get (1, 0) * v.x + Get (1, 1) * v.y + Get (1, 2) * v.z + Get (1, 3);
	res.z = Get (2, 0) * v.x + Get (2, 1) * v.y + Get (2, 2) * v.z + Get (2, 3);
	w     = Get (3, 0) * v.x + Get (3, 1) * v.y + Get (3, 2) * v.z + Get (3, 3);
	if( Abs(w) > 1.0e-7f )
	{
		float invW = 1.0f / w;
		output.x = res.x * invW;
		output.y = res.y * invW;
		output.z = res.z * invW;
		return true;
	}
	else
	{
		output.x = 0.0f;
		output.y = 0.0f;
		output.z = 0.0f;
		return false;
	}
}

inline bool Matrix4x4f::PerspectiveMultiplyVector3( const Vector3f& v, Vector3f& output ) const
{
	Vector3f res;
	float w;
	res.x = Get (0, 0) * v.x + Get (0, 1) * v.y + Get (0, 2) * v.z;
	res.y = Get (1, 0) * v.x + Get (1, 1) * v.y + Get (1, 2) * v.z;
	res.z = Get (2, 0) * v.x + Get (2, 1) * v.y + Get (2, 2) * v.z;
	w     = Get (3, 0) * v.x + Get (3, 1) * v.y + Get (3, 2) * v.z;
	if( Abs(w) > 1.0e-7f )
	{
		float invW = 1.0f / w;
		output.x = res.x * invW;
		output.y = res.y * invW;
		output.z = res.z * invW;
		return true;
	}
	else
	{
		output.x = 0.0f;
		output.y = 0.0f;
		output.z = 0.0f;
		return false;
	}
}

inline Vector3f Matrix4x4f::InverseMultiplyPoint3Affine (const Vector3f& inV) const
{
	Vector3f v (inV.x - Get (0, 3), inV.y - Get (1, 3), inV.z - Get (2, 3));
	Vector3f res;
	res.x = Get (0, 0) * v.x + Get (1, 0) * v.y + Get (2, 0) * v.z;
	res.y = Get (0, 1) * v.x + Get (1, 1) * v.y + Get (2, 1) * v.z;
	res.z = Get (0, 2) * v.x + Get (1, 2) * v.y + Get (2, 2) * v.z;
	return res;
}

inline Vector3f Matrix4x4f::InverseMultiplyVector3Affine (const Vector3f& v) const
{
	Vector3f res;
	res.x = Get (0, 0) * v.x + Get (1, 0) * v.y + Get (2, 0) * v.z;
	res.y = Get (0, 1) * v.x + Get (1, 1) * v.y + Get (2, 1) * v.z;
	res.z = Get (0, 2) * v.x + Get (1, 2) * v.y + Get (2, 2) * v.z;
	return res;
}


inline bool IsFinite (const Matrix4x4f& f)
{
	return
	IsFinite(f.m_Data[0]) & IsFinite(f.m_Data[1]) & IsFinite(f.m_Data[2]) &
	IsFinite(f.m_Data[4]) & IsFinite(f.m_Data[5]) & IsFinite(f.m_Data[6]) &
	IsFinite(f.m_Data[8]) & IsFinite(f.m_Data[9]) & IsFinite(f.m_Data[10]) &
	IsFinite(f.m_Data[12]) & IsFinite(f.m_Data[13]) & IsFinite(f.m_Data[14]) & IsFinite(f.m_Data[15]);
}

inline Matrix4x4f operator * (const Matrix4x4f& lhs, const Matrix4x4f& rhs)
{
	Matrix4x4f result;
	MultiplyMatrix4x4(&lhs, &rhs, &result);
	return result;
}

struct Matrix4x3f
{
	float m_Data[12];
	Matrix4x3f()
	{
		memset(m_Data, 0, sizeof(m_Data));
	}
	void Set(Matrix4x4f& m)
	{
		// Matrix4x4 的坐标信息保存在 12/13/14 ( Ex: Matrix * Vector4f 左乘 )
		// 需要转换为 3/7/11 ( Ex: Vector4f * Matrix 右乘 )
		m_Data[0] = m.m_Data[0]; m_Data[1] = m.m_Data[4]; m_Data[2] = m.m_Data[8]; m_Data[3] = m.m_Data[12];
		m_Data[4] = m.m_Data[1]; m_Data[5] = m.m_Data[5]; m_Data[6] = m.m_Data[9]; m_Data[7] = m.m_Data[13];
		m_Data[8] = m.m_Data[2]; m_Data[9] = m.m_Data[6]; m_Data[10] = m.m_Data[10]; m_Data[11] = m.m_Data[14];
	}
	void Get(Matrix4x4f& m)
	{
		m.m_Data[0] = m_Data[0]; m.m_Data[1] = m_Data[4]; m.m_Data[2] = m_Data[8]; m.m_Data[3] = 0.f;
		m.m_Data[4] = m_Data[1]; m.m_Data[5] = m_Data[5]; m.m_Data[6] = m_Data[9]; m.m_Data[7] = 0.f;
		m.m_Data[8] = m_Data[2]; m.m_Data[9] = m_Data[6]; m.m_Data[10] = m_Data[10]; m.m_Data[11] = 0.f;
		m.m_Data[12] = m_Data[3]; m.m_Data[13] = m_Data[7]; m.m_Data[14] = m_Data[11]; m.m_Data[15] = 1.f;
	}
};

typedef std::vector<Matrix4x3f>	Matrix4x3fs;

QIMP_END_NAMESPACE

#endif //__MATH_MATRIX4X4_H__