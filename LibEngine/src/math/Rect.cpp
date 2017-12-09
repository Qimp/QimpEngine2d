#include "Private.h"


using namespace std;

QIMP_BEGIN_NAMESPACE
////////////////////////////////////////////////////////////////////////////////
//Rect
////////////////////////////////////////////////////////////////////////////////
void Rect::Clamp (const Rect &r) {
	int x2 = x + width;
	int y2 = y + height;
	int rx2 = r.x + r.width;
	int ry2 = r.y + r.height;

	if (x < r.x) x = r.x;
	if (x2 > rx2) x2 = rx2;
	if (y < r.y) y = r.y;
	if (y2 > ry2) y2 = ry2;

	width = x2 - x;
	if (width < 0) width = 0;

		height = y2 - y;
		if (height < 0) height = 0;
}

//----------------------------------------------------------
void Rect::Union(int px, int py)
{
	int x0 = std::min(GetLeft(), px);
	int x1 = std::max(GetRight(), px);
	int y0 = std::min(GetTop(), py);
	int y1 = std::max(GetBottom(), py);
	int w = x1 - x0;
	int h = y1 - y0;
	Set(x0, y0, w, h);
}

//----------------------------------------------------------
void Rect::Union(const Rect& rect) {
	int x0 = std::min(GetLeft(),rect.GetLeft());
	int x1 = std::max(GetRight(),rect.GetRight());
	int y0 = std::min(GetTop(),rect.GetTop());
	int y1 = std::max(GetBottom(),rect.GetBottom());
	int w = x1 - x0;
	int h = y1 - y0;
	Set(x0, y0, w, h);
}

////////////////////////////////////////////////////////////////////////////////
//Rectf
////////////////////////////////////////////////////////////////////////////////
Rectf::Rectf ()
{
  Reset();
}
void Rectf::SetCenterPos (float _x, float _y)
{
	x = _x - width * 0.5f;
	y = _y - height * 0.5f;
}

Vector2f Rectf::GetCenterPos()const
{
	return Vector2f(x + width * 0.5f, y + height * 0.5f);
}

void Rectf::Clamp (const Rectf &r) {
	float x2 = x + width;
	float y2 = y + height;
	float rx2 = r.x + r.width;
	float ry2 = r.y + r.height;

	if (x < r.x) x = r.x;
	if (x2 > rx2) x2 = rx2;
	if (y < r.y) y = r.y;
	if (y2 > ry2) y2 = ry2;

	width = x2 - x;
	if (width < 0.0f) width = 0.0f;

		height = y2 - y;
		if (height < 0.0f) height = 0.0f;
}

//----------------------------------------------------------
void Rectf::Union(float px, float py) {
	Union(Vector2f(px, py));
}

//----------------------------------------------------------
void Rectf::Union(const Vector2f& p) {
	float x0 = std::min(GetLeft(), p.x);
	float x1 = std::max(GetRight(), p.x);
	float y0 = std::min(GetTop(), p.y);
	float y1 = std::max(GetBottom(), p.y);
	float w = x1 - x0;
	float h = y1 - y0;
	Set(x0, y0, w, h);
}

//----------------------------------------------------------
void Rectf::Union(const Rectf& rect) {
	float x0 = std::min(GetLeft(),rect.GetLeft());
	float x1 = std::max(GetRight(),rect.GetRight());
	float y0 = std::min(GetTop(),rect.GetTop());
	float y1 = std::max(GetBottom(),rect.GetBottom());
	float w = x1 - x0;
	float h = y1 - y0;
	Set(x0, y0, w, h);
}

//----------------------------------------------------------
void Rectf::Union(const Vector2f& p0, const Vector2f& p1) {
	Union(p0);
	Union(p1);
}
QIMP_END_NAMESPACE