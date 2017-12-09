#include "Private.h"

QIMP_BEGIN_NAMESPACE

const AABB AABB::zero = AABB(Vector3f::zero, Vector3f::zero);

MinMaxAABB& MinMaxAABB::operator *= (float s)
{
	m_Min *= s;
	m_Max *= s;
	return *this;
}

MinMaxAABB& MinMaxAABB::operator *= (const Vector3f& inV)
{
	m_Min.x *= inV.x;
	m_Min.y *= inV.y;
	m_Min.z *= inV.z;
	m_Max.x *= inV.x;
	m_Max.y *= inV.y;
	m_Max.z *= inV.z;
	return *this;
}

MinMaxAABB& MinMaxAABB::operator += (const Vector3f& inV)
{
	m_Min += inV;
	m_Max += inV;
	return *this;
}

MinMaxAABB& MinMaxAABB::operator -= (const Vector3f& inV)
{
	m_Min -= inV;
	m_Max -= inV;
	return *this;
}

void CalculateClosestPoint (const Vector3f& rkPoint, const AABB& rkBox, Vector3f& outPoint, float& outSqrDistance)
{
	// compute coordinates of point in box coordinate system
	Vector3f kClosest = rkPoint - rkBox.GetCenter();

	// project test point onto box
	float fSqrDistance = 0.0f;
	float fDelta;
	
	for (int i=0;i<3;i++)
	{
		if ( kClosest[i] < -rkBox.GetExtent (i) )
		{
			fDelta = kClosest[i] + rkBox.GetExtent (i);
			fSqrDistance += fDelta * fDelta;
			kClosest[i] = -rkBox.GetExtent (i);
		}
		else if ( kClosest[i] > rkBox.GetExtent(i) )
		{
			fDelta = kClosest[i] - rkBox.GetExtent (i);
			fSqrDistance += fDelta * fDelta;
			kClosest[i] = rkBox.GetExtent (i);
		}
	}
	
	// Inside
	if (fSqrDistance == 0.0F)
	{
		outPoint = rkPoint;
		outSqrDistance = 0.0F;
	}
	// Outside
	else
	{
		outPoint = kClosest + rkBox.GetCenter();
		outSqrDistance = fSqrDistance;
	}
}


// Sphere-AABB distance, Arvo's algorithm
float CalculateSqrDistance (const Vector3f& rkPoint, const AABB& rkBox)
{
	Vector3f closest = rkPoint - rkBox.GetCenter();
	float sqrDistance = 0.0f;
	
	for (int i = 0; i < 3; ++i)
	{
		float clos = closest[i];
		float ext = rkBox.GetExtent(i);
		if (clos < -ext)
		{
			float delta = clos + ext;
			sqrDistance += delta * delta;
			closest[i] = -ext;
		}
		else if (clos > ext)
		{
			float delta = clos - ext;
			sqrDistance += delta * delta;
			closest[i] = ext;
		}
	}

	return sqrDistance;
}

void AABB::GetVertices (Vector3f* outVertices) const
{
	outVertices[0] = m_Center + Vector3f (-m_Extent.x, -m_Extent.y, -m_Extent.z);
	outVertices[1] = m_Center + Vector3f (+m_Extent.x, -m_Extent.y, -m_Extent.z);
	outVertices[2] = m_Center + Vector3f (+m_Extent.x, +m_Extent.y, -m_Extent.z);
	outVertices[3] = m_Center + Vector3f (-m_Extent.x, +m_Extent.y, -m_Extent.z);

	outVertices[4] = m_Center + Vector3f (-m_Extent.x, -m_Extent.y, +m_Extent.z);
	outVertices[5] = m_Center + Vector3f (+m_Extent.x, -m_Extent.y, +m_Extent.z);
	outVertices[6] = m_Center + Vector3f (+m_Extent.x, +m_Extent.y, +m_Extent.z);
	outVertices[7] = m_Center + Vector3f (-m_Extent.x, +m_Extent.y, +m_Extent.z);
}

