#ifndef __MATH_INTERSECT_H__
#define __MATH_INTERSECT_H__


QIMP_BEGIN_NAMESPACE

class Intersect2D
{
public:
	static bool		LineLine(const Vector2f& line1Start, const Vector2f& line1End, const Vector2f& line2Start, const Vector2f& line2End, Vector2f* intersection = NULL);
	static bool		SegmentSegment(const Vector2f& line1Start, const Vector2f& line1End, const Vector2f& line2Start, const Vector2f& line2End, Vector2f* intersection = NULL);

};

class BoundingBox;
class Intersect3D
{
public:
	static bool		LinePlane(const Vector3f& lineStart, const Vector3f& lineEnd, const Plane& plane, Vector3f* intersection = NULL);
	static bool		LineSphere(const Vector3f& lineStart, const Vector3f& lineEnd, const Vector3f& sphereCenter, float sphereRadius, Vector3f* intersection = NULL);
	static bool		RaySphere(const Vector3f& rayStart, const Vector3f& rayDir, const Vector3f& sphereCenter, float sphereRadius, Vector3f * intersection = NULL);
	static bool		RayTriangle(const Vector3f& rayStart, const Vector3f& rayDir, Vector3f& p0, Vector3f& p1, Vector3f& p2, float* retT, float* retU, float* retV);
	static bool		SegmentSphere(const Vector3f& segmentStart, const Vector3f& segmentEnd, const Vector3f& sphereCenter, float sphereRadius, Vector3f* pIntersection = NULL);
};

QIMP_END_NAMESPACE

#endif //__MATH_INTERSECT_H__
