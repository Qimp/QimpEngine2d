#include "Private.h"

QIMP_BEGIN_NAMESPACE

bool InsidePoly(vector<Vector2f> & points, vector<Vector2f> & poly)
{
	for (size_t i=0; i < points.size(); i++)
	{
		if (!InsidePoly(points[i], poly))
			return false;
	}
	return true;
}

bool InsidePoly(Vector2f & p, vector<Vector2f> & poly)
{
	float minX = poly[0].x;
	float maxX = poly[0].x;
	float minY = poly[0].y;
	float maxY = poly[0].y;
	for ( size_t i = 1 ; i < poly.size() ; i++ )
	{
		Vector2f& q = poly[i];
		minX = std::min( q.x, minX );
		maxX = std::max( q.x, maxX );
		minY = std::min( q.y, minY );
		maxY = std::max( q.y, maxY );
	}

	if ( p.x < minX || p.x > maxX || p.y < minY || p.y > maxY )
		return false;

	size_t i, j;
	bool c = false;
	size_t nvert = poly.size();

	for (i=0, j = nvert-1; i < nvert; j = i++)
	{
		if ( ((poly[i].y > p.y) != (poly[j].y > p.y)) &&
			(p.x < (poly[j].x - poly[i].x) *(p.y - poly[i].y) / (poly[j].y - poly[i].y) + poly[i].x) )
			c = !c;
	}
	return c;
}

bool InsidePolySegment(Vector2f & p, vector<Vector2f> & poly)
{
	float minX = poly[0].x;
	float maxX = poly[0].x;
	float minY = poly[0].y;
	float maxY = poly[0].y;
	for ( size_t i = 1 ; i < poly.size() ; i++ )
	{
		Vector2f& q = poly[i];
		minX = std::min( q.x, minX );
		maxX = std::max( q.x, maxX );
		minY = std::min( q.y, minY );
		maxY = std::max( q.y, maxY );
	}

	if ( p.x < minX || p.x > maxX || p.y < minY || p.y > maxY )
		return false;

	bool c = false;
	size_t nvert = poly.size();

	for (size_t i=0; i<nvert; i+=2)
	{
		if ( ((poly[i+1].y > p.y) != (poly[i].y > p.y)) &&
			(p.x < (poly[i].x - poly[i+1].x) *(p.y - poly[i+1].y) / (poly[i].y - poly[i+1].y) + poly[i+1].x) )
			c = !c;
	}
	return c;
}

int IsClockwisePoly(vector<Vector2f> & poly, size_t n)
{
	int i,j,k;
	int count = 0;
	int z;

	if (n < 3)
		return(0);

	for (i=0; i<(int)n ;i++) {
		j = (i + 1) % n;
		k = (i + 2) % n;
		z  = (int)((poly[j].x - poly[i].x) * (poly[k].y - poly[j].y));
		z -= (int)((poly[j].y - poly[i].y) * (poly[k].x - poly[j].x));
		if (z < 0)
			count--;
		else if (z > 0)
			count++;
	}

	if (count > 0)
		return(1);
	else if (count < 0)
		return(-1);
	else
		return(0);
}

//--------------------------------------------------
//	直线方程：a*x + b*y + c==0
bool Intersect2D::LineLine(const Vector2f& line1Start, const Vector2f& line1End, const Vector2f& line2Start, const Vector2f& line2End, Vector2f* intersection)
{
	float d = (line1Start.x - line1End.x) * (line2Start.y - line2End.y) - (line1Start.y - line1End.y) * (line2Start.x - line2End.x);

	if (fabs(d) < EPSILON)
		return false;

	if (!intersection)
		return true;

	//求两直线得交点坐标
	Vector2f hit; 
	float a1 = line1End.y - line1Start.y;
	float b1 = line1Start.x - line1End.x;
	float c1 = line1End.x * line1Start.y - line1Start.x * line1End.y;
	float a2 = line2End.y - line2Start.y;
	float b2 = line2Start.x - line2End.x;
	float c2 = line2End.x * line2Start.y - line2Start.x * line2End.y;
	//注意这里b1 = 0 
	if (fabs(b1) < EPSILON)
	{
		intersection->x = -c1 / a1;
		intersection->y = (-c2 - a2 * hit.x) / b2;
	}       
	else
	{
		intersection->x = (c1 * b2 - b1 * c2) / (b1 * a2 - b2 * a1);
		intersection->y = (-c1 - a1 * hit.x) / b1;
	}

	return true;
}

