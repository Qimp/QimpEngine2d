#include "Private.h"


QIMP_BEGIN_NAMESPACE
	//----------------------------------------------------------
	Polyline::Polyline()
	{
		SetRightVector();
		Clear();
	}

	//----------------------------------------------------------
	Polyline::Polyline(const vector<Vector3f>& verts)
	{
		SetRightVector();
		Clear();
		AddVertices(verts);
	}

	//----------------------------------------------------------
	Polyline Polyline::FromRectangle(const Rectf& rect)
	{
		Polyline polyline;
		polyline.AddVertex(rect.GetLeft(), rect.GetTop());
		polyline.AddVertex(rect.GetRight(), rect.GetTop());
		polyline.AddVertex(rect.GetRight(), rect.GetBottom());
		polyline.AddVertex(rect.GetLeft(), rect.GetBottom());
		polyline.Close();
		return polyline;
	}

	//----------------------------------------------------------
	void Polyline::Clear()
	{
		SetClosed(false);
		m_Points.clear();
		m_CurveVertices.clear();
		FlagHasChanged();
	}

	//----------------------------------------------------------
	void Polyline::AddVertex(const Vector3f& p)
	{
		m_CurveVertices.clear();
		m_Points.push_back(p);
		FlagHasChanged();
	}

	//----------------------------------------------------------
	void Polyline::AddVertex( float x, float y, float z/* =0 */ )
	{
		m_CurveVertices.clear();
		AddVertex(Vector3f(x,y,z));
		FlagHasChanged();
	}

	//----------------------------------------------------------
	void Polyline::AddVertices(const vector<Vector3f>& verts)
	{
		m_CurveVertices.clear();
		m_Points.insert( m_Points.end(), verts.begin(), verts.end() );
		FlagHasChanged();
	}

	//----------------------------------------------------------
	void Polyline::AddVertices(const Vector3f* verts, int numverts)
	{
		m_CurveVertices.clear();
		m_Points.insert( m_Points.end(), verts, verts + numverts );
		FlagHasChanged();
	}

	//----------------------------------------------------------
	void Polyline::InsertVertex(const Vector3f &p, int index)
	{
		m_CurveVertices.clear();
		m_Points.insert(m_Points.begin()+index, p);
		FlagHasChanged();
	}

	//----------------------------------------------------------
	void Polyline::InsertVertex(float x, float y, float z, int index)
	{
		InsertVertex(Vector3f(x, y, z), index);
	}


	//----------------------------------------------------------
	size_t Polyline::Size() const
	{
		return m_Points.size();
	}

	//----------------------------------------------------------
	const Vector3f& Polyline::operator[] (int index) const
	{
		return m_Points[index];
	}

	//----------------------------------------------------------
	Vector3f& Polyline::operator[] (int index)
	{
		FlagHasChanged();
		return m_Points[index];
	}

	//----------------------------------------------------------
	void Polyline::Resize(size_t size)
	{
		m_Points.resize(size);
		FlagHasChanged();
	}

	//----------------------------------------------------------
	void Polyline::SetClosed( bool tf )
	{
		m_bClosed = tf;
		FlagHasChanged();
	}

	//----------------------------------------------------------
	bool Polyline::IsClosed() const
	{
		return m_bClosed;
	}

	//----------------------------------------------------------
	void Polyline::Close()
	{
		SetClosed(true);
	}

	//----------------------------------------------------------
	bool Polyline::HasChanged()
	{
		if (m_bHasChanged)
		{
			m_bHasChanged=false;
			return true;
		}
		else
		{
			return false;
		}
	}

	//----------------------------------------------------------
	void Polyline::FlagHasChanged()
	{
		m_bHasChanged = true;
		m_bCacheIsDirty = true;
	}

	//----------------------------------------------------------
	vector<Vector3f> & Polyline::GetVertices()
	{
		FlagHasChanged();
		return m_Points;
	}

	//----------------------------------------------------------
	const vector<Vector3f> & Polyline::GetVertices() const
	{
		return m_Points;
	}


	//----------------------------------------------------------
	void Polyline::_SetCircleResolution(int res)
	{
		if (res > 1 && res != (int)m_CirclePoints.size())
		{
			m_CirclePoints.resize(res);

			float angle = 0.0f;
			const float angleAdder = (float)TWO_PI / (float)res;
			for (int i = 0; i < res; i++)
			{
				m_CirclePoints[i].x = cos(angle);
				m_CirclePoints[i].y = sin(angle);
				m_CirclePoints[i].z = 0.0f;
				angle += angleAdder;
			}
		}
	}

	//----------------------------------------------------------
	// wraps any radian angle -FLT_MAX to +FLT_MAX into 0->2PI range.
	// TODO, make angle treatment consistent across all functions
	// should always be radians?  or should this take degrees?
	// used internally, so perhaps not as important
	float Polyline::_WrapAngle(float angleRadians)
	{
		return Wrap(angleRadians, 0.0f, (float)TWO_PI);
	}

	//----------------------------------------------------------
	void Polyline::BezierTo( const Vector3f & cp1, const Vector3f & cp2, const Vector3f & to, int curveResolution )
	{
		// if, and only if poly vertices has points, we can make a bezier
		// from the last point
		m_CurveVertices.clear();

		// the resolultion with which we computer this bezier
		// is arbitrary, can we possibly make it dynamic?

		if (Size() > 0)
		{
			float x0 = m_Points[Size()-1].x;
			float y0 = m_Points[Size()-1].y;
			float z0 = m_Points[Size()-1].z;

			float   ax, bx, cx;
			float   ay, by, cy;
			float   az, bz, cz;
			float   t, t2, t3;
			float   x, y, z;

			// polynomial coefficients
			cx = 3.0f * (cp1.x - x0);
			bx = 3.0f * (cp2.x - cp1.x) - cx;
			ax = to.x - x0 - cx - bx;

			cy = 3.0f * (cp1.y - y0);
			by = 3.0f * (cp2.y - cp1.y) - cy;
			ay = to.y - y0 - cy - by;

			cz = 3.0f * (cp1.z - z0);
			bz = 3.0f * (cp2.z - cp1.z) - cz;
			az = to.z - z0 - cz - bz;

			for (int i = 0; i < curveResolution; i++)
			{
				t 	=  (float)i / (float)(curveResolution-1);
				t2 = t * t;
				t3 = t2 * t;
				x = (ax * t3) + (bx * t2) + (cx * t) + x0;
				y = (ay * t3) + (by * t2) + (cy * t) + y0;
				z = (az * t3) + (bz * t2) + (cz * t) + z0;
				m_Points.push_back(Vector3f(x,y,z));
			}
		}
		FlagHasChanged();
	}

	//----------------------------------------------------------
	void Polyline::QuadBezierTo(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, int curveResolution)
	{
		m_CurveVertices.clear();
		for(int i=0; i <= curveResolution; i++)
		{
			double t = (double)i / (double)(curveResolution);
			double a = (1.0 - t)*(1.0 - t);
			double b = 2.0 * t * (1.0 - t);
			double c = t*t;
			double x = a * x1 + b * x2 + c * x3;
			double y = a * y1 + b * y2 + c * y3;
			double z = a * z1 + b * z2 + c * z3;
			m_Points.push_back(Vector3f((float)x, (float)y, (float)z));
		}
		FlagHasChanged();
	}

	//----------------------------------------------------------
	void Polyline::CurveTo( const Vector3f & to, int curveResolution )
	{

		m_CurveVertices.push_back(to);

		if (m_CurveVertices.size() == 4)
		{

			float x0 = m_CurveVertices[0].x;
			float y0 = m_CurveVertices[0].y;
			float z0 = m_CurveVertices[0].z;
			float x1 = m_CurveVertices[1].x;
			float y1 = m_CurveVertices[1].y;
			float z1 = m_CurveVertices[1].z;
			float x2 = m_CurveVertices[2].x;
			float y2 = m_CurveVertices[2].y;
			float z2 = m_CurveVertices[2].z;
			float x3 = m_CurveVertices[3].x;
			float y3 = m_CurveVertices[3].y;
			float z3 = m_CurveVertices[3].z;

			float t,t2,t3;
			float x,y,z;

			for (int i = 0; i < curveResolution; i++)
			{

				t 	=  (float)i / (float)(curveResolution-1);
				t2 	= t * t;
				t3 	= t2 * t;

				x = 0.5f * ( ( 2.0f * x1 ) +
					( -x0 + x2 ) * t +
					( 2.0f * x0 - 5.0f * x1 + 4 * x2 - x3 ) * t2 +
					( -x0 + 3.0f * x1 - 3.0f * x2 + x3 ) * t3 );

				y = 0.5f * ( ( 2.0f * y1 ) +
					( -y0 + y2 ) * t +
					( 2.0f * y0 - 5.0f * y1 + 4 * y2 - y3 ) * t2 +
					( -y0 + 3.0f * y1 - 3.0f * y2 + y3 ) * t3 );

				z = 0.5f * ( ( 2.0f * z1 ) +
					( -z0 + z2 ) * t +
					( 2.0f * z0 - 5.0f * z1 + 4 * z2 - z3 ) * t2 +
					( -z0 + 3.0f * z1 - 3.0f * z2 + z3 ) * t3 );

				m_Points.push_back(Vector3f(x,y,z));
			}
			m_CurveVertices.pop_front();
		}
		FlagHasChanged();
	}

	//----------------------------------------------------------
	void Polyline::Arc(const Vector3f & center, float radiusX, float radiusY, float angleBegin, float angleEnd, bool clockwise, int circleResolution)
	{

		if (circleResolution<=1) circleResolution=2;
		_SetCircleResolution(circleResolution);
		m_Points.reserve(m_Points.size()+circleResolution);

		const float epsilon = 0.0001f;

		const size_t nCirclePoints = m_CirclePoints.size();
		float segmentArcSize  = (float)TWO_PI / (float)nCirclePoints;

		// convert angles to radians and wrap them into the range 0-M_TWO_PI and
		float angleBeginRad = _WrapAngle(DegToRad(angleBegin));
		float angleEndRad =   _WrapAngle(DegToRad(angleEnd));

		while(angleBeginRad >= angleEndRad) angleEndRad += (float)TWO_PI;

		// determine the directional angle delta
		float d = clockwise ? angleEndRad - angleBeginRad : angleBeginRad - angleEndRad;
		// find the shortest angle delta, clockwise delta direction yeilds POSITIVE values
		float deltaAngle = atan2(sin(d),cos(d));

		// establish the remaining angle that we have to work through
		float remainingAngle = deltaAngle;

		// if the delta angle is in the CCW direction OR the start and stop angles are
		// effectively the same adjust the remaining angle to be a be a full rotation
		if(deltaAngle < 0 || abs(deltaAngle) < epsilon) remainingAngle += (float)TWO_PI;

		Vector3f radii(radiusX,radiusY);
		Vector3f point;

		int currentLUTIndex = 0;
		bool isFirstPoint = true; // special case for the first point

		while(remainingAngle > 0) 
		{
			if(isFirstPoint) 
			{
				// TODO: should this be the exact point on the circle or
				// should it be an intersecting point on the line that connects two
				// surrounding LUT points?
				//
				// get the EXACT first point requested (for points that
				// don't fall precisely on a LUT entry)
				point = Vector3f(cos(angleBeginRad),sin(angleBeginRad));
				// set up the get any in between points from the LUT
				float ratio = angleBeginRad / (float)TWO_PI * (float)nCirclePoints;
				currentLUTIndex = clockwise ? (int)ceil(ratio) : (int)floor(ratio);
				float lutAngleAtIndex = currentLUTIndex * segmentArcSize;
				// the angle between the beginning angle and the next angle in the LUT table
				float d = clockwise ? (lutAngleAtIndex - angleBeginRad) : (angleBeginRad - lutAngleAtIndex);
				float firstPointDelta = atan2(sin(d),cos(d)); // negative is in the clockwise direction

				// if the are "equal", get the next one CCW
				if(abs(firstPointDelta) < epsilon) 
				{
					currentLUTIndex = clockwise ? (currentLUTIndex + 1) : (currentLUTIndex - 1);
					firstPointDelta = segmentArcSize; // we start at the next lut point
				}

				// start counting from the offset
				remainingAngle -= firstPointDelta;
				isFirstPoint = false;
			} 
			else
			{
				point = Vector3f(m_CirclePoints[currentLUTIndex].x, m_CirclePoints[currentLUTIndex].y);
				if(clockwise) 
				{
					currentLUTIndex++; // go to the next LUT point
					remainingAngle -= segmentArcSize; // account for next point
					// if the angle overshoots, then the while loop will fail next time
				}
				else
				{
					currentLUTIndex--; // go to the next LUT point
					remainingAngle -= segmentArcSize; // account for next point
					// if the angle overshoots, then the while loop will fail next time
				}
			}

			// keep the current lut index in range
			if(clockwise) 
			{
				currentLUTIndex = currentLUTIndex % nCirclePoints;
			}
			else 
			{
				if(currentLUTIndex < 0) currentLUTIndex = nCirclePoints + currentLUTIndex;
			}

			// add the point to the poly line
			point = Scale(point, radii) + center;
			m_Points.push_back(point);

			// if the next LUT point moves us past the end angle then
			// add a a point a the exact end angle and call it finished
			if(remainingAngle < epsilon) 
			{
				point = Vector3f(cos(angleEndRad),sin(angleEndRad));
				point = Scale(point, radii) + center;
				m_Points.push_back(point);
				remainingAngle = 0; // call it finished, the next while loop test will fail
			}
		}
		FlagHasChanged();
	}

	//----------------------------------------------------------
	float Polyline::GetPerimeter() const
	{
		if(m_Points.size() < 2) 
		{
			return 0;
		}
		else 
		{
			_UpdateCache();
			return m_Lengths.back();
		}
	}

	//----------------------------------------------------------
	float Polyline::GetArea() const
	{
		_UpdateCache();
		return m_Area;
	}

	//----------------------------------------------------------
	Vector3f Polyline::GetCentroid2D() const
	{
		_UpdateCache();
		return m_Centroid2D;
	}

	//----------------------------------------------------------
	Rectf Polyline::GetBoundingBox() const 
	{

		Rectf box;
		for(size_t i = 0; i < Size(); i++) 
		{
			if(i == 0) 
			{
				box.Set(m_Points[i].x, m_Points[i].y,0,0);
			}
			else 
			{
				box.Union(m_Points[i].x, m_Points[i].y);
			}
		}
		return box;
	}

	//----------------------------------------------------------
	Polyline Polyline::GetSmoothed(int smoothingSize, float smoothingShape) const 
	{
		int n = Size();
		smoothingSize = Clamp(smoothingSize, 0, n);
		smoothingShape = Clamp(smoothingShape, 0.0f, 1.0f);

		// precompute weights and normalization
		vector<float> weights;
		weights.resize(smoothingSize);
		// side weights
		for(int i = 1; i < smoothingSize; i++) 
		{
			float curWeight = (float)Map((float)i, 0.0f, (float)smoothingSize, 1.0f, (float)smoothingShape);
			weights[i] = curWeight;
		}

		// make a copy of this polyline
		Polyline result = *this;

		for(int i = 0; i < n; i++) 
		{
			float sum = 1; // center weight
			for(int j = 1; j < smoothingSize; j++) 
			{
				Vector3f cur;
				int leftPosition = i - j;
				int rightPosition = i + j;
				if(leftPosition < 0 && m_bClosed)
				{
					leftPosition += n;
				}
				if(leftPosition >= 0) 
				{
					cur += m_Points[leftPosition];
					sum += weights[j];
				}
				if(rightPosition >= n && m_bClosed) 
				{
					rightPosition -= n;
				}
				if(rightPosition < n) 
				{
					cur += m_Points[rightPosition];
					sum += weights[j];
				}
				result[i] += cur * weights[j];
			}
			result[i] /= sum;
		}

		return result;
	}

	//----------------------------------------------------------
	Polyline Polyline::GetResampledBySpacing(float spacing) const 
	{
		if(spacing==0 || Size() == 0) return *this;
		Polyline poly;
		float totalLength = GetPerimeter();
		for(float f=0; f<totalLength; f += spacing) 
		{
			poly.LineTo(GetPointAtLength(f));
		}

		if(!IsClosed()) 
		{
			if(poly.Size() > 0) poly.m_Points.back() = m_Points.back();
			poly.SetClosed(false);
		}
		else 
		{
			poly.SetClosed(true);
		}

		return poly;
	}

	//----------------------------------------------------------
	Polyline Polyline::GetResampledByCount(int count) const 
	{
		float perimeter = GetPerimeter();
		if(count < 2)
		{
			//		ofLogWarning("Polyline") << "getResampledByCount(): requested " << count <<" points, using minimum count of 2 ";
			count = 2;
		}
		return Polyline::GetResampledBySpacing(perimeter / (count-1));
	}

	//----------------------------------------------------------
	static Vector3f GetClosestPointUtil(const Vector3f& p1, const Vector3f& p2, const Vector3f& p3, float* normalizedPosition) 
	{
		// if p1 is coincident with p2, there is no line
		if(p1 == p2) 
		{
			if(normalizedPosition != NULL) 
			{
				*normalizedPosition = 0;
			}
			return p1;
		}

		float u = (p3.x - p1.x) * (p2.x - p1.x);
		u += (p3.y - p1.y) * (p2.y - p1.y);
		// perfect place for fast inverse sqrt...
		float len = (p2 - p1).Length();
		u /= (len * len);

		// clamp u
		if(u > 1) 
		{
			u = 1;
		}
		else if(u < 0) 
		{
			u = 0;
		}
		if(normalizedPosition != NULL) 
		{
			*normalizedPosition = u;
		}
		return Lerp(p1, p2, u);
	}

	//----------------------------------------------------------
	// a much faster but less accurate version would check distances to vertices first,
	// which assumes vertices are evenly spaced
	Vector3f Polyline::GetClosestPoint(const Vector3f& target, unsigned int* nearestIndex) const 
	{
		const Polyline & polyline = *this;

		if(polyline.Size() < 2) 
		{
			if(nearestIndex != NULL) 
			{
				nearestIndex = 0;
			}
			return target;
		}

		float distance = 0;
		Vector3f nearestPoint;
		unsigned int nearest = 0;
		float normalizedPosition = 0;
		unsigned int lastPosition = polyline.Size() - 1;
		if(polyline.IsClosed()) 
		{
			lastPosition++;
		}
		for(int i = 0; i < (int) lastPosition; i++) 
		{
			bool repeatNext = i == (int) (polyline.Size() - 1);

			const Vector3f& cur = polyline[i];
			const Vector3f& next = repeatNext ? polyline[0] : polyline[i + 1];

			float curNormalizedPosition = 0;
			Vector3f curNearestPoint = GetClosestPointUtil(cur, next, target, &curNormalizedPosition);
			float curDistance = Distance(curNearestPoint, target);
			if(i == 0 || curDistance < distance) 
			{
				distance = curDistance;
				nearest = i;
				nearestPoint = curNearestPoint;
				normalizedPosition = curNormalizedPosition;
			}
		}

		if(nearestIndex != NULL) 
		{
			if(normalizedPosition > .5) 
			{
				nearest++;
				if(nearest == polyline.Size()) 
				{
					nearest = 0;
				}
			}
			*nearestIndex = nearest;
		}

		return nearestPoint;
	}

	//--------------------------------------------------
	bool Polyline::Inside(const Vector3f & p, const Polyline & polyline){
		return Polyline::Inside(p.x,p.y,polyline);
	}

	//--------------------------------------------------
	bool Polyline::Inside(float x, float y, const Polyline & polyline){
		int counter = 0;
		int i;
		double xinters;
		Vector3f p1,p2;

		int N = polyline.Size();

		p1 = polyline[0];
		for (i=1;i<=N;i++)
		{
			p2 = polyline[i % N];
			if (y > std::min(p1.y, p2.y)) 
			{
				if (y <= std::max(p1.y,p2.y)) 
				{
					if (x <= std::max(p1.x,p2.x)) 
					{
						if (p1.y != p2.y) 
						{
							xinters = (y-p1.y)*(p2.x-p1.x)/(p2.y-p1.y)+p1.x;
							if (p1.x == p2.x || x <= xinters)
								counter++;
						}
					}
				}
			}
			p1 = p2;
		}

		if (counter % 2 == 0) return false;
		else return true;
	}

	//--------------------------------------------------
	bool Polyline::Inside(float x, float y) const 
	{
		return Polyline::Inside(x, y, *this);

	}

	//--------------------------------------------------
	bool Polyline::Inside(const Vector3f & p) const 
	{
		return Polyline::Inside(p, *this);
	}


	typedef struct
	{
		Vector3f P0;
		Vector3f P1;
	}Segment;

	// dot product (3D) which allows vector operations in arguments
