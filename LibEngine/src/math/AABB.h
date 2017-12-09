#ifndef __MATH_AABB_H__
#define __MATH_AABB_H__

#include "math/Vector3.h"
#include "math/Matrix4x4.h"

QIMP_BEGIN_NAMESPACE

	class MinMaxAABB;
	class Quaternionf;
	class Matrix3x3f;


class AABB
{
public:
	static const AABB	zero;

public:
	Vector3f			m_Center;
	Vector3f			m_Extent;

private:
	void				FromMinMaxAABB(const MinMaxAABB& aabb);

public:
	AABB () {}
	AABB(const Vector3f& c, const Vector3f& e)										{ m_Center = c; m_Extent = e; }
	AABB(const MinMaxAABB& aabb)													{ FromMinMaxAABB(aabb); }
	AABB& operator = (const MinMaxAABB& aabb)										{ FromMinMaxAABB(aabb); return *this; }

	bool operator == (const AABB& b) const											{ return m_Center == b.m_Center && m_Extent == b.m_Extent; }
	bool operator != (const AABB& b) const											{ return m_Center != b.m_Center || m_Extent != b.m_Extent; }
	
	void				SetCenterAndExtent( const Vector3f& c, const Vector3f& e )	{ m_Center = c; m_Extent = e; }
	
	const Vector3f&		GetCenter()													{ return m_Center; }
	const Vector3f&		GetExtent()													{ return m_Extent; }
	float&				GetExtent(int i)											{ return m_Extent[i]; }
	const Vector3f&		GetCenter() const											{ return m_Center; }
	const Vector3f&		GetExtent () const											{ return m_Extent; }
	float				GetExtent (int i) const										{ return m_Extent[i]; }
	Vector3f			GetMin() const												{ return m_Center - m_Extent; }
	Vector3f			GetMax() const												{ return m_Center + m_Extent; }
	
	void				Expand(float inValue);
	bool				IsValid() const;
	bool				IsInside(const Vector3f& inPoint) const;
	void				GetVertices(Vector3f* outVertices) const;
	void				Encapsulate(const Vector3f& inPoint);
};

bool IsContainedInAABB (const AABB& inside, const AABB& bigBounds);


// Find minimum AABB which includes both AABBs
MinMaxAABB AddAABB (const MinMaxAABB& lhs, const MinMaxAABB& rhs);

// Transforms AABB.
// Can be thought of as Converting OBB to an AABB:
// rotate the center and extents of the OBB And find the smallest enclosing AABB around it.
void TransformAABB (const AABB& aabb, const Vector3f& position, const Quaternionf& rotation, AABB& result);

/// This is not mathematically correct for non-uniform scaled objects. But it seems to work well enough.
/// If you use it with non-uniform scale make sure to verify it extensively.
void TransformAABB (const AABB& aabb, const Matrix4x4f& transform, AABB& result);

/// This version is much slower but works correctly with non-uniform scale
void TransformAABBSlow (const AABB& aabb, const Matrix4x4f& transform, AABB& result);

void InverseTransformAABB (const AABB& aabb, const Vector3f& position, const Quaternionf& rotation, AABB& result);


/// The closest distance to the surface or inside the aabb.
float CalculateSqrDistance (const Vector3f& rkPoint, const AABB& rkBox);


/// Returns the sqr distance and the closest point inside or on the surface of the aabb.
/// If inside the aabb, distance will be zero and rkPoint will be returned.
void CalculateClosestPoint (const Vector3f& rkPoint, const AABB& rkBox, Vector3f& outPoint, float& outSqrDistance);

class MinMaxAABB
{
public:
	
	Vector3f	m_Min;
	Vector3f	m_Max;
	