//--------------------------------------------------
bool Intersect2D::SegmentSegment(const Vector2f& line1Start, const Vector2f& line1End, const Vector2f& line2Start, const Vector2f& line2End, Vector2f* intersection)
{
	Vector2f diffLA, diffLB;
	float compareA, compareB;
	diffLA = line1End - line1Start;
	diffLB = line2End - line2Start;
	compareA = diffLA.x*line1Start.y - diffLA.y*line1Start.x;
	compareB = diffLB.x*line2Start.y - diffLB.y*line2Start.x;
	if (
		(
		( ( diffLA.x*line2Start.y - diffLA.y*line2Start.x ) < compareA ) ^
		( ( diffLA.x*line2End.y - diffLA.y*line2End.x ) < compareA )
		)
		&&
		(
		( ( diffLB.x*line1Start.y - diffLB.y*line1Start.x ) < compareB ) ^
		( ( diffLB.x*line1End.y - diffLB.y*line1End.x) < compareB )
		)
		)
	{
		if (intersection)
		{
			float lDetDivInv = 1 / ((diffLA.x*diffLB.y) - (diffLA.y*diffLB.x));
			intersection->x = -((diffLA.x*compareB) - (compareA*diffLB.x)) * lDetDivInv;
			intersection->y = -((diffLA.y*compareB) - (compareA*diffLB.y)) * lDetDivInv;
		}
		return true;
	}

	return false;
}


//--------------------------------------------------
Vector3f BezierPoint( Vector3f a, Vector3f b, Vector3f c, Vector3f d, float t){
	float tp = 1.0f - t;
	return a*tp*tp*tp + b*3*t*tp*tp + c*3*t*t*tp + d*t*t*t;
}

//--------------------------------------------------
Vector3f CurvePoint( Vector3f a, Vector3f b, Vector3f c, Vector3f d, float t){
	Vector3f pt;
	float t2 = t * t;
	float t3 = t2 * t;
	pt.x = 0.5f * ( ( 2.0f * b.x ) +
		( -a.x + c.x ) * t +
		( 2.0f * a.x - 5.0f * b.x + 4 * c.x - d.x ) * t2 +
		( -a.x + 3.0f * b.x - 3.0f * c.x + d.x ) * t3 );
	pt.y = 0.5f * ( ( 2.0f * b.y ) +
		( -a.y + c.y ) * t +
		( 2.0f * a.y - 5.0f * b.y + 4 * c.y - d.y ) * t2 +
		( -a.y + 3.0f * b.y - 3.0f * c.y + d.y ) * t3 );
	return pt;
}

Vector3f CatmullRomPoint(Vector3f a, Vector3f b, Vector3f c, Vector3f d, float t)
{
	float factor = 0.5f;
	Vector3f c0 = b;
	Vector3f c1 = (c - a) * factor;
	Vector3f c2 = (c - b) * 3.f - (d - b) * factor - (c - a)* 2.f * factor;
	Vector3f c3 = (c - b) * -2.f + (d - b) * factor + (c - a) * factor;

	Vector3f curvePoint = c3 * t * t * t + c2 * t * t + c1 * t + c0;

	return curvePoint;
}

//--------------------------------------------------
Vector3f BezierTangent( Vector3f a, Vector3f b, Vector3f c, Vector3f d, float t){
	return (d-a-c*3+b*3)*(t*t)*3 + (a+c-b*2)*t*6 - a*3+b*3;
}

//--------------------------------------------------
Vector3f CurveTangent( Vector3f a, Vector3f b, Vector3f c, Vector3f d, float t){
	Vector3f v0 = ( c - a )*0.5;
	Vector3f v1 = ( d - b )*0.5;
	return ( b*2 -c*2 + v0 + v1)*(3*t*t) + ( c*3 - b*3 - v1 - v0*2 )*( 2*t) + v0;

}