void MinMaxAABB::GetVertices( Vector3f outVertices[8] ) const
{
	//    5-----6
	//   /     /|
	//  1-----2 |
	//  | 4   | 7
	//  |     |/
	//  0-----3
	outVertices[0].Set( m_Min.x, m_Min.y, m_Min.z );
	outVertices[1].Set( m_Max.x, m_Min.y, m_Min.z );
	outVertices[2].Set( m_Max.x, m_Max.y, m_Min.z );
	outVertices[3].Set( m_Min.x, m_Max.y, m_Min.z );
	outVertices[4].Set( m_Min.x, m_Min.y, m_Max.z );
	outVertices[5].Set( m_Max.x, m_Min.y, m_Max.z );
	outVertices[6].Set( m_Max.x, m_Max.y, m_Max.z );
	outVertices[7].Set( m_Min.x, m_Max.y, m_Max.z );
}



bool AABB::IsInside (const Vector3f& inPoint) const
{
	if (inPoint[0] < m_Center[0] - m_Extent[0])
		return false;
	if (inPoint[0] > m_Center[0] + m_Extent[0])
		return false;

	if (inPoint[1] < m_Center[1] - m_Extent[1])
		return false;
	if (inPoint[1] > m_Center[1] + m_Extent[1])
		return false;

	if (inPoint[2] < m_Center[2] - m_Extent[2])
		return false;
	if (inPoint[2] > m_Center[2] + m_Extent[2])
		return false;
	
	return true;
}

void AABB::Encapsulate (const Vector3f& inPoint) {
	MinMaxAABB temp = *this;
	temp.Encapsulate (inPoint);
	FromMinMaxAABB (temp);
}

bool MinMaxAABB::IsInside (const Vector3f& inPoint) const
{
	if (inPoint[0] < m_Min[0])
		return false;
	if (inPoint[0] > m_Max[0])
		return false;

	if (inPoint[1] < m_Min[1])
		return false;
	if (inPoint[1] > m_Max[1])
		return false;

	if (inPoint[2] < m_Min[2])
		return false;
	if (inPoint[2] > m_Max[2])
		return false;
	
	return true;
}

MinMaxAABB AddAABB (const MinMaxAABB& lhs, const MinMaxAABB& rhs)
{
	MinMaxAABB minMax;
	if (lhs.IsValid())
		minMax = lhs;
	
	if (rhs.IsValid())
	{
		minMax.Encapsulate (rhs.GetMax ());
		minMax.Encapsulate (rhs.GetMin ());
	}

	return minMax;
}

inline Vector3f RotateExtents (const Vector3f& extents, const Matrix3x3f& rotation)
{
	Vector3f newExtents;
	for (int i=0;i<3;i++)
		newExtents[i] = Abs (rotation.Get (i, 0) * extents.x) + Abs (rotation.Get (i, 1) * extents.y) + Abs (rotation.Get (i, 2) * extents.z);
	return newExtents;	
}

inline Vector3f RotateExtents (const Vector3f& extents, const Matrix4x4f& rotation)
{
	Vector3f newExtents;
	for (int i=0;i<3;i++)
		newExtents[i] = Abs (rotation.Get (i, 0) * extents.x) + Abs (rotation.Get (i, 1) * extents.y) + Abs (rotation.Get (i, 2) * extents.z);
	return newExtents;	
}

void TransformAABB (const AABB& aabb, const Vector3f& position, const Quaternionf& rotation, AABB& result)
{
	Matrix3x3f m;
	QuaternionToMatrix (rotation, m);
	
	Vector3f extents = RotateExtents (aabb.GetExtent (), m);
	Vector3f center = m.MultiplyPoint3 (aabb.GetCenter ());
	center += position;
	result.SetCenterAndExtent( center, extents );
}

void TransformAABB (const AABB& aabb, const Matrix4x4f& transform, AABB& result)
{
	Vector3f extents = RotateExtents (aabb.GetExtent (), transform);
	Vector3f center = transform.MultiplyPoint3 (aabb.GetCenter ());
	result.SetCenterAndExtent( center, extents );
}