#define dot(u,v)   ((u).x * (v).x + (u).y * (v).y + (u).z * (v).z)
#define norm2(v)   dot(v,v)        // norm2 = squared length of vector
#define norm(v)    sqrt(norm2(v))  // norm = length of vector
#define d2(u,v)    norm2(u-v)      // distance squared = norm2 of difference
#define d(u,v)     norm(u-v)       // distance = norm of difference

	//--------------------------------------------------
	static void SimplifyDP(float tol, Vector3f* v, int j, int k, int* mk )
	{
		if (k <= j+1) // there is nothing to simplify
			return;

		// check for adequate approximation by segment S from v[j] to v[k]
		int     maxi	= j;          // index of vertex farthest from S
		float   maxd2	= 0;         // distance squared of farthest vertex
		float   tol2	= tol * tol;  // tolerance squared
		Segment S		= {v[j], v[k]};  // segment from v[j] to v[k]
		Vector3f u;
		u				= S.P1 - S.P0;   // segment direction vector
		double  cu		= dot(u,u);     // segment length squared

		// test each vertex v[i] for max distance from S
		// compute using the Feb 2001 Algorithm's dist_ofxPoint_to_Segment()
		// Note: this works in any dimension (2D, 3D, ...)
		Vector3f  w;
		Vector3f   Pb;                // base of perpendicular from v[i] to S
		float  b, cw, dv2;        // dv2 = distance v[i] to S squared

		for (int i=j+1; i<k; i++)
		{
			// compute distance squared
			w = v[i] - S.P0;
			cw = dot(w,u);
			if ( cw <= 0 ) dv2 = d2(v[i], S.P0);
			else if ( cu <= cw ) dv2 = d2(v[i], S.P1);
			else 
			{
				b = (float)(cw / cu);
				Pb = S.P0 + u*b;
				dv2 = d2(v[i], Pb);
			}
			// test with current max distance squared
			if (dv2 <= maxd2) continue;

			// v[i] is a new max vertex
			maxi = i;
			maxd2 = dv2;
		}
		if (maxd2 > tol2)        // error is worse than the tolerance
		{
			// split the polyline at the farthest vertex from S
			mk[maxi] = 1;      // mark v[maxi] for the simplified polyline
			// recursively simplify the two subpolylines at v[maxi]
			SimplifyDP( tol, v, j, maxi, mk );  // polyline v[j] to v[maxi]
			SimplifyDP( tol, v, maxi, k, mk );  // polyline v[maxi] to v[k]
		}
		// else the approximation is OK, so ignore intermediate vertices
		return;
	}

	//--------------------------------------------------
	void Polyline::Simplify(float tol){
		if(m_Points.size() < 2) return;

		int n = Size();

		if(n == 0) {
			return;
		}

		vector <Vector3f> sV;
		sV.resize(n);

		int    i, k, m, pv;            // misc counters
		float  tol2 = tol * tol;       // tolerance squared
		vector<Vector3f> vt;
		vector<int> mk;
		vt.resize(n);
		mk.resize(n,0);


		// STAGE 1.  Vertex Reduction within tolerance of prior vertex cluster
		vt[0] = m_Points[0];              // start at the beginning
		for (i=k=1, pv=0; i<n; i++) 
		{
			if (d2(m_Points[i], m_Points[pv]) < tol2) continue;

			vt[k++] = m_Points[i];
			pv = i;
		}
		if (pv < n-1) vt[k++] = m_Points[n-1];      // finish at the end

		// STAGE 2.  Douglas-Peucker polyline simplification
		mk[0] = mk[k-1] = 1;       // mark the first and last vertices
		SimplifyDP( tol, &vt[0], 0, k-1, &mk[0] );

		// copy marked vertices to the output simplified polyline
		for (i=m=0; i<k; i++)
		{
			if (mk[i]) sV[m++] = vt[i];
		}

		//get rid of the unused points
		if( m < (int)sV.size() )
		{
			m_Points.assign( sV.begin(),sV.begin()+m );
		}else{
			m_Points = sV;
		}

	}

	//--------------------------------------------------
	void Polyline::Draw()
	{
		//ofGetCurrentRenderer()->draw(*this);
	}

	//--------------------------------------------------
	void Polyline::SetRightVector(Vector3f v) 
	{
		m_RightVector = v;
		FlagHasChanged();
	}

	//--------------------------------------------------
	Vector3f Polyline::GetRightVector() const
	{
		return m_RightVector;
	}


	//--------------------------------------------------
	float Polyline::GetIndexAtLength(float length) const 
	{
		if(m_Points.size() < 2) return 0;
		_UpdateCache();

		float totalLength = GetPerimeter();
		length = Clamp(length, 0.0f, totalLength);

		int lastPointIndex = IsClosed() ? m_Points.size() : m_Points.size()-1;

		int i1 = Clamp((int)floor(length / totalLength * lastPointIndex), 0, (int)m_Lengths.size()-2);   // start approximation here
		int leftLimit = 0;
		int rightLimit = lastPointIndex;

		float distAt1, distAt2;
		for(size_t iterations = 0; iterations < 32; iterations ++) 
		{	// limit iterations
			i1 = Clamp(i1, 0, (int)m_Lengths.size()-1);
			distAt1 = m_Lengths[i1];
			if(distAt1 <= length) 
			{         // if Length at i1 is less than desired Length (this is good)
				distAt2 = m_Lengths[i1+1];
				if(distAt2 > length)
				{
					float t = Map(length, distAt1, distAt2, 0, 1);
					return i1 + t;
				}
				else 
				{
					leftLimit = i1;
				}
			}
			else 
			{
				rightLimit = i1;
			}
			i1 = (leftLimit + rightLimit)/2;
		}
		return 0;
	}


	//--------------------------------------------------
	float Polyline::GetIndexAtPercent(float f) const 
	{
		return GetIndexAtLength(f * GetPerimeter());
	}

	//--------------------------------------------------
	float Polyline::GetLengthAtIndex(int index) const 
	{
		if (m_Points.size() < 2) return 0;
		_UpdateCache();
		return m_Lengths[GetWrappedIndex(index)];
	}

	//--------------------------------------------------
	float Polyline::GetLengthAtIndexInterpolated(float findex) const 
	{
		if (m_Points.size() < 2) return 0;
		_UpdateCache();
		int i1, i2;
		float t;
		_GetInterpolationParams(findex, i1, i2, t);
		return Lerp(GetLengthAtIndex(i1), GetLengthAtIndex(i2), t);
	}


	//--------------------------------------------------
	Vector3f Polyline::GetPointAtLength(float f) const
	{
		if(m_Points.size() < 2) return Vector3f();
		_UpdateCache();
		return GetPointAtIndexInterpolated(GetIndexAtLength(f));
	}

	//--------------------------------------------------
	Vector3f Polyline::GetPointAtPercent(float f) const 
	{
		float length = GetPerimeter();
		return GetPointAtLength(f * length);
	}


	//--------------------------------------------------
	Vector3f Polyline::GetPointAtIndexInterpolated(float findex) const 
	{
		if(m_Points.size() < 2) return Vector3f();
		int i1, i2;
		float t;
		_GetInterpolationParams(findex, i1, i2, t);
		Vector3f leftPoint(m_Points[i1]);
		Vector3f rightPoint(m_Points[i2]);
		return Lerp(leftPoint, rightPoint, t);
	}


	//--------------------------------------------------
	float Polyline::GetAngleAtIndex(int index) const 
	{
		if(m_Points.size() < 2) return 0;
		_UpdateCache();
		return m_Angles[GetWrappedIndex(index)];
	}

	//--------------------------------------------------
	float Polyline::GetAngleAtIndexInterpolated(float findex) const 
	{
		if(m_Points.size() < 2) return 0;
		int i1, i2;
		float t;
		_GetInterpolationParams(findex, i1, i2, t);
		return Lerp(GetAngleAtIndex(i1), GetAngleAtIndex(i2), t);
	}

	//--------------------------------------------------
	Vector3f Polyline::GetRotationAtIndex(int index) const 
	{
		if(m_Points.size() < 2) return Vector3f();
		_UpdateCache();
		return m_Rotations[GetWrappedIndex(index)];
	}

	//--------------------------------------------------
	Vector3f Polyline::GetRotationAtIndexInterpolated(float findex) const 
	{
		if(m_Points.size() < 2) return Vector3f();
		int i1, i2;
		float t;
		_GetInterpolationParams(findex, i1, i2, t);
		return Lerp(GetRotationAtIndex(i1), GetRotationAtIndex(i2), t);
	}

	//--------------------------------------------------
	Vector3f Polyline::GetTangentAtIndex(int index) const 
	{
		if(m_Points.size() < 2) return Vector3f();
		_UpdateCache();
		return m_Tangents[GetWrappedIndex(index)];
	}

	//--------------------------------------------------
	Vector3f Polyline::GetTangentAtIndexInterpolated(float findex) const 
	{
		if(m_Points.size() < 2) return Vector3f();
		int i1, i2;
		float t;
		_GetInterpolationParams(findex, i1, i2, t);
		return Lerp(GetTangentAtIndex(i1), GetTangentAtIndex(i2), t);
	}

	//--------------------------------------------------
	Vector3f Polyline::GetNormalAtIndex(int index) const 
	{
		if(m_Points.size() < 2) return Vector3f();
		_UpdateCache();
		return m_Normals[GetWrappedIndex(index)];
	}

	//--------------------------------------------------
	Vector3f Polyline::GetNormalAtIndexInterpolated(float findex) const 
	{
		if(m_Points.size() < 2) return Vector3f();
		int i1, i2;
		float t;
		_GetInterpolationParams(findex, i1, i2, t);
		return Lerp(GetNormalAtIndex(i1), GetNormalAtIndex(i2), t);
	}


	//--------------------------------------------------
	void Polyline::_CalcData(int index, Vector3f &tangent, float &angle, Vector3f &rotation, Vector3f &normal) const 
	{
		int i1 = GetWrappedIndex(index - 1);
		int i2 = GetWrappedIndex(index);
		int i3 = GetWrappedIndex(index + 1);

		Vector3f p1(m_Points[i1]);
		Vector3f p2(m_Points[i2]);
		Vector3f p3(m_Points[i3]);

		Vector3f v1(p1 - p2); // vector to previous point
		Vector3f v2(p3 - p2); // vector to next point
		v1 = Normalize(v1);
		v2 = Normalize(v2);

		tangent = (v2 - v1);
		tangent = Normalize(tangent);

		rotation = Cross(v1, v2);
		angle = 180.0f - RadToDeg(acos(Clamp(v1.x * v2.x + v1.y * v2.y + v1.z * v2.z, -1.0f, 1.0f)));

		normal = Cross(m_RightVector, tangent);
		normal = Normalize(normal);
	}


	//--------------------------------------------------
	int Polyline::GetWrappedIndex(int index) const 
	{
		if(m_Points.empty()) return 0;

		if(index < 0) return IsClosed() ? (index + m_Points.size()) % m_Points.size() : 0;
		if(index > int(m_Points.size())-1) return IsClosed() ? index % m_Points.size() : m_Points.size() - 1;
		return index;
	}

	//--------------------------------------------------
	void Polyline::_GetInterpolationParams(float findex, int &i1, int &i2, float &t) const 
	{
		i1 = (int)floor(findex);
		t = findex - i1;
		i1 = GetWrappedIndex(i1);
		i2 = GetWrappedIndex(i1 + 1);
	}

	//--------------------------------------------------
	void Polyline::_UpdateCache(bool bForceUpdate) const 
	{
		if (m_bCacheIsDirty || bForceUpdate) 
		{
			m_Lengths.clear();
			m_Angles.clear();
			m_Rotations.clear();
			m_Normals.clear();
			m_Tangents.clear();
			m_Area = 0;
			m_Centroid2D.Set(0, 0, 0);
			m_bCacheIsDirty = false;

			if (m_Points.size() < 2) return;

			// area
			for(int i=0;i<(int)m_Points.size()-1;i++)
			{
				m_Area += m_Points[i].x * m_Points[i+1].y - m_Points[i+1].x * m_Points[i].y;
			}
			m_Area += m_Points[m_Points.size()-1].x * m_Points[0].y - m_Points[0].x * m_Points[m_Points.size()-1].y;
			m_Area *= 0.5;


			// centroid
			// TODO: doesn't seem to work on all concave shapes
			for(int i=0;i<(int)m_Points.size()-1;i++)
			{
				m_Centroid2D.x += (m_Points[i].x + m_Points[i+1].x) * (m_Points[i].x*m_Points[i+1].y - m_Points[i+1].x*m_Points[i].y);
				m_Centroid2D.y += (m_Points[i].y + m_Points[i+1].y) * (m_Points[i].x*m_Points[i+1].y - m_Points[i+1].x*m_Points[i].y);
			}
			m_Centroid2D.x += (m_Points[m_Points.size()-1].x + m_Points[0].x) * (m_Points[m_Points.size()-1].x*m_Points[0].y - m_Points[0].x*m_Points[m_Points.size()-1].y);
			m_Centroid2D.y += (m_Points[m_Points.size()-1].y + m_Points[0].y) * (m_Points[m_Points.size()-1].x*m_Points[0].y - m_Points[0].x*m_Points[m_Points.size()-1].y);

			m_Centroid2D.x /= (6*m_Area);
			m_Centroid2D.y /= (6*m_Area);


			// per vertex cache
			m_Lengths.resize(m_Points.size());
			m_Tangents.resize(m_Points.size());
			m_Angles.resize(m_Points.size());
			m_Normals.resize(m_Points.size());
			m_Rotations.resize(m_Points.size());

			float angle;
			Vector3f rotation;
			Vector3f normal;
			Vector3f tangent;

			float length = 0;
			for(int i=0; i<(int)m_Points.size(); i++) 
			{
				m_Lengths[i] = length;

				_CalcData(i, tangent, angle, rotation, normal);
				m_Tangents[i] = tangent;
				m_Angles[i] = angle;
				m_Rotations[i] = rotation;
				m_Normals[i] = normal;

				length += Distance(m_Points[i], m_Points[GetWrappedIndex(i + 1)]);
			}

			if(IsClosed()) m_Lengths.push_back(length);
		}
	}
QIMP_END_NAMESPACE
