#ifndef VECTOR2_H
#define VECTOR2_H

#include <algorithm>
#include "FloatConversion.h"

QIMP_BEGIN_NAMESPACE

class Vector2f
{
	public:
	float x, y;
		
	//DECLARE_SERIALIZE_OPTIMIZE_TRANSFER (Vector2f)
	
	Vector2f () : x(0.f) , y(0.f) {}
	Vector2f (float inX, float inY)												{ x = inX; y = inY; }	
	explicit Vector2f (const float* p)											{ x = p[0]; y = p[1]; }	
			
	void Set (float inX, float inY)												{ x = inX; y = inY; }

	float* GetPtr ()															{ return &x; }
	const float* const GetPtr ()const											{ return &x; }
	float& operator[] (int i)													{ AssertIf (i < 0 || i > 1); return (&x)[i]; }
	const float& operator[] (int i)const										{ AssertIf (i < 0 || i > 1); return (&x)[i]; }
	
	void operator () (float inX, float inY)										{ x = inX; y = inY; }
	Vector2f& operator += (const Vector2f& inV)									{ x += inV.x; y += inV.y; return *this; }
	Vector2f& operator -= (const Vector2f& inV)									{ x -= inV.x; y -= inV.y; return *this; }
	Vector2f& operator *= (const float s)										{ x *= s; y *= s; return *this; }
	Vector2f& operator /= (const float s)										{ Assert(!CompareApproximately (s, 0.0f)); x /= s; y /= s; return *this; }
	bool operator == (const Vector2f& v)const									{ return x == v.x && y == v.y; }
	bool operator != (const Vector2f& v)const									{ return x != v.x || y != v.y; }

	friend Vector2f operator + (const Vector2f& lhs, const Vector2f& rhs)		{ return Vector2f(lhs.x + rhs.x, lhs.y + rhs.y); }
	friend Vector2f operator - (const Vector2f& lhs, const Vector2f& rhs)		{ return Vector2f(lhs.x - rhs.x, lhs.y - rhs.y); }
	friend Vector2f operator * (const Vector2f& inV, float s)					{ return Vector2f(inV.x * s, inV.y * s); }
	friend Vector2f operator * (const float s, const Vector2f& inV)				{ return Vector2f(inV.x * s, inV.y * s); }
	friend Vector2f operator / (const Vector2f& inV, float s)					{ Vector2f temp(inV); temp /= s; return temp; }

	Vector2f operator - () const												{ return Vector2f(-x, -y); }
	Vector2f operator + (const Vector2f& inV)									{ Vector2f t(*this); t += inV; return t; }
	Vector2f operator - (const Vector2f& inV)									{ Vector2f t(*this); t -= inV; return t; }
	Vector2f operator * (const float inV)										{ Vector2f t(*this); t *= inV; return t; }
	Vector2f operator / (const float inV)										{ Vector2f t(*this); t /= inV; return t; }

	Vector2f& Scale (const Vector2f& inV)										{ x *= inV.x; y *= inV.y; return *this;}
	friend Vector2f Scale (const Vector2f& lhs, const Vector2f& rhs) 			{ return Vector2f (lhs.x * rhs.x, lhs.y * rhs.y); }
	friend float Dot (const Vector2f& lhs, const Vector2f& rhs)					{ return lhs.x * rhs.x + lhs.y * rhs.y; }
	friend float Angle (const Vector2f& lhs, const Vector2f& rhs)				{ return acos (std::min (1.0f, std::max (-1.0f, Dot (lhs, rhs) / (Magnitude (lhs) * Magnitude (rhs))))); }
	
	inline float Length() const													{ return (float)sqrt( x*x + y*y); }
	inline float LengthSquared() const											{ return (float)(x*x + y*y); }
	friend float Distance (const Vector2f& lhs, const Vector2f& rhs)			{ Vector2f out = lhs - rhs; return (float)sqrt(out.x*out.x + out.y*out.y); }

	friend Vector2f Inverse (const Vector2f& inVec)								{ return Vector2f (1.0F / inVec.x, 1.0F / inVec.y); }
        
	friend float SqrMagnitude (const Vector2f& inV) 							{ return Dot (inV, inV); }
	friend float Magnitude (const Vector2f& inV)								{return std::sqrt(Dot (inV, inV));}

	Vector2f& Normalize()														{ float s = 1.0f / Length(); x *= s; y *= s; return *this; }
	Vector2f& NormalizeSafe()
	{
		float mag = this->Length();
		if (mag > Vector2f::epsilon)
		{
			float s = 1.0f / mag;
			x *= s;	y *= s;
		}
		return *this;
	}

//	friend Vector2f Abs (const Vector2f& inV)											{ return Vector2f (::Abs (inV.x), ::Abs (inV.y)); }
		
	friend Vector2f Lerp (const Vector2f& from, const Vector2f& to, float t)	{return to * t + from * (1.0f - t);}
	
	// Returns a vector with the smaller of every component from v0 and v1
	friend Vector2f min (const Vector2f& lhs, const Vector2f& rhs) 				{ return Vector2f (std::min (lhs.x, rhs.x), std::min (lhs.y, rhs.y)); }
	// Returns a vector with the larger  of every component from v0 and v1
	friend Vector2f max (const Vector2f& lhs, const Vector2f& rhs)				{ return Vector2f (std::max (lhs.x, rhs.x), std::max (lhs.y, rhs.y)); }
	
	static const float		epsilon;
	static const float		infinity;
	static const Vector2f	infinityVec;
	static const Vector2f	zero;
	static const Vector2f	xAxis;
	static const Vector2f	yAxis;
};



bool CompareApproximately (const Vector2f& inV0, const Vector2f& inV1, float inMaxDist = Vector2f::epsilon);

inline bool CompareApproximately (const Vector2f& inV0, const Vector2f& inV1, float inMaxDist)
{
	return SqrMagnitude (inV1 - inV0) < inMaxDist * inMaxDist;
}

/// Returns the abs of every component of the vector
inline Vector2f Abs (const Vector2f& v) { return Vector2f (std::abs(v.x), std::abs(v.y)); }

inline Vector2f NormalizeSafe (const Vector2f& inV)
{
	float mag = Magnitude (inV);
	if (mag > Vector2f::epsilon)
		return inV / Magnitude (inV);
	else
		return Vector2f::zero;
}

QIMP_END_NAMESPACE

#endif //__MATH_VECTOR2_H__