void TransformAABBSlow (const AABB& aabb, const Matrix4x4f& transform, AABB& result)
{
	MinMaxAABB transformed;
	transformed.Init ();
	
	Vector3f v[8];
	aabb.GetVertices (v);
	for (int i=0;i<8;i++)
		transformed.Encapsulate (transform.MultiplyPoint3 (v[i]));

	result = transformed;
}


void InverseTransformAABB (const AABB& aabb, const Vector3f& position, const Quaternionf& rotation, AABB& result)
{
	Matrix3x3f m;
	QuaternionToMatrix (Inverse (rotation), m);

	Vector3f extents = RotateExtents (aabb.GetExtent (), m);
	Vector3f center = aabb.GetCenter () - position;
	center = m.MultiplyPoint3 (center);
	
	result.SetCenterAndExtent( center, extents );
}

bool IsContainedInAABB (const AABB& inside, const AABB& bigBounds)
{
	bool outside = false;
	outside |= inside.m_Center[0] - inside.m_Extent[0] < bigBounds.m_Center[0] - bigBounds.m_Extent[0];
	outside |= inside.m_Center[0] + inside.m_Extent[0] > bigBounds.m_Center[0] + bigBounds.m_Extent[0];

	outside |= inside.m_Center[1] - inside.m_Extent[1] < bigBounds.m_Center[1] - bigBounds.m_Extent[1];
	outside |= inside.m_Center[1] + inside.m_Extent[1] > bigBounds.m_Center[1] + bigBounds.m_Extent[1];

	outside |= inside.m_Center[2] - inside.m_Extent[2] < bigBounds.m_Center[2] - bigBounds.m_Extent[2];
	outside |= inside.m_Center[2] + inside.m_Extent[2] > bigBounds.m_Center[2] + bigBounds.m_Extent[2];

	return !outside;
}

bool IntersectsWithBox(const MinMaxAABB& lhs, const MinMaxAABB& rhs)
{
	return (lhs.m_Min.x <= rhs.m_Max.x && lhs.m_Max.x >= rhs.m_Min.x
		&& lhs.m_Min.y <= rhs.m_Max.y && lhs.m_Max.y >= rhs.m_Min.y
		&& lhs.m_Min.z <= rhs.m_Max.z && lhs.m_Max.z >= rhs.m_Min.z);
}

bool IntersectsWithLine(const MinMaxAABB& aabb, const Vector3f& a, const Vector3f& b)
{
	Vector3f linemiddle = (a + b) / 2;
	Vector3f linevect = a - b;
	float halflength = linevect.Length() / 2;
	linevect = Normalize(linevect);
	const Vector3f e = aabb.GetExtent();
	const Vector3f t = aabb.GetCenter() - linemiddle;

	if ((fabs(t.x) > e.x + halflength * fabs(linevect.x)) ||
		(fabs(t.y) > e.y + halflength * fabs(linevect.y)) ||
		(fabs(t.z) > e.z + halflength * fabs(linevect.z)))
		return false;

	float r = e.y * (float)fabs(linevect.z) + e.z * (float)fabs(linevect.y);
	if (fabs(t.y*linevect.z - t.z*linevect.y) > r)
		return false;

	r = e.x * (float)fabs(linevect.z) + e.z * (float)fabs(linevect.x);
	if (fabs(t.z*linevect.x - t.x*linevect.z) > r)
		return false;

	r = e.x * (float)fabs(linevect.y) + e.y * (float)fabs(linevect.x);
	if (fabs(t.x*linevect.y - t.y*linevect.x) > r)
		return false;

	return true;
}