	MinMaxAABB ()										{ Init (); }
	MinMaxAABB (Vector3f min, Vector3f max) : m_Min(min), m_Max(max) { };
	MinMaxAABB (const AABB& aabb)					{ FromAABB (aabb); }
	MinMaxAABB& operator = (const AABB& aabb)	{ FromAABB (aabb); return *this; }
	MinMaxAABB& operator *=(float s);
	MinMaxAABB& operator *=(const Vector3f& inV);
	MinMaxAABB& operator +=(const Vector3f& inV);
	MinMaxAABB& operator -=(const Vector3f& inV);

	void Init ();

	const Vector3f& GetMin () const 				{ return m_Min; }
	const Vector3f& GetMax () const				{ return m_Max; }
	Vector3f GetCenter () const				{ return 0.5F * (m_Max + m_Min); }
	Vector3f GetExtent () const				{ return 0.5F * (m_Max - m_Min); }
	Vector3f GetSize () const				{ return (m_Max - m_Min); }
	
	void Encapsulate (const Vector3f& inPoint);
	void Encapsulate (const AABB& aabb);
	void Encapsulate (const MinMaxAABB& other);
	
	void Expand (float inValue);
	void Expand (const Vector3f& inOffset);
		
	// TODO : rename - it has different meaning than AABB::IsValid
	bool IsValid () const;
	
	bool IsInside (const Vector3f& inPoint) const;
	
	void GetVertices( Vector3f outVertices[8] ) const;

private:
	
	void FromAABB (const AABB& inAABB);
};

inline void AABB::Expand (float inValue)
{
	m_Extent += Vector3f (inValue, inValue, inValue);
}

inline void AABB::FromMinMaxAABB (const MinMaxAABB& inAABB)
{
	m_Center = (inAABB.GetMax () + inAABB.GetMin ()) * 0.5F;
	m_Extent = (inAABB.GetMax () - inAABB.GetMin ()) * 0.5F;
}		

inline bool AABB::IsValid () const
{
	return IsFinite(m_Center) && IsFinite(m_Extent);
}

inline void MinMaxAABB::Encapsulate (const Vector3f& inPoint)
{
	m_Min = min (m_Min, inPoint);
	m_Max = max (m_Max, inPoint);
}

inline void MinMaxAABB::Encapsulate (const AABB& aabb)
{
	Encapsulate (aabb.GetCenter()+aabb.GetExtent());
	Encapsulate (aabb.GetCenter()-aabb.GetExtent());
}

inline void MinMaxAABB::Encapsulate (const MinMaxAABB& other)
{
	m_Min = min (m_Min, other.m_Min);
	m_Max = max (m_Max, other.m_Max);
}

inline void MinMaxAABB::Expand (float inValue)
{
	Vector3f offset = Vector3f (inValue, inValue, inValue);
	m_Min -= offset;
	m_Max += offset;
}

inline void MinMaxAABB::Expand (const Vector3f& inOffset)
{
	m_Min -= inOffset;
	m_Max += inOffset;
}

inline bool MinMaxAABB::IsValid () const
{
	return !(m_Min == Vector3f::infinityVec || m_Max == -Vector3f::infinityVec);
}

inline void MinMaxAABB::Init ()
{
	m_Min = Vector3f::infinityVec;
	m_Max = -Vector3f::infinityVec;
}
	
inline void MinMaxAABB::FromAABB (const AABB& inAABB)
{
	m_Min = inAABB.GetCenter () - inAABB.GetExtent ();
	m_Max = inAABB.GetCenter () + inAABB.GetExtent ();
}

bool IntersectsWithBox(const MinMaxAABB& lhs, const MinMaxAABB& rhs);
bool IntersectsWithLine(const MinMaxAABB& aabb, const Vector3f& a, const Vector3f& b);
bool IntersectsWithRay(const MinMaxAABB& aabb, const Vector3f& origin, const Vector3f& dir);
bool IntersectsWithSphere(const MinMaxAABB& aabb, const Vector3f& center, float r);
MinMaxAABB Slerp(const MinMaxAABB& from, const MinMaxAABB& to, float t);

QIMP_END_NAMESPACE

#endif//__MATH_AABB_H__
