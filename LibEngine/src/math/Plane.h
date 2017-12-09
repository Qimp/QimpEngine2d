#ifndef __MATH_PLANE_H__
#define __MATH_PLANE_H__

QIMP_BEGIN_NAMESPACE

class Plane
{
private:
	Vector3f					m_Normal;
	float						m_Distance;

public:
	const float&				A() const							{ return m_Normal.x; };
	const float&				B() const							{ return m_Normal.y; };
	const float&				C() const							{ return m_Normal.z; };
	const float&				D() const							{ return m_Distance; };
	float&						D()									{ return m_Distance; };
	const Vector3f&				GetNormal() const					{ return m_Normal; };

	Plane()															{ };
	Plane(float a, float b, float c, float d)						{ m_Normal.x = a; m_Normal.y = b; m_Normal.z = c; m_Distance = d; };

	Plane&						operator *= (float scale);
	bool						operator == (const Plane& p) const	{ return m_Normal == p.m_Normal && m_Distance == p.m_Distance; };
	bool						operator != (const Plane& p)const	{ return m_Normal != p.m_Normal || m_Distance != p.m_Distance; };

	void						SetInvalid()						{ m_Normal = Vector3f::zero; m_Distance = 0.0F; };
	void						SetABCD(const float a, const float b, const float c, const float d);		// Just sets the coefficients. Does NOT normalize!
	void						Set3Points(const Vector3f& a, const Vector3f& b, const Vector3f& c);
	void						Set3PointsUnnormalized(const Vector3f& a, const Vector3f& b, const Vector3f& c);
	void						SetNormalAndPosition(const Vector3f& inNormal, const Vector3f& inPoint);

	float						GetDistanceToPoint(const Vector3f& inPt) const;
	bool						GetSide(const Vector3f& inPt) const;
	bool						SameSide(const Vector3f& inPt0, const Vector3f& inPt1);
	Vector3f					LineHit(const Vector3f &v0, const Vector3f &v1) const;
	void						NormalizeUnsafe();
//	void						NormalizeRobust();
};

inline float Plane::GetDistanceToPoint(const Vector3f& inPt)const
{
	Assert(IsNormalized(m_Normal));
	return Dot(m_Normal, inPt) + m_Distance;
}

// Returns true if we are on the front side (same as: GetDistanceToPoint () > 0.0)
inline bool Plane::GetSide(const Vector3f& inPt) const
{
	return Dot(m_Normal, inPt) + m_Distance > 0.0F;
}

// Calculates the m_Normal from 3 points unnormalized
inline Vector3f CalcRawNormalFromTriangle(const Vector3f& a, const Vector3f& b, const Vector3f& c)
{
	return Cross(b - a, c - a);
}

inline Vector3f RobustNormalFromTriangle(const Vector3f& v0, const Vector3f& v1, const Vector3f& v2)
{
	Vector3f m_Normal = CalcRawNormalFromTriangle(v0, v1, v2);
	return NormalizeRobust(m_Normal);
}

inline void Plane::SetABCD(float a, float b, float c, float d)
{
	m_Normal.Set(a, b, c);
	m_Distance = d;
}

inline void Plane::Set3Points(const Vector3f& a, const Vector3f& b, const Vector3f& c)
{
	m_Normal = CalcRawNormalFromTriangle(a, b, c);
	m_Normal = Normalize(m_Normal);
	m_Distance = -Dot(m_Normal, a);
	AssertIf(!IsNormalized(m_Normal));
}

inline void Plane::Set3PointsUnnormalized(const Vector3f& a, const Vector3f& b, const Vector3f& c)
{
	m_Normal = CalcRawNormalFromTriangle(a, b, c);
	m_Distance = -Dot(m_Normal, a);
}

inline void Plane::SetNormalAndPosition(const Vector3f& inNormal, const Vector3f& inPoint)
{
	m_Normal = inNormal;
	AssertIf(!IsNormalized(m_Normal, 0.001f));
	m_Distance = -Dot(inNormal, inPoint);
}

inline bool Plane::SameSide(const Vector3f& inPt0, const Vector3f& inPt1)
{
	float d0 = GetDistanceToPoint(inPt0);
	float d1 = GetDistanceToPoint(inPt1);
	if (d0 > 0.0f && d1 > 0.0f)
		return true;
	else if (d0 <= 0.0f && d1 <= 0.0f)
		return true;
	else
		return false;
}

inline Plane& Plane::operator *= (float scale)
{
	m_Normal *= scale;
	m_Distance *= scale;
	return *this;
}

inline Vector3f Plane::LineHit(const Vector3f &v0, const Vector3f &v1) const
{
	Vector3f dir = v1 - v0;
	float d = GetDistanceToPoint(v0);
	return v0 - (d / Dot(m_Normal, dir)) * dir;
}

inline void Plane::NormalizeUnsafe()
{
	float invMag = 1.0f / Length(m_Normal);
	m_Normal *= invMag;
	m_Distance *= invMag;
}

// It uses NormalizeRobust(), so it handles zero and extremely small vectors,
// but can be slow. Another option would be to use plain normalize, but
// always remember to check for division by zero with zero vectors.
//inline void Plane::NormalizeRobust()
//{
//	float invMag;
//	m_Normal = NormalizeRobust(m_Normal, invMag);
//	m_Distance *= invMag;
//}


QIMP_END_NAMESPACE

#endif //__MATH_PLANE_H__