bool IntersectsWithRay(const MinMaxAABB& aabb, const Vector3f& origin, const Vector3f& dir)
{
	//如果原点在盒子内部 则相交
	if (aabb.IsInside(origin))
	{
		return true;
	}

	float xh, yh, zh, t;

	//根据原点的位置依次检测盒子的六个面 求射线跟面的交点
	//再判断交点是否在对应的盒子面内部
	if (origin.x < aabb.m_Min.x && dir.x > 0.0f)
	{
		t = (aabb.m_Min.x - origin.x) / dir.x;

		yh = origin.y + t * dir.y;
		zh = origin.z + t * dir.z;

		if (yh < aabb.m_Max.y && yh > aabb.m_Min.y && zh < aabb.m_Max.z && zh > aabb.m_Min.z)
		{
			return true;
		}
	}

	if (origin.x > aabb.m_Max.x && dir.x < 0.0f)
	{
		t = (aabb.m_Max.x - origin.x) / dir.x;

		yh = origin.y + t * dir.y;
		zh = origin.z + t * dir.z;

		if (yh < aabb.m_Max.y && yh > aabb.m_Min.y && zh < aabb.m_Max.z && zh > aabb.m_Min.z)
		{
			return true;
		}
	}

	if (origin.y < aabb.m_Min.y && dir.y > 0.0f)
	{
		t = (aabb.m_Min.y - origin.y) / dir.y;

		xh = origin.x + t * dir.x;
		zh = origin.z + t * dir.z;

		if (xh < aabb.m_Max.x && xh > aabb.m_Min.x && zh < aabb.m_Max.z && zh > aabb.m_Min.z)
		{
			return true;
		}
	}

	if (origin.y > aabb.m_Max.y && dir.y < 0.0f)
	{
		t = (aabb.m_Max.y - origin.y) / dir.y;

		xh = origin.x + t * dir.x;
		zh = origin.z + t * dir.z;

		if (xh < aabb.m_Max.x && xh > aabb.m_Min.x && zh < aabb.m_Max.z && zh > aabb.m_Min.z)
		{
			return true;
		}
	}

	if (origin.z < aabb.m_Min.z && dir.z > 0.0f)
	{
		t = (aabb.m_Min.z - origin.z) / dir.z;

		xh = origin.x + t * dir.x;
		yh = origin.y + t * dir.y;

		if (xh < aabb.m_Max.x && xh > aabb.m_Min.x && yh < aabb.m_Max.y && yh > aabb.m_Min.y)
		{
			return true;
		}
	}

	if (origin.z > aabb.m_Max.z && dir.z < 0.0f)
	{
		t = (aabb.m_Max.z - origin.z) / dir.z;

		xh = origin.x + t * dir.x;
		yh = origin.y + t * dir.y;

		if (xh < aabb.m_Max.x && xh > aabb.m_Min.x && yh < aabb.m_Max.y && yh > aabb.m_Min.y)
		{
			return true;
		}
	}

	return false;
}

bool IntersectsWithSphere(const MinMaxAABB& aabb, const Vector3f& center, float r)
{
	Vector3f p;
	if (center.x < aabb.m_Min.x)
	{
		p.x = aabb.m_Min.x;
	}
	else if (center.x > aabb.m_Max.x)
	{
		p.x = aabb.m_Max.x;
	}
	else
	{
		p.x = center.x;
	}

	if (center.y < aabb.m_Min.y)
	{
		p.y = aabb.m_Min.y;
	}
	else if (center.y > aabb.m_Max.y)
	{
		p.y = aabb.m_Max.y;
	}
	else
	{
		p.y = center.y;
	}

	if (center.z < aabb.m_Min.z)
	{
		p.z = aabb.m_Min.z;
	}
	else if (center.z > aabb.m_Max.z)
	{
		p.z = aabb.m_Max.z;
	}
	else
	{
		p.z = center.z;
	}

	if (fabs((p - center).Length()) < fabs(r))
	{
		return true;
	}
	else
	{
		return false;
	}
}

MinMaxAABB Slerp(const MinMaxAABB& from, const MinMaxAABB& to, float t)
{
	MinMaxAABB result;
	result.m_Max = Slerp(from.m_Max, to.m_Max, t);
	result.m_Min = Slerp(from.m_Min, to.m_Min, t);
	return result;
}

QIMP_END_NAMESPACE