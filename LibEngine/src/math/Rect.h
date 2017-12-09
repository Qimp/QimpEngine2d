#ifndef __MATH_RECT_H__
#define __MATH_RECT_H__

#include "FloatConversion.h"
#include "Vector2.h"

QIMP_BEGIN_NAMESPACE

class Rect
{
public:
	int	x;
	int	y;
	int	width; 
	int	height;

	Rect (int inX = 0, int inY = 0, int iWidth = 0, int iHeight = 0)
	{
		x = inX; width = iWidth;
		y = inY; height = iHeight;
	}
	int GetLeft() const { return x; };
	int GetTop() const { return y; };
	int GetRight() const { return x + width; }
	int GetBottom() const { return y + height; }
	void SetLeft(int l) { int oldXMax = GetXMax(); x = l; width = oldXMax - x; }
	void SetTop(int t) { int oldYMax = GetYMax(); y = t; height = oldYMax - y; }
	void SetRight(int r) { width = r - x; }
	void SetBottom(int b) { height = b - y; }


	int GetXMax() const { return x + width; }
	int GetYMax() const { return y + height; }

	void Union(int px, int py);
	void Union(const Rect& rect);

	/// Return true if rectangle is empty.
	inline bool IsEmpty () const
	{ return width <= 0 || height <= 0; }

	/// Sets the rectangle
	inline void Set(int inX, int inY, int iWidth, int iHeight) 
	{
		x = inX; width = iWidth;
		y = inY; height = iHeight;
	}

	void SetCenterPos (int x, int y);
	Vector2f GetCenterPos()const;

	/// Ensure this is inside the rect r.
	void Clamp (const Rect &r);

	/// Move rectangle by deltaX, deltaY.
	inline void Move (int dX, int dY)		{ x += dX; y += dY; }

	/// Return the width of rectangle.
	inline int Width () const					{ return width; }

	/// Return the height of rectangle.
	inline int Height () const					{ return height; }

	inline bool Contains (const Rect& r)
	{
		return Contains(r.x, r.y) ||
			Contains(r.x + r.width, r.y) ||
			Contains(r.x + r.width, r.y + r.height) ||
			Contains(r.x, r.y + r.height);
	}

	/// Return true if a point lies within rectangle bounds.
	inline bool Contains (int px, int py) const		{ return (px >= x) && (px < x + width) && (py >= y) && (py < y + height); }

	inline bool Intersects(const Rect& r)
	{
		// Rects are disjoint if there's at least one separating axis
		bool disjoint = x + width < r.x;
		disjoint |= r.x + r.width < x;
		disjoint |= y + height < r.y;
		disjoint |= r.y + r.height < y;
		return !disjoint;
	}

	/// Normalize a rectangle such that xmin <= xmax and ymin <= ymax.
	inline void Normalize ()
	{
		width = std::max(width, 0);
		height = std::max(height, 0);
	}

	bool operator == (const Rect& r)const		{ return x == r.x && y == r.y && width == r.width && height == r.height; }
	bool operator != (const Rect& r)const		{ return x != r.x || y != r.y || width != r.width || height != r.height; }
};

/// A rectangle.
class Rectf
{
public:
	float x; ///< Rectangle x coordinate.
	float y; ///< Rectangle y coordinate.
	float width; ///< Rectangle width.
	float height; ///< Rectangle height.

	//DECLARE_SERIALIZE_OPTIMIZE_TRANSFER (Rectf)

	/// Create a empty rectangle.
	Rectf ();

	/// Create a new rectangle.
	Rectf (float inX, float inY, float iWidth, float iHeight)
	{
		x = inX; width = iWidth;
		y = inY; height = iHeight;
	}
	float GetLeft() const { return x; };
	float GetTop() const { return y; };
	float GetRight() const { return x + width; }
	float GetBottom() const { return y + height; }
	void SetLeft(float l) { float oldXMax = GetXMax(); x = l; width = oldXMax - x; }
	void SetTop(float t) { float oldYMax = GetYMax(); y = t; height = oldYMax - y; }
	void SetRight(float r) { width = r - x; }
	void SetBottom(float b) { height = b - y; }


