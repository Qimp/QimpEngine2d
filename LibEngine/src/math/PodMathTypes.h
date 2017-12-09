#ifndef __MATH_POD_MATH_TYPES_H__
#define __MATH_POD_MATH_TYPES_H__

namespace pod
{

struct v2f
{
	float x, y;
};

struct v3f
{
	float x, y, z;
};

struct m44f
{
	float m[16];
};

QIMP_END_NAMESPACE

#endif //__MATH_POD_MATH_TYPES_H__