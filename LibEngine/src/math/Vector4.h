#ifndef __MATH_VECTOR4_H__
#define __MATH_VECTOR4_H__

#pragma once

#include "Vector3.h"

QIMP_BEGIN_NAMESPACE

class Vector4f
{
public:
	Vector4f() {}
	Vector4f( float inX, float inY, float inZ, float inW ) : x(inX), y(inY), z(inZ), w(inW) {}
	explicit Vector4f( const Vector3f& v, float inW ) : x(v.x), y(v.y), z(v.z), w(inW) {}
	explicit Vector4f( const float* v ) : x(v[0]), y(v[1]), z(v[2]), w(v[3]) {}
	static Vector4f Create(float inX, float inY, float inZ, float inW)
	{
		Vector4f p(inX, inY, inZ, inW);
		return p;
	}
	void Set( float inX, float inY, float inZ, float inW )				{ x = inX; y = inY; z = inZ; w = inW; }
	void Set( const float* array )										{ x = array[0]; y = array[1]; z = array[2]; w = array[3]; }

	float* GetPtr()														{ return &x; }
	const float* const GetPtr() const									{ return &x; }

	float& operator[] (int i)											{ AssertIf (i < 0 || i > 3); return (&x)[i]; }
	const float& operator[] (int i)const								{ AssertIf (i < 0 || i > 3); return (&x)[i]; }

	Vector3f GetVector3f() 												{ return Vector3f(x, y, z); }

	Vector4f& operator = (const Vector3f& v)							{ x = v.x; y = v.y; z = v.z; w = 1.0f; return *this; }

	bool operator == (const Vector4f& v) const							{ return x == v.x && y == v.y && z == v.z && w == v.w; }
	bool operator != (const Vector4f& v) const							{ return x != v.x || y != v.y || z != v.z || w != v.w; }
	bool operator == (const float v[4]) const							{ return x == v[0] && y == v[1] && z == v[2] && w == v[3]; }
	bool operator != (const float v[4]) const							{ return x != v[0] || y != v[1] || z != v[2] || w != v[3]; }

	Vector4f& operator += (const Vector4f& v)							{ x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
	Vector4f& operator -= (const Vector4f& v)							{ x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
	Vector4f& operator *= (const Vector4f& v)							{ x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }
	Vector4f& operator *= (float s)										{ x *= s; y *= s; z *= s; w *= s; return *this; }
	Vector4f& operator /= (float s);

	void operator () (float inX, float inY, float inZ, float inW)		{ x = inX; y = inY; z = inZ; w = inW;  }
	Vector4f operator - () const										{ return Vector4f (-x, -y, -z, -w); }

	Vector4f& Scale (const Vector4f& v)									{ x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this;}
	inline float Length() const											{ return (float)sqrt(x*x + y*y + z*z + w*w); }
	inline float LengthSquared() const									{ return (float)(x*x + y*y + z*z + w*w); }

	Vector4f& Normalize()												{ float s = 1.0f / Length(); x *= s; y *= s; z *= s; w *= s; return *this; }
	Vector4f& NormalizeSafe()
	{
		float mag = this->Length();
		if (mag > Vector4f::epsilon)
		{
			float s = 1.0f / mag;
			x *= s;	y *= s;	z *= s;	w *= s;
		}
		return (*this);
	}



	static const float		epsilon;
	static const float		infinity;
	static const Vector4f	infinityVec;
	static const Vector4f	zero;
	static const Vector4f	one;

	//DEFINE_GET_TYPESTRING_IS_ANIMATION_CHANNEL (Vector4f)
	//template<class TransferFunction> void Transfer (TransferFunction& transfer);

	float x;
	float y;
	float z;
	float w;
};


inline Vector4f operator * (const Vector4f& lhs, const Vector4f& rhs)	{ return Vector4f (lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w); }
inline Vector4f operator * (const Vector4f& inV, const float s)			{ return Vector4f (inV.x * s, inV.y * s, inV.z * s, inV.w * s); }
inline Vector4f operator + (const Vector4f& lhs, const Vector4f& rhs)	{ return Vector4f (lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w); }
inline Vector4f operator - (const Vector4f& lhs, const Vector4f& rhs)	{ return Vector4f (lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w); }
inline float Dot (const Vector4f& lhs, const Vector4f& rhs)				{ return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w; }

inline float LengthSqr(const Vector4f& inV) 							{ return Dot(inV, inV); }
inline float Length(const Vector4f& inV)								{ return std::sqrt(Dot(inV, inV)); }

inline float Distance (const Vector4f& lhs, const Vector4f& rhs)		{ Vector4f out = lhs - rhs; return (float)sqrt(out.x*out.x + out.y*out.y + out.z*out.z + out.w*out.w); }

inline Vector4f Lerp (const Vector4f& from, const Vector4f& to, float t) { return to * t + from * (1.0F - t); }

inline Vector4f min (const Vector4f& lhs, const Vector4f& rhs) 			{ return Vector4f (std::min (lhs.x, rhs.x), std::min (lhs.y, rhs.y), std::min (lhs.z, rhs.z), std::min (lhs.z, rhs.w)); }
inline Vector4f max (const Vector4f& lhs, const Vector4f& rhs)			{ return Vector4f (std::max (lhs.x, rhs.x), std::max (lhs.y, rhs.y), std::max (lhs.z, rhs.z), std::max (lhs.z, rhs.w)); }


inline Vector4f& Vector4f::operator /= (float s)
{
	AssertIf (CompareApproximately (s, 0.0F));
	x /= s;
	y /= s;
	z /= s;
	w /= s;
	return *this;
}

QIMP_END_NAMESPACE

#endif//__MATH_VECTOR4_H__