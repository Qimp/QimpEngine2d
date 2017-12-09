#ifndef __MATH_UTILS_H__
#define __MATH_UTILS_H__


QIMP_BEGIN_NAMESPACE

bool		InsidePolySegment(Vector2f & p, vector<Vector2f> & poly);
bool		InsidePoly(Vector2f & p, vector<Vector2f> & poly);
bool		InsidePoly(vector<Vector2f> & points, vector<Vector2f> & poly);

int			IsClockwisePoly(vector<Vector2f> & poly, size_t n);

Vector2f	LineInter(Vector3f l1, Vector3f l2);
bool 		LineSegmentIntersection(Vector3f line1Start, Vector3f line1End, Vector3f line2Start, Vector3f line2End, Vector3f & intersection);

Vector3f 	BezierPoint( Vector3f a, Vector3f b, Vector3f c, Vector3f d, float t);
Vector3f 	CurvePoint( Vector3f a, Vector3f b, Vector3f c, Vector3f d, float t);
Vector3f 	BezierTangent( Vector3f a, Vector3f b, Vector3f c, Vector3f d, float t);
Vector3f 	CurveTangent( Vector3f a, Vector3f b, Vector3f c, Vector3f d, float t);


QIMP_END_NAMESPACE

#endif //__MATH_UTILS_H__