	float GetXMax() const { return x + width; }
	float GetYMax() const { return y + height; }

	void Union(float px, float py);
	void Union(const Vector2f& p);
	void Union(const Rectf& rect);
	void Union(const Vector2f& p0, const Vector2f& p1);
	/// Return true if rectangle is empty.
	inline bool IsEmpty () const
	{ return width <= 0.00001F || height <= 0.00001F; }
	
	inline void		SetPosition(const Vector2f& position) { x = position.x; y = position.y; }
	inline Vector2f GetPosition() const { return Vector2f(x, y); }

	inline void		SetSize(const Vector2f& size) { width = size.x; height = size.y; }
	inline Vector2f GetSize() const { return Vector2f(width, height); }
	/// Resets the rectangle
	inline void Reset() 
	{ x = y = width = height = 0.0f; }

	/// Sets the rectangle
	inline void Set(float inX, float inY, float iWidth, float iHeight) 
	{
		x = inX; width = iWidth;
		y = inY; height = iHeight;
	}

	inline void Scale (float dx, float dy)		{ x *= dx; width *= dx; y *= dy; height *= dy;}

	/// Set Center position of rectangle (size stays the same)
	void SetCenterPos (float x, float y);
	Vector2f GetCenterPos()const;

	/// Ensure this is inside the rect r.
	void Clamp (const Rectf &r);

	/// Move rectangle by deltaX, deltaY.
	inline void Move (float dX, float dY)		{ x += dX; y += dY; }

	/// Return the width of rectangle.
	inline float Width () const					{ return width; }

	/// Return the height of rectangle.
	inline float Height () const					{ return height; }

	inline bool Contains (const Rectf& r)
	{
		return Contains(r.x, r.y) ||
			Contains(r.x + r.width, r.y) ||
			Contains(r.x + r.width, r.y + r.height) ||
			Contains(r.x, r.y + r.height);
	}

	/// Return true if a point lies within rectangle bounds.
	inline bool Contains (float px, float py) const		{ return (px >= x) && (px < x + width) && (py >= y) && (py < y + height); }
	inline bool Contains (const Vector2f& p) const		{ return Contains(p.x, p.y); }
	/// Return true if a relative point lies within rectangle bounds.
	inline bool ContainsRel (float x, float y) const
	{ return (x >= 0) && (x < Width ()) && (y >= 0) && (y < Height ()); }

	inline bool Intersects(const Rectf& r)
	{
		// Rects are disjoint if there's at least one separating axis
		bool disjoint = x + width < r.x;
		disjoint |= r.x + r.width < x;
		disjoint |= y + height < r.y;
		disjoint |= r.y + r.height < y;
		return !disjoint;
	}

	/// Normalize a rectangle such that xmin <= xmax and ymin <= ymax.
	inline void Normalize ()
	{
		width = std::max(width, 0.0F);
		height = std::max(height, 0.0F);
	}
	
	bool operator == (const Rectf& r)const		{ return x == r.x && y == r.y && width == r.width && height == r.height; }
	bool operator != (const Rectf& r)const		{ return x != r.x || y != r.y || width != r.width || height != r.height; }
};

inline bool CompareApproximately (const Rectf& lhs, const Rectf& rhs)
{
	return CompareApproximately (lhs.x, rhs.x) && CompareApproximately (lhs.y, rhs.y) && 
	         CompareApproximately (lhs.width, rhs.width) && CompareApproximately (lhs.height, rhs.height);
}

/// Make a rect with width & height
inline Rectf MinMaxRect (float minx, float miny, float maxx, float maxy) { return Rectf (minx, miny, maxx - minx, maxy - miny); }

QIMP_END_NAMESPACE

#endif //__MATH_RECT_H__
