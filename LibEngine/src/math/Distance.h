#ifndef __MATH_DISTANCE_H__
#define __MATH_DISTANCE_H__


QIMP_BEGIN_NAMESPACE

struct Distance3D
{
	static float		PointToLine(const Vector3f& point, const Vector3f& lineStart, const Vector3f& lineEnd);
	static float		PointToLines(const Vector3f& point, size_t count, const Vector3f* inVertices);
	static float		LineToLine(const Vector3f& line1Start, const Vector3f& line1End, const Vector3f& line2Start, const Vector3f& line2End);
	static float		SegmentToSegment(const Vector3f& line0Start, const Vector3f& line0End, const Vector3f& line1Start, const Vector3f& line1End);
};

QIMP_END_NAMESPACE

#endif //__MATH_DISTANCE_H__
