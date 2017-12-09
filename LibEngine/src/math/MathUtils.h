#ifndef __MATH_UTILS_H__
#define __MATH_UTILS_H__


QIMP_BEGIN_NAMESPACE

bool		InsidePolySegment(Vector2f & p, vector<Vector2f> & poly);
bool		InsidePoly(Vector2f & p, vector<Vector2f> & poly);
bool		InsidePoly(vector<Vector2f> & points, vector<Vector2f> & poly);

int			IsClockwisePoly(vector<Vector2f> & poly, size_t n);

//Vector2f	LineInter(Vector3f l1, Vector3f l2);
//bool 		LineSegmentIntersection(Vector3f line1Start, Vector3f line1End, Vector3f line2Start, Vector3f line2End, Vector3f & intersection);

Vector3f 	BezierPoint( Vector3f a, Vector3f b, Vector3f c, Vector3f d, float t);
Vector3f 	CurvePoint( Vector3f a, Vector3f b, Vector3f c, Vector3f d, float t);
Vector3f	CatmullRomPoint(Vector3f a, Vector3f b, Vector3f c, Vector3f d, float t);

Vector3f 	BezierTangent( Vector3f a, Vector3f b, Vector3f c, Vector3f d, float t);
Vector3f 	CurveTangent( Vector3f a, Vector3f b, Vector3f c, Vector3f d, float t);

template< class T >
T TriangleInterpolate(const T& T0, const T& T1, const T& T2, const T& T3, float x, float y)
{
	if (y > 1.0f - x)
	{
		x -= 1.0f;
		y -= 1.0f;
		return (T)((T3 - T1)*x + (T3 - T2)*y + T3);
	}
	else
	{
		return (T)((T2 - T0)*x + (T1 - T0)*y + T0);
	}
}

template< class Key, class DataType >
inline DataType GetMapValue(const std::map<Key, DataType>& DataMap, Key Pos)
{
	typename std::map< Key, DataType >::const_iterator next = DataMap.upper_bound(Pos);
	if (next == DataMap.begin())
		return next->second;
	if (next == DataMap.end())
		return (--next)->second;
	typename std::map< Key, DataType >::const_iterator pre = next;
	--pre;
	if (pre == DataMap.end())
		return next->second;

	if (pre->second == next->second)
		return pre->second;

	float weight = (next->first - (float)Pos) / (next->first - pre->first);
	return (DataType)(pre->second*weight + next->second*(1.0f - weight));
}

template< class Key>
inline Quaternionf GetMapValue(const std::map<Key, Quaternionf>& DataMap, Key Pos)
{
	typename std::map< Key, Quaternionf >::const_iterator next = DataMap.upper_bound(Pos);
	if (next == DataMap.begin())
		return next->second;
	if (next == DataMap.end())
		return (--next)->second;
	typename std::map< Key, Quaternionf >::const_iterator pre = next;
	--pre;
	if (pre == DataMap.end())
		return next->second;

	if (pre->second == next->second)
		return pre->second;


	float weight = ((float)Pos - pre->first) / (next->first - pre->first);
	return Slerp(pre->second, next->second, weight);// pre->second.Slerp(weight, next->second);
}

//float		DistancePointToLine(const Vector3f& point, const Vector3f& lineStart, const Vector3f& lineEnd);
//float		DistancePointPolyLine(const Vector3f& point, size_t count, const Vector3f* inVertices);
//float		DistanceLineToLine(const Vector3f& line1Start, const Vector3f& line1End, const Vector3f& line2Start, const Vector3f& line2End);
//float		DistanceSegmentToSegment(const Vector3f& line0Start, const Vector3f& line0End, const Vector3f& line1Start, const Vector3f& line1End);
//
//bool		LineIntersectSpere(const Vector3f& lineStart, const Vector3f& lineEnd, const Vector3f& sphereCenter, float sphereRadius, Vector3f* hit = NULL);

QIMP_END_NAMESPACE

#endif //__MATH_UTILS_H__
