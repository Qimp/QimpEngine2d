#pragma once

QIMP_BEGIN_NAMESPACE

static Vector2f CreateVector2f(float x, float y)
{
	Vector2f v(x, y);
	return v;
}

static Vector3f CreateVector3f(float x, float y, float z)
{
	Vector3f v(x, y, z);
	return v;
}

static Vector4f CreateVector4f(float x, float y, float z, float w)
{
	Vector4f v(x, y, z, w);
	return v;
}

static Quaternionf CreateQuaternionf(float x, float y, float z, float w)
{
	Quaternionf q(x, y, z, w);
	return q;
}

static Colorf CreateColorf(float r, float g, float b, float a)
{
	Colorf c(r, g, b, a);
	return c;
}

static Color32 CreateColor32(uint8 r, uint8 g, uint8 b, uint8 a)
{
	Color32 c(r, g, b, a);
	return c;
}

static Color32 CreateColor32ARGB(uint32 argb)
{
	uint8 r = (argb >> 16) & 0xff;
	uint8 g = (argb >>	8) & 0xff;
	uint8 b = (argb >>	0) & 0xff;
	uint8 a = (argb >> 24) & 0xff;

	Color32 c(r, g, b, a);
	return c;
}

static AABB CreateAABB(Vector3f c, Vector3f e)
{
	AABB aabb(c, e);
	return aabb;
}

static MinMaxAABB CreateMinMaxAABB(Vector3f min, Vector3f max)
{
	MinMaxAABB aabb(min, max);
	return aabb;
}

static OBB CreateOBB(Vector3f c, Vector3f q, Vector3f e)
{
	Quaternionf qua = EulerToQuaternion(q);
	OBB obb(c, qua, e);
	return obb;
}

static OBB CreateOBB(Vector3f s, Vector3f e)
{
	OBB obb(s, e);
	return obb;
}

static Rect CreateRect(int x, int y, int width, int height)
{
	Rect r(x, y, width, height);
	return r;
}

static Matrix4x4f CreateMatrix4x4f()
{
	return Matrix4x4f::identity;
}

QIMP_END_NAMESPACE