//--------------------------------------------------
static Vector3f ProjectPointLine(const Vector3f& point, const Vector3f& lineStart, const Vector3f& lineEnd)
{
	Vector3f relativePoint = point - lineStart;
	Vector3f lineDirection = lineEnd - lineStart;
	float length = Length(lineDirection);
	Vector3f normalizedLineDirection = lineDirection;
	if (length > Vector3f::epsilon)
		normalizedLineDirection /= length;

	float dot = Dot(normalizedLineDirection, relativePoint);
	//dot = Clamp(dot, 0.0f, length);

	return lineStart + normalizedLineDirection * dot;
}

//--------------------------------------------------
float Distance3D::PointToLine(const Vector3f& point, const Vector3f& lineStart, const Vector3f& lineEnd)
{
	return Length(ProjectPointLine(point, lineStart, lineEnd) - point);
}

float Distance3D::PointToLines(const Vector3f& point, size_t count, const Vector3f* inVertices)
{
	float dist = numeric_limits<float>::infinity();
	for (size_t i = 1; i < count; i++)
		dist = std::min(dist, Distance3D::PointToLine(point, inVertices[i - 1], inVertices[i]));
	return dist;
}

float Distance3D::LineToLine(const Vector3f& line0Start, const Vector3f& line0End, const Vector3f& line1Start, const Vector3f& line1End)
{
	Vector3f u(line0End - line0Start);
	Vector3f v(line1End - line1Start);
	Vector3f w(line0Start - line1Start);

	float    a = Dot(u,u);        // always >= 0
	float    b = Dot(u,v);
	float    c = Dot(v,v);        // always >= 0
	float    d = Dot(u,w);
	float    e = Dot(v,w);
	float    D = a*c - b*b;       // always >= 0
	float    sc, tc;

	// compute the line parameters of the two closest points
	if (D < EPSILON) {         // the lines are almost parallel
		sc = 0.0;
		tc = (b>c ? d/b : e/c);   // use the largest denominator
	}
	else {
		sc = (b*e - c*d) / D;
		tc = (a*e - b*d) / D;
	}

	// get the difference of the two closest points
	Vector3f dp = w + (sc * u) - (tc * v);  // = L1(sc) - L2(tc)

	return dp.Length();   // return the closest distance
}

// dist3D_Segment_to_Segment(): get the 3D minimum distance between 2 segments
//    Input:  two 3D line segments S1 and S2
//    Return: the shortest distance between S1 and S2
float Distance3D::SegmentToSegment(const Vector3f& line0Start, const Vector3f& line0End, const Vector3f& line1Start, const Vector3f& line1End)
{
	Vector3f u(line0End - line0Start);
	Vector3f v(line1End - line1Start);
	Vector3f w(line0Start - line1Start);
	float    a = Dot(u, u);         // always >= 0
	float    b = Dot(u, v);
	float    c = Dot(v, v);         // always >= 0
	float    d = Dot(u, w);
	float    e = Dot(v, w);
	float    D = a*c - b*b;        // always >= 0
	float    sc, sN, sD = D;       // sc = sN / sD, default sD = D >= 0
	float    tc, tN, tD = D;       // tc = tN / tD, default tD = D >= 0

	// compute the line parameters of the two closest points
	if (D < EPSILON) { // the lines are almost parallel
		sN = 0.0f;         // force using point P0 on segment S1
		sD = 1.0f;         // to prevent possible division by 0.0 later
		tN = e;
		tD = c;
	}
	else {                 // get the closest points on the infinite lines
		sN = (b*e - c*d);
		tN = (a*e - b*d);
		if (sN < 0.0f) {        // sc < 0 => the s=0 edge is visible
			sN = 0.0f;
			tN = e;
			tD = c;
		}
		else if (sN > sD) {  // sc > 1  => the s=1 edge is visible
			sN = sD;
			tN = e + b;
			tD = c;
		}
	}

	if (tN < 0.0f) {            // tc < 0 => the t=0 edge is visible
		tN = 0.0f;
		// recompute sc for this edge
		if (-d < 0.0f)
			sN = 0.0f;
		else if (-d > a)
			sN = sD;
		else {
			sN = -d;
			sD = a;
		}
	}
	else if (tN > tD) {      // tc > 1  => the t=1 edge is visible
		tN = tD;
		// recompute sc for this edge
		if ((-d + b) < 0.0f)
			sN = 0.0f;
		else if ((-d + b) > a)
			sN = sD;
		else {
			sN = (-d + b);
			sD = a;
		}
	}
	// finally do the division to get sc and tc
	sc = (abs(sN) < EPSILON ? 0.0f : sN / sD);
	tc = (abs(tN) < EPSILON ? 0.0f : tN / tD);

	// get the difference of the two closest points
	Vector3f   dP = w + (sc * u) - (tc * v);  // =  S1(sc) - S2(tc)

	return dP.Length();   // return the closest distance
}

bool Intersect3D::LinePlane(const Vector3f& lineStart, const Vector3f& lineEnd, const Plane& plane, Vector3f* intersection)
{
	Vector3f direct(lineEnd - lineStart);
	direct = Normalize(direct);

	float vdot = Dot(direct, plane.GetNormal());
	float ndot = -Dot(lineStart, plane.GetNormal()) - plane.D();

	// is line parallel to the plane? if so, even if the line is
	// at the plane it is not considered as intersection because
	// it would be impossible to determine the point of intersection
	if (CompareApproximately(vdot, 0.0f))
		return false;

	// the resulting intersection is behind the origin of the ray
	// if the result is negative ( enter < 0 )
	if (intersection)
		*intersection = lineStart + (ndot / vdot) * direct;

	return true;
}

bool Intersect3D::LineSphere(const Vector3f& lineStart, const Vector3f& lineEnd, const Vector3f& sphereCenter, float sphereRadius, Vector3f* intersection)
{
	Vector3f v0 = sphereCenter - lineStart;
	Vector3f dir(lineEnd - lineStart);

	dir = Normalize(dir);

	// Project v0 onto ray dir
	float d = Dot(v0, dir);

	// We get the point which is closest to the spheres centre
	Vector3f vP = lineStart + dir * d;

	// Now if the length between this point and the spheres center is less
	// than the radius we have a hit!
	float dist = (sphereCenter - vP).Length();
	if (dist > sphereRadius)
		return false;

	if (intersection == NULL)
		return true;

	float pythag = sqrtf(sphereRadius*sphereRadius - dist*dist);

	float rayToSphere = v0.Length();

	// Check if the ray start point is in front or behind the center
	if (Dot(v0, dir) > 0)
	{
		// Check if the ray origin is inside there sphere
		if (rayToSphere > sphereRadius)
		{
			// Ray origin is outside there sphere, so we work out the length
			// along the ray to our spheres surface hit point
			float hitLen = d - pythag;

			*intersection = lineStart + dir*hitLen;
		}
		else
		{
			// Our rays origin is inside the sphere, so our hit point is the 
			// exit point of the ray, so we know the distance from our vP to
			// the spheres surface, and hence we can work out the exit point.
			*intersection = vP + dir*(pythag);
		}
	}
	else
	{
		if (rayToSphere > sphereRadius)
		{
			// This is always tricky to see at first, but we can still have vP, but
			// our ray origin is outside our sphere and its heading away from our sphere
			return false;
		}
		else
		{
			// Our ray origin is inside the sphere, and we can now workout the exit
			// hit point
			*intersection = vP + dir*(pythag);
		}

	}
	return true;
}

bool Intersect3D::RaySphere(const Vector3f & rayStart, const Vector3f & rayDir, const Vector3f& sphereCenter, float sphereRadius, Vector3f * intersection /* = NULL */)
{
	Vector3f v0 = sphereCenter - rayStart;
	float radiusSquared = sphereRadius *sphereRadius;

	if (v0.LengthSqr() < radiusSquared)
	{
		//	射线源点在球里面，则肯定有交点
		if (intersection != NULL)
			*intersection = sphereCenter;
		return true;
	}

	Vector3f dir = Normalize(rayDir);

	float d = Dot(v0, dir);

	Vector3f vP = rayStart + dir * d;

	float dist = (sphereCenter - vP).LengthSqr();
	if (dist > radiusSquared)
		return false;
	
	float rayToSphere = v0.LengthSqr();

	if (Dot(v0, dir) > 0)
	{
		float pythag = sqrtf(radiusSquared - dist);
		float hitLen = d - pythag;

		if (intersection != NULL)
			*intersection = rayStart + dir*hitLen;
	}
	else
	{
		if (rayToSphere > radiusSquared)
		{
			return false;
		}
		else
		{
			float pythag = sqrtf(radiusSquared - dist);
			if (intersection != NULL)
				*intersection = vP + dir*(pythag);
		}
	}
	return true;
}

bool Intersect3D::SegmentSphere(const Vector3f& segmentStart, const Vector3f& segmentEnd, const Vector3f& sphereCenter, float sphereRadius, Vector3f* pIntersection /* = NULL */)
{
	Vector3f v0 = sphereCenter - segmentStart;
	Vector3f v1 = sphereCenter - segmentEnd;
	Vector3f tmp = segmentEnd - segmentStart;
	if (tmp.Length() < Vector3f::epsilon)
	{
		if (v0.Length() >= sphereRadius)
			return false;

		if (pIntersection != NULL)
			*pIntersection = segmentEnd;
		return true;
	}
	Vector3f dir(segmentEnd - segmentStart);

	dir = Normalize(dir);

	float d = Dot(v0, dir);

	Vector3f vP = segmentStart + dir * d;

	float dist = (sphereCenter - vP).LengthSqr();
	if (dist > sphereRadius * sphereRadius)
		return false;
	
	float pythag = sqrtf(sphereRadius*sphereRadius - dist);

	float rayToSphere = v0.Length();

	Vector3f retPos;
	if (Dot(v0, dir) > 0)
	{
		if (rayToSphere > sphereRadius)
		{
			float hitLen = d - pythag;

			retPos = segmentStart + dir*hitLen;
		}
		else
		{
			retPos = vP + dir*(pythag);
		}
	}
	else
	{
		if (rayToSphere > sphereRadius)
		{
			return false;
		}
		else
		{
			retPos = vP + dir*(pythag);
		}

	}
	if ((retPos.x < segmentStart.x && retPos.x < segmentEnd.x) || (retPos.x > segmentStart.x && retPos.x > segmentEnd.x)
		|| (retPos.y < segmentStart.y && retPos.y < segmentEnd.y) || (retPos.y > segmentStart.y && retPos.y > segmentEnd.y)
		|| (retPos.z < segmentStart.z && retPos.z < segmentEnd.z) || (retPos.z > segmentStart.z && retPos.z > segmentEnd.z))
	{
		//	垂直点不在线段上
		if (v0.LengthSqr() <= sphereRadius * sphereRadius)
		{
			if (pIntersection != NULL)
				*pIntersection = segmentStart;
			return true;
		}
		if (v1.LengthSqr() <= sphereRadius * sphereRadius)
		{
			if (pIntersection != NULL)
				*pIntersection = segmentEnd;
			return true;
		}
		return false;
	}
	if (pIntersection != NULL)
		*pIntersection = retPos;
	return true;
}

bool Intersect3D::RayTriangle(const Vector3f& rayStart, const Vector3f& rayDir, Vector3f& p0, Vector3f& p1, Vector3f& p2, float* retT, float* retU, float* retV)
{
	// Find vectors for two edges sharing vert0
	Vector3f edge1 = p1 - p0;
	Vector3f edge2 = p2 - p0;

	// Begin calculating determinant - also used to calculate U parameter
	Vector3f pvec = Cross(rayDir, edge2);

	// If determinant is near zero, ray lies in plane of triangle
	float det = Dot(edge1, pvec);

	Vector3f tvec;
	if (det > 0)
	{
		tvec = rayStart - p0;
	}
	else
	{
		tvec = p0 - rayStart;
		det = -det;
	}

	if (det < 0.0001f)
		return false;

	// Calculate U parameter and test bounds
	*retU = Dot(tvec, pvec);
	if (*retU < 0.0f || *retU > det)
		return false;

	// Prepare to test V parameter
	Vector3f qvec = Cross(tvec, edge1);

	// Calculate V parameter and test bounds
	*retV = Dot(rayDir, qvec);
	if (*retV < 0.0f || *retU + *retV > det)
		return false;

	// Calculate t, scale parameters, ray intersects triangle
	*retT = Dot(edge2, qvec);
	float fInvDet = 1.0f / det;
	*retT *= fInvDet;
	*retU *= fInvDet;
	*retV *= fInvDet;

	return true;
}

QIMP_END_NAMESPACE
