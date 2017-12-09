#ifndef SIMD_PPU
#define SIMD_PPU

#include "Runtime/Math/Simd/SimdMath.h"
#define USE_WPERMWI_EQUIVALENT 1
typedef vec_float4 vec4f;
typedef vec_bint4 vec4b;
typedef vec_bint4 vec4bs;


#if USE_WPERMWI_EQUIVALENT
#	define SWZ_MASK(x, y, z, w) (((x&3)<<6) | ((y&3)<<4) | ((z&3)<<2) | (w&3))
#else
#	define SWZ_MASK(x, y, z, w) (((x)<<24) | ((y)<<16) | ((z)<<8) | (w))
#endif

#define cvec4f(name, x,y,z,w) static const vec4f name = {(x),(y),(z),(w)}
#define cvec4b(name, x,y,z,w) static const vec4b name = {(x),(y),(z),(w)}
#define cvec4fs(name, s) static const vec4f name = {(s),(s),(s),(s)}

enum simd_mask
{
	kXYZW = SWZ_MASK(0,1,2,3),
	kXXXX = SWZ_MASK(0,0,0,0),
	kYYYY = SWZ_MASK(1,1,1,1),
	kZZZZ = SWZ_MASK(2,2,2,2),
	kWWWW = SWZ_MASK(3,3,3,3),
	
	kXWYZ = SWZ_MASK(0,3,1,2),
	kXZWY = SWZ_MASK(0,2,3,1),
	
	kYZWX = SWZ_MASK(1,2,3,0),
	kYXZW = SWZ_MASK(1,0,2,3),
	kYWZX = SWZ_MASK(1,3,2,0),
	kYZXW = SWZ_MASK(1,2,0,3),
	kYXWZ = SWZ_MASK(1,0,3,2),
	
    kZWXY = SWZ_MASK(2,3,0,1),
	kZYXW = SWZ_MASK(2,1,0,3),
	kZYWX = SWZ_MASK(2,1,3,0),
	kZXYW = SWZ_MASK(2,0,1,3),
	
	kWYZX = SWZ_MASK(3,1,2,0),
	kWXZY = SWZ_MASK(3,0,2,1),
	kWYXZ = SWZ_MASK(3,1,0,2),
	kWWWZ = SWZ_MASK(3,3,3,2),
	kWWZZ = SWZ_MASK(3,3,2,2),
	kWZYX = SWZ_MASK(3,2,1,0),
};

#define Vzero() __vzero()
#define Vone() vec_ctf(vec_splat_u32(1), 0)


#if USE_WPERMWI_EQUIVALENT
#	define Vpermute(v, mask) __vpermwi2<mask>( (v) )
#else
#	define Vpermute(v, mask) __vpermwi3( (v), (mask) )
#endif

#if USE_WPERMWI_EQUIVALENT

template <const int i>
vec_float4 __vpermwi2(vec_float4 v0a)
{
#if 1
	if (i == SWZ_MASK(0,0,0,0))
	{
		return vec_splat( v0a, 0 );
	}
	else if (i == SWZ_MASK(1,0,0,0))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		vec_float4	v2a			=	vec_splat( v0a, 0 );
		return						vec_sld( v1a, v2a, 12 );
	}
	else if (i == SWZ_MASK(2,0,0,0))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_splat( v0a, 0 );
		return						vec_sld( v1a, v2a, 12 );
	}
	else if (i == SWZ_MASK(3,0,0,0))
	{
		vec_float4	v1a			=	vec_splat( v0a, 0 );
		return						vec_sld( v0a, v1a, 12 );
	}
	else if (i == SWZ_MASK(0,1,0,0))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		vec_float4	v2a			=	vec_splat( v0a, 0 );
		return						vec_sld( v1a, v2a, 8 );
	}
	else if (i == SWZ_MASK(1,1,0,0))
	{
		vec_float4	v1a			=	vec_mergeh( v0a, v0a );
		return						vec_sld( v1a, v1a, 8 );
	}
	else if (i == SWZ_MASK(2,1,0,0))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_splat( v0a, 0 );
		vec_float4	v3a			=	vec_mergeh( v1a, v0a );
		return						vec_sld( v3a, v2a, 8 );
	}
	else if (i == SWZ_MASK(3,1,0,0))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_mergeh( v0a, v1a );
		return						vec_sld( v2a, v0a, 4 );
	}
	else if (i == SWZ_MASK(0,2,0,0))
	{
		vec_float4	v1a			=	vec_splat( v0a, 0 );
		vec_float4	v2a			=	vec_mergel( v0a, v1a );
		return						vec_mergeh( v1a, v2a );
	}
	else if (i == SWZ_MASK(1,2,0,0))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_splat( v0a, 0 );
		return						vec_sld( v1a, v2a, 8 );
	}
	else if (i == SWZ_MASK(2,2,0,0))
	{
		vec_float4	v1a			=	vec_splat( v0a, 2 );
		vec_float4	v2a			=	vec_splat( v0a, 0 );
		return						vec_sld( v1a, v2a, 8 );
	}
	else if (i == SWZ_MASK(3,2,0,0))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_sld( v1a, v0a, 12 );
		return						vec_mergeh( v1a, v2a );
	}
	else if (i == SWZ_MASK(0,3,0,0))
	{
		vec_float4	v1a			=	vec_splat( v0a, 0 );
		vec_float4	v2a			=	vec_sld( v0a, v0a, 4 );
		return						vec_mergel( v1a, v2a );
	}
	else if (i == SWZ_MASK(1,3,0,0))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_mergeh( v1a, v0a );
		return						vec_sld( v2a, v2a, 12 );
	}
	else if (i == SWZ_MASK(2,3,0,0))
	{
		vec_float4	v1a			=	vec_splat( v0a, 0 );
		return						vec_sld( v0a, v1a, 8 );
	}
	else if (i == SWZ_MASK(3,3,0,0))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		return						vec_mergel( v1a, v1a );
	}
	else if (i == SWZ_MASK(0,0,1,0))
	{
		vec_float4	v1a			=	vec_mergeh( v0a, v0a );
		return						vec_mergeh( v0a, v1a );
	}
	else if (i == SWZ_MASK(1,0,1,0))
	{
		vec_float4	v1a			=	vec_splat( v0a, 0 );
		vec_float4	v2a			=	vec_splat( v0a, 1 );
		return						vec_mergel( v2a, v1a );
	}
	else if (i == SWZ_MASK(2,0,1,0))
	{
		vec_float4	v1a			=	vec_splat( v0a, 2 );
		vec_float4	v2a			=	vec_sld( v1a, v0a, 8 );
		return						vec_sld( v2a, v0a, 4 );
	}
	else if (i == SWZ_MASK(3,0,1,0))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		return						vec_sld( v1a, v0a, 4 );
	}
	else if (i == SWZ_MASK(0,1,1,0))
	{
		vec_float4	v1a			=	vec_mergeh( v0a, v0a );
		return						vec_sld( v1a, v0a, 4 );
	}
	else if (i == SWZ_MASK(1,1,1,0))
	{
		vec_float4	v1a			=	vec_splat( v0a, 1 );
		return						vec_sld( v1a, v0a, 4 );
	}
	else if (i == SWZ_MASK(2,1,1,0))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_mergeh( v1a, v0a );
		return						vec_sld( v2a, v2a, 8 );
	}
	else if (i == SWZ_MASK(3,1,1,0))
	{
		vec_float4	v1a			=	vec_splat( v0a, 1 );
		vec_float4	v2a			=	vec_sld( v1a, v0a, 8 );
		return						vec_sld( v0a, v2a, 12 );
	}
	else if (i == SWZ_MASK(0,2,1,0))
	{
		vec_float4	v1a			=	vec_splat( v0a, 2 );
		vec_float4	v2a			=	vec_sld( v1a, v0a, 12 );
		return						vec_mergeh( v0a, v2a );
	}
	else if (i == SWZ_MASK(1,2,1,0))
	{
		vec_float4	v1a			=	vec_splat( v0a, 1 );
		vec_float4	v2a			=	vec_sld( v1a, v0a, 12 );
		return						vec_sld( v2a, v2a, 8 );
	}
	else if (i == SWZ_MASK(2,2,1,0))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		vec_float4	v2a			=	vec_splat( v0a, 2 );
		vec_float4	v3a			=	vec_sld( v1a, v0a, 12 );
		return						vec_sld( v2a, v3a, 8 );
	}
	else if (i == SWZ_MASK(3,2,1,0))
	{
		vec_float4	v1a			=	vec_splat( v0a, 1 );
		vec_float4	v2a			=	vec_sld( v1a, v0a, 12 );
		vec_float4	v3a			=	vec_mergel( v0a, v2a );
		return						vec_sld( v3a, v2a, 8 );
	}
	else if (i == SWZ_MASK(0,3,1,0))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		return						vec_mergeh( v0a, v1a );
	}
	else if (i == SWZ_MASK(1,3,1,0))
	{
		vec_float4	v1a			=	vec_mergeh( v0a, v0a );
		vec_float4	v2a			=	vec_sld( v0a, v0a, 4 );
		return						vec_mergel( v1a, v2a );
	}
	else if (i == SWZ_MASK(2,3,1,0))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_mergeh( v1a, v0a );
		return						vec_sld( v0a, v2a, 8 );
	}
	else if (i == SWZ_MASK(3,3,1,0))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_sld( v0a, v1a, 4 );
		return						vec_mergel( v2a, v1a );
	}
	else if (i == SWZ_MASK(0,0,2,0))
	{
		vec_float4	v1a			=	vec_splat( v0a, 0 );
		vec_float4	v2a			=	vec_mergel( v1a, v0a );
		return						vec_sld( v1a, v2a, 12 );
	}
	else if (i == SWZ_MASK(1,0,2,0))
	{
		vec_float4	v1a			=	vec_splat( v0a, 0 );
		vec_float4	v2a			=	vec_sld( v0a, v0a, 4 );
		return						vec_mergeh( v2a, v1a );
	}
	else if (i == SWZ_MASK(2,0,2,0))
	{
		vec_float4	v1a			=	vec_splat( v0a, 2 );
		vec_float4	v2a			=	vec_splat( v0a, 0 );
		return						vec_mergel( v1a, v2a );
	}
	else if (i == SWZ_MASK(3,0,2,0))
	{
		vec_float4	v1a			=	vec_splat( v0a, 0 );
		vec_float4	v2a			=	vec_mergel( v1a, v0a );
		return						vec_sld( v0a, v2a, 12 );
	}
	else if (i == SWZ_MASK(0,1,2,0))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		return						vec_sld( v1a, v0a, 4 );
	}
	else if (i == SWZ_MASK(1,1,2,0))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_mergeh( v0a, v1a );
		return						vec_sld( v2a, v0a, 4 );
	}
	else if (i == SWZ_MASK(2,1,2,0))
	{
		vec_float4	v1a			=	vec_splat( v0a, 2 );
		vec_float4	v2a			=	vec_mergeh( v1a, v0a );
		return						vec_sld( v2a, v2a, 8 );
	}
	else if (i == SWZ_MASK(3,1,2,0))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		vec_float4	v2a			=	vec_mergel( v0a, v1a );
		return						vec_sld( v2a, v2a, 8 );
	}
	else if (i == SWZ_MASK(0,2,2,0))
	{
		vec_float4	v1a			=	vec_splat( v0a, 2 );
		vec_float4	v2a			=	vec_splat( v0a, 0 );
		vec_float4	v3a			=	vec_sld( v1a, v0a, 8 );
		return						vec_sld( v2a, v3a, 12 );
	}
	else if (i == SWZ_MASK(1,2,2,0))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_mergeh( v1a, v1a );
		return						vec_sld( v2a, v0a, 4 );
	}
	else if (i == SWZ_MASK(2,2,2,0))
	{
		vec_float4	v1a			=	vec_splat( v0a, 2 );
		return						vec_sld( v1a, v0a, 4 );
	}
	else if (i == SWZ_MASK(3,2,2,0))
	{
		vec_float4	v1a			=	vec_mergel( v0a, v0a );
		vec_float4	v2a			=	vec_sld( v0a, v1a, 8 );
		return						vec_sld( v2a, v0a, 4 );
	}
	else if (i == SWZ_MASK(0,3,2,0))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_mergel( v1a, v0a );
		return						vec_sld( v1a, v2a, 12 );
	}
	else if (i == SWZ_MASK(1,3,2,0))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_sld( v0a, v0a, 12 );
		return						vec_mergel( v2a, v1a );
	}
	else if (i == SWZ_MASK(2,3,2,0))
	{
		vec_float4	v1a			=	vec_splat( v0a, 2 );
		vec_float4	v2a			=	vec_sld( v0a, v0a, 4 );
		return						vec_mergel( v1a, v2a );
	}
	else if (i == SWZ_MASK(3,3,2,0))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_mergel( v1a, v0a );
		return						vec_sld( v0a, v2a, 12 );
	}
	else if (i == SWZ_MASK(0,0,3,0))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_splat( v0a, 0 );
		return						vec_sld( v2a, v1a, 8 );
	}
	else if (i == SWZ_MASK(1,0,3,0))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_mergeh( v0a, v1a );
		return						vec_sld( v2a, v1a, 8 );
	}
	else if (i == SWZ_MASK(2,0,3,0))
	{
		vec_float4	v1a			=	vec_splat( v0a, 0 );
		return						vec_mergel( v0a, v1a );
	}
	else if (i == SWZ_MASK(3,0,3,0))
	{
		vec_float4	v1a			=	vec_splat( v0a, 0 );
		vec_float4	v2a			=	vec_splat( v0a, 3 );
		return						vec_mergel( v2a, v1a );
	}
	else if (i == SWZ_MASK(0,1,3,0))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		vec_float4	v2a			=	vec_sld( v0a, v0a, 12 );
		return						vec_sld( v1a, v2a, 8 );
	}
	else if (i == SWZ_MASK(1,1,3,0))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_splat( v0a, 1 );
		return						vec_sld( v2a, v1a, 8 );
	}
	else if (i == SWZ_MASK(2,1,3,0))
	{
		vec_float4	v1a			=	vec_splat( v0a, 1 );
		vec_float4	v2a			=	vec_sld( v1a, v0a, 4 );
		return						vec_mergel( v0a, v2a );
	}
	else if (i == SWZ_MASK(3,1,3,0))
	{
		vec_float4	v1a			=	vec_splat( v0a, 3 );
		vec_float4	v2a			=	vec_mergeh( v1a, v0a );
		return						vec_sld( v2a, v2a, 8 );
	}
	else if (i == SWZ_MASK(0,2,3,0))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		vec_float4	v2a			=	vec_splat( v0a, 0 );
		return						vec_sld( v2a, v1a, 12 );
	}
	else if (i == SWZ_MASK(1,2,3,0))
	{
		return						vec_sld( v0a, v0a, 4 );
	}
	else if (i == SWZ_MASK(2,2,3,0))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		vec_float4	v2a			=	vec_splat( v0a, 2 );
		return						vec_sld( v2a, v1a, 12 );
	}
	else if (i == SWZ_MASK(3,2,3,0))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		return						vec_sld( v0a, v1a, 12 );
	}
	else if (i == SWZ_MASK(0,3,3,0))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_mergel( v1a, v1a );
		return						vec_sld( v1a, v2a, 12 );
	}
	else if (i == SWZ_MASK(1,3,3,0))
	{
		vec_float4	v1a			=	vec_splat( v0a, 3 );
		vec_float4	v2a			=	vec_sld( v1a, v0a, 8 );
		return						vec_sld( v2a, v2a, 12 );
	}
	else if (i == SWZ_MASK(2,3,3,0))
	{
		vec_float4	v1a			=	vec_mergel( v0a, v0a );
		return						vec_sld( v1a, v0a, 4 );
	}
	else if (i == SWZ_MASK(3,3,3,0))
	{
		vec_float4	v1a			=	vec_splat( v0a, 3 );
		return						vec_sld( v1a, v0a, 4 );
	}
	else if (i == SWZ_MASK(0,0,0,1))
	{
		vec_float4	v1a			=	vec_splat( v0a, 0 );
		return						vec_sld( v1a, v0a, 8 );
	}
	else if (i == SWZ_MASK(1,0,0,1))
	{
		vec_float4	v1a			=	vec_mergeh( v0a, v0a );
		return						vec_sld( v1a, v1a, 12 );
	}
	else if (i == SWZ_MASK(2,0,0,1))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_mergeh( v0a, v0a );
		return						vec_sld( v1a, v2a, 12 );
	}
	else if (i == SWZ_MASK(3,0,0,1))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		return						vec_mergeh( v1a, v0a );
	}
	else if (i == SWZ_MASK(0,1,0,1))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		return						vec_sld( v1a, v0a, 8 );
	}
	else if (i == SWZ_MASK(1,1,0,1))
	{
		vec_float4	v1a			=	vec_splat( v0a, 1 );
		return						vec_sld( v1a, v0a, 8 );
	}
	else if (i == SWZ_MASK(2,1,0,1))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_mergeh( v1a, v0a );
		return						vec_sld( v2a, v0a, 8 );
	}
	else if (i == SWZ_MASK(3,1,0,1))
	{
		vec_float4	v1a			=	vec_splat( v0a, 1 );
		vec_float4	v2a			=	vec_sld( v0a, v0a, 4 );
		return						vec_mergel( v2a, v1a );
	}
	else if (i == SWZ_MASK(0,2,0,1))
	{
		vec_float4	v1a			=	vec_mergeh( v0a, v0a );
		vec_float4	v2a			=	vec_mergel( v0a, v1a );
		return						vec_mergeh( v1a, v2a );
	}
	else if (i == SWZ_MASK(1,2,0,1))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		return						vec_sld( v1a, v0a, 8 );
	}
	else if (i == SWZ_MASK(2,2,0,1))
	{
		vec_float4	v1a			=	vec_splat( v0a, 2 );
		return						vec_sld( v1a, v0a, 8 );
	}
	else if (i == SWZ_MASK(3,2,0,1))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_mergel( v0a, v1a );
		return						vec_sld( v2a, v0a, 8 );
	}
	else if (i == SWZ_MASK(0,3,0,1))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_splat( v0a, 0 );
		return						vec_sld( v2a, v1a, 12 );
	}
	else if (i == SWZ_MASK(1,3,0,1))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_splat( v0a, 1 );
		return						vec_sld( v2a, v1a, 12 );
	}
	else if (i == SWZ_MASK(2,3,0,1))
	{
		return						vec_sld( v0a, v0a, 8 );
	}
	else if (i == SWZ_MASK(3,3,0,1))
	{
		vec_float4	v1a			=	vec_splat( v0a, 3 );
		return						vec_sld( v1a, v0a, 8 );
	}
	else if (i == SWZ_MASK(0,0,1,1))
	{
		return						vec_mergeh( v0a, v0a );
	}
	else if (i == SWZ_MASK(1,0,1,1))
	{
		vec_float4	v1a			=	vec_splat( v0a, 1 );
		return						vec_mergeh( v1a, v0a );
	}
	else if (i == SWZ_MASK(2,0,1,1))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_mergeh( v0a, v1a );
		return						vec_sld( v2a, v2a, 12 );
	}
	else if (i == SWZ_MASK(3,0,1,1))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		vec_float4	v2a			=	vec_splat( v0a, 1 );
		return						vec_sld( v1a, v2a, 4 );
	}
	else if (i == SWZ_MASK(0,1,1,1))
	{
		vec_float4	v1a			=	vec_splat( v0a, 1 );
		return						vec_mergeh( v0a, v1a );
	}
	else if (i == SWZ_MASK(1,1,1,1))
	{
		return						vec_splat( v0a, 1 );
	}
	else if (i == SWZ_MASK(2,1,1,1))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_splat( v0a, 1 );
		return						vec_sld( v1a, v2a, 12 );
	}
	else if (i == SWZ_MASK(3,1,1,1))
	{
		vec_float4	v1a			=	vec_splat( v0a, 1 );
		return						vec_sld( v0a, v1a, 12 );
	}
	else if (i == SWZ_MASK(0,2,1,1))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_mergeh( v1a, v0a );
		return						vec_sld( v2a, v1a, 4 );
	}
	else if (i == SWZ_MASK(1,2,1,1))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_splat( v0a, 1 );
		return						vec_sld( v1a, v2a, 8 );
	}
	else if (i == SWZ_MASK(2,2,1,1))
	{
		vec_float4	v1a			=	vec_splat( v0a, 1 );
		vec_float4	v2a			=	vec_splat( v0a, 2 );
		return						vec_sld( v2a, v1a, 8 );
	}
	else if (i == SWZ_MASK(3,2,1,1))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_splat( v0a, 1 );
		vec_float4	v3a			=	vec_mergel( v0a, v1a );
		return						vec_sld( v3a, v2a, 8 );
	}
	else if (i == SWZ_MASK(0,3,1,1))
	{
		vec_float4	v1a			=	vec_splat( v0a, 1 );
		vec_float4	v2a			=	vec_sld( v0a, v1a, 12 );
		return						vec_mergeh( v0a, v2a );
	}
	else if (i == SWZ_MASK(1,3,1,1))
	{
		vec_float4	v1a			=	vec_splat( v0a, 1 );
		vec_float4	v2a			=	vec_sld( v0a, v1a, 4 );
		return						vec_mergel( v1a, v2a );
	}
	else if (i == SWZ_MASK(2,3,1,1))
	{
		vec_float4	v1a			=	vec_splat( v0a, 1 );
		return						vec_sld( v0a, v1a, 8 );
	}
	else if (i == SWZ_MASK(3,3,1,1))
	{
		vec_float4	v1a			=	vec_splat( v0a, 3 );
		vec_float4	v2a			=	vec_splat( v0a, 1 );
		return						vec_sld( v1a, v2a, 8 );
	}
	else if (i == SWZ_MASK(0,0,2,1))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		vec_float4	v2a			=	vec_mergel( v1a, v0a );
		return						vec_mergeh( v2a, v0a );
	}
	else if (i == SWZ_MASK(1,0,2,1))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		return						vec_mergeh( v1a, v0a );
	}
	else if (i == SWZ_MASK(2,0,2,1))
	{
		vec_float4	v1a			=	vec_splat( v0a, 2 );
		return						vec_mergeh( v1a, v0a );
	}
	else if (i == SWZ_MASK(3,0,2,1))
	{
		vec_float4	v1a			=	vec_splat( v0a, 2 );
		vec_float4	v2a			=	vec_sld( v0a, v1a, 12 );
		return						vec_mergeh( v2a, v0a );
	}
	else if (i == SWZ_MASK(0,1,2,1))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_splat( v0a, 1 );
		return						vec_sld( v1a, v2a, 4 );
	}

	else if (i == SWZ_MASK(1,1,2,1))
	{
		vec_float4	v1a			=	vec_splat( v0a, 1 );
		vec_float4	v2a			=	vec_sld( v0a, v0a, 4 );
		return						vec_mergeh( v2a, v1a );
	}

	else if (i == SWZ_MASK(2,1,2,1))
	{
		vec_float4	v1a			=	vec_mergeh( v0a, v0a );
		vec_float4	v2a			=	vec_splat( v0a, 2 );
		return						vec_mergel( v2a, v1a );
	}

	else if (i == SWZ_MASK(3,1,2,1))
	{
		vec_float4	v1a			=	vec_splat( v0a, 1 );
		vec_float4	v2a			=	vec_mergel( v1a, v0a );
		return						vec_sld( v0a, v2a, 12 );
	}

	else if (i == SWZ_MASK(0,2,2,1))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_splat( v0a, 2 );
		vec_float4	v3a			=	vec_sld( v2a, v1a, 8 );
		return						vec_sld( v1a, v3a, 12 );
	}

	else if (i == SWZ_MASK(1,2,2,1))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_mergeh( v1a, v1a );
		return						vec_sld( v2a, v1a, 4 );
	}

	else if (i == SWZ_MASK(2,2,2,1))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_splat( v0a, 2 );
		return						vec_sld( v2a, v1a, 4 );
	}

	else if (i == SWZ_MASK(3,2,2,1))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_mergel( v0a, v1a );
		return						vec_sld( v2a, v2a, 8 );
	}

	else if (i == SWZ_MASK(0,3,2,1))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_mergel( v1a, v0a );
		vec_float4	v3a			=	vec_sld( v1a, v2a, 8 );
		return						vec_sld( v3a, v1a, 4 );
	}

	else if (i == SWZ_MASK(1,3,2,1))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_sld( v0a, v1a, 12 );
		return						vec_mergeh( v1a, v2a );
	}

	else if (i == SWZ_MASK(2,3,2,1))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_mergel( v0a, v1a );
		return						vec_sld( v0a, v2a, 8 );
	}

	else if (i == SWZ_MASK(3,3,2,1))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_splat( v0a, 3 );
		vec_float4	v3a			=	vec_mergel( v0a, v1a );
		return						vec_sld( v2a, v3a, 8 );
	}

	else if (i == SWZ_MASK(0,0,3,1))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_mergeh( v0a, v1a );
		return						vec_sld( v2a, v2a, 12 );
	}

	else if (i == SWZ_MASK(1,0,3,1))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		vec_float4	v2a			=	vec_mergel( v1a, v0a );
		return						vec_mergel( v2a, v1a );
	}

	else if (i == SWZ_MASK(2,0,3,1))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		return						vec_mergel( v0a, v1a );
	}

	else if (i == SWZ_MASK(3,0,3,1))
	{
		vec_float4	v1a			=	vec_splat( v0a, 3 );
		return						vec_mergeh( v1a, v0a );
	}

	else if (i == SWZ_MASK(0,1,3,1))
	{
		vec_float4	v1a			=	vec_splat( v0a, 1 );
		vec_float4	v2a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v3a			=	vec_mergel( v2a, v1a );
		return						vec_sld( v3a, v3a, 8 );
	}

	else if (i == SWZ_MASK(1,1,3,1))
	{
		vec_float4	v1a			=	vec_splat( v0a, 1 );
		vec_float4	v2a			=	vec_sld( v0a, v1a, 12 );
		return						vec_sld( v1a, v2a, 8 );
	}

	else if (i == SWZ_MASK(2,1,3,1))
	{
		vec_float4	v1a			=	vec_splat( v0a, 1 );
		return						vec_mergel( v0a, v1a );
	}

	else if (i == SWZ_MASK(3,1,3,1))
	{
		vec_float4	v1a			=	vec_splat( v0a, 3 );
		vec_float4	v2a			=	vec_splat( v0a, 1 );
		return						vec_mergel( v1a, v2a );
	}

	else if (i == SWZ_MASK(0,2,3,1))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_sld( v0a, v1a, 8 );
		return						vec_sld( v1a, v2a, 12 );
	}

	else if (i == SWZ_MASK(1,2,3,1))
	{
		vec_float4	v1a			=	vec_splat( v0a, 1 );
		return						vec_sld( v0a, v1a, 4 );
	}

	else if (i == SWZ_MASK(2,2,3,1))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_mergel( v1a, v0a );
		return						vec_sld( v2a, v2a, 4 );
	}

	else if (i == SWZ_MASK(3,2,3,1))
	{
		vec_float4	v1a			=	vec_splat( v0a, 1 );
		vec_float4	v2a			=	vec_sld( v0a, v1a, 8 );
		return						vec_sld( v0a, v2a, 12 );
	}

	else if (i == SWZ_MASK(0,3,3,1))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_mergel( v1a, v0a );
		vec_float4	v3a			=	vec_sld( v0a, v1a, 4 );
		return						vec_mergel( v2a, v3a );
	}

	else if (i == SWZ_MASK(1,3,3,1))
	{
		vec_float4	v1a			=	vec_splat( v0a, 1 );
		vec_float4	v2a			=	vec_sld( v0a, v1a, 12 );
		vec_float4	v3a			=	vec_mergel( v1a, v0a );
		return						vec_sld( v3a, v2a, 8 );
	}

	else if (i == SWZ_MASK(2,3,3,1))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_mergel( v0a, v0a );
		return						vec_sld( v2a, v1a, 4 );
	}

	else if (i == SWZ_MASK(3,3,3,1))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_splat( v0a, 3 );
		return						vec_sld( v2a, v1a, 4 );
	}

	else if (i == SWZ_MASK(0,0,0,2))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		vec_float4	v2a			=	vec_splat( v0a, 0 );
		return						vec_sld( v2a, v1a, 4 );
	}

	else if (i == SWZ_MASK(1,0,0,2))
	{
		vec_float4	v1a			=	vec_mergeh( v0a, v0a );
		vec_float4	v2a			=	vec_splat( v0a, 2 );
		vec_float4	v3a			=	vec_sld( v1a, v1a, 8 );
		return						vec_sld( v3a, v2a, 4 );
	}

	else if (i == SWZ_MASK(2,0,0,2))
	{
		vec_float4	v1a			=	vec_splat( v0a, 2 );
		vec_float4	v2a			=	vec_sld( v1a, v0a, 4 );
		vec_float4	v3a			=	vec_mergeh( v0a, v1a );
		return						vec_sld( v2a, v3a, 8 );
	}

	else if (i == SWZ_MASK(3,0,0,2))
	{
		vec_float4	v1a			=	vec_mergeh( v0a, v0a );
		vec_float4	v2a			=	vec_sld( v0a, v1a, 8 );
		return						vec_sld( v2a, v2a, 4 );
	}

	else if (i == SWZ_MASK(0,1,0,2))
	{
		vec_float4	v1a			=	vec_splat( v0a, 0 );
		vec_float4	v2a			=	vec_sld( v0a, v0a, 4 );
		return						vec_mergeh( v1a, v2a );
	}

	else if (i == SWZ_MASK(1,1,0,2))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_mergeh( v1a, v0a );
		return						vec_sld( v2a, v2a, 12 );
	}

	else if (i == SWZ_MASK(2,1,0,2))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_sld( v1a, v0a, 4 );
		return						vec_mergel( v2a, v1a );
	}

	else if (i == SWZ_MASK(3,1,0,2))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_sld( v0a, v0a, 12 );
		return						vec_mergel( v1a, v2a );
	}

	else if (i == SWZ_MASK(0,2,0,2))
	{
		vec_float4	v1a			=	vec_splat( v0a, 2 );
		vec_float4	v2a			=	vec_splat( v0a, 0 );
		return						vec_mergel( v2a, v1a );
	}

	else if (i == SWZ_MASK(1,2,0,2))
	{
		vec_float4	v1a			=	vec_splat( v0a, 2 );
		vec_float4	v2a			=	vec_mergeh( v1a, v0a );
		return						vec_sld( v2a, v2a, 12 );
	}

	else if (i == SWZ_MASK(2,2,0,2))
	{
		vec_float4	v1a			=	vec_splat( v0a, 2 );
		vec_float4	v2a			=	vec_sld( v1a, v0a, 4 );
		return						vec_mergel( v2a, v1a );
	}

	else if (i == SWZ_MASK(3,2,0,2))
	{
		vec_float4	v1a			=	vec_splat( v0a, 2 );
		vec_float4	v2a			=	vec_sld( v0a, v0a, 4 );
		return						vec_mergel( v2a, v1a );
	}

	else if (i == SWZ_MASK(0,3,0,2))
	{
		vec_float4	v1a			=	vec_splat( v0a, 0 );
		vec_float4	v2a			=	vec_mergel( v1a, v0a );
		return						vec_sld( v2a, v2a, 8 );
	}

	else if (i == SWZ_MASK(1,3,0,2))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		vec_float4	v2a			=	vec_mergel( v1a, v0a );
		return						vec_sld( v2a, v2a, 8 );
	}

	else if (i == SWZ_MASK(2,3,0,2))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_splat( v0a, 2 );
		return						vec_sld( v1a, v2a, 4 );
	}

	else if (i == SWZ_MASK(3,3,0,2))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_mergel( v0a, v1a );
		return						vec_sld( v2a, v2a, 4 );
	}

	else if (i == SWZ_MASK(0,0,1,2))
	{
		vec_float4	v1a			=	vec_splat( v0a, 0 );
		return						vec_sld( v1a, v0a, 12 );
	}

	else if (i == SWZ_MASK(1,0,1,2))
	{
		vec_float4	v1a			=	vec_splat( v0a, 1 );
		return						vec_sld( v1a, v0a, 12 );
	}

	else if (i == SWZ_MASK(2,0,1,2))
	{
		vec_float4	v1a			=	vec_splat( v0a, 2 );
		return						vec_sld( v1a, v0a, 12 );
	}

	else if (i == SWZ_MASK(3,0,1,2))
	{
		return						vec_sld( v0a, v0a, 12 );
	}

	else if (i == SWZ_MASK(0,1,1,2))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		return						vec_mergeh( v0a, v1a );
	}

	else if (i == SWZ_MASK(1,1,1,2))
	{
		vec_float4	v1a			=	vec_splat( v0a, 1 );
		vec_float4	v2a			=	vec_sld( v0a, v0a, 4 );
		return						vec_mergeh( v1a, v2a );
	}

	else if (i == SWZ_MASK(2,1,1,2))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_mergel( v1a, v1a );
		return						vec_sld( v1a, v2a, 12 );
	}

	else if (i == SWZ_MASK(3,1,1,2))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_mergeh( v1a, v1a );
		return						vec_sld( v0a, v2a, 12 );
	}

	else if (i == SWZ_MASK(0,2,1,2))
	{
		vec_float4	v1a			=	vec_mergel( v0a, v0a );
		return						vec_mergeh( v0a, v1a );
	}

	else if (i == SWZ_MASK(1,2,1,2))
	{
		vec_float4	v1a			=	vec_splat( v0a, 1 );
		vec_float4	v2a			=	vec_splat( v0a, 2 );
		return						vec_mergel( v1a, v2a );
	}

	else if (i == SWZ_MASK(2,2,1,2))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_splat( v0a, 2 );
		return						vec_sld( v2a, v1a, 8 );
	}

	else if (i == SWZ_MASK(3,2,1,2))
	{
		vec_float4	v1a			=	vec_splat( v0a, 2 );
		vec_float4	v2a			=	vec_sld( v0a, v1a, 4 );
		return						vec_sld( v2a, v2a, 8 );
	}

	else if (i == SWZ_MASK(0,3,1,2))
	{
		vec_float4	v1a			=	vec_splat( v0a, 2 );
		vec_float4	v2a			=	vec_sld( v0a, v1a, 12 );
		return						vec_mergeh( v0a, v2a );
	}

	else if (i == SWZ_MASK(1,3,1,2))
	{
		vec_float4	v1a			=	vec_splat( v0a, 1 );
		vec_float4	v2a			=	vec_mergel( v1a, v0a );
		return						vec_sld( v2a, v2a, 8 );
	}

	else if (i == SWZ_MASK(2,3,1,2))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		return						vec_sld( v0a, v1a, 8 );
	}

	else if (i == SWZ_MASK(3,3,1,2))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_splat( v0a, 3 );
		return						vec_sld( v2a, v1a, 8 );
	}

	else if (i == SWZ_MASK(0,0,2,2))
	{
		vec_float4	v1a			=	vec_splat( v0a, 0 );
		vec_float4	v2a			=	vec_splat( v0a, 2 );
		return						vec_sld( v1a, v2a, 8 );
	}

	else if (i == SWZ_MASK(1,0,2,2))
	{
		vec_float4	v1a			=	vec_splat( v0a, 0 );
		vec_float4	v2a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v3a			=	vec_mergel( v1a, v0a );
		return						vec_mergeh( v2a, v3a );
	}

	else if (i == SWZ_MASK(2,0,2,2))
	{
		vec_float4	v1a			=	vec_splat( v0a, 2 );
		vec_float4	v2a			=	vec_sld( v1a, v0a, 4 );
		return						vec_sld( v2a, v1a, 8 );
	}

	else if (i == SWZ_MASK(3,0,2,2))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_splat( v0a, 2 );
		return						vec_sld( v1a, v2a, 8 );
	}

	else if (i == SWZ_MASK(0,1,2,2))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		vec_float4	v2a			=	vec_splat( v0a, 2 );
		return						vec_sld( v1a, v2a, 8 );
	}

	else if (i == SWZ_MASK(1,1,2,2))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		return						vec_mergel( v1a, v1a );
	}

	else if (i == SWZ_MASK(2,1,2,2))
	{
		vec_float4	v1a			=	vec_splat( v0a, 2 );
		vec_float4	v2a			=	vec_sld( v0a, v0a, 4 );
		return						vec_mergeh( v1a, v2a );
	}

	else if (i == SWZ_MASK(3,1,2,2))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_mergel( v1a, v0a );
		return						vec_sld( v0a, v2a, 12 );
	}

	else if (i == SWZ_MASK(0,2,2,2))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_splat( v0a, 2 );
		return						vec_sld( v1a, v2a, 12 );
	}

	else if (i == SWZ_MASK(1,2,2,2))
	{
		vec_float4	v1a			=	vec_mergel( v0a, v0a );
		vec_float4	v2a			=	vec_sld( v0a, v0a, 4 );
		return						vec_mergeh( v2a, v1a );
	}

	else if (i == SWZ_MASK(2,2,2,2))
	{
		return						vec_splat( v0a, 2 );
	}

	else if (i == SWZ_MASK(3,2,2,2))
	{
		vec_float4	v1a			=	vec_splat( v0a, 2 );
		return						vec_sld( v0a, v1a, 12 );
	}

	else if (i == SWZ_MASK(0,3,2,2))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_splat( v0a, 2 );
		vec_float4	v3a			=	vec_mergel( v1a, v0a );
		return						vec_sld( v3a, v2a, 8 );
	}

	else if (i == SWZ_MASK(1,3,2,2))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_mergel( v0a, v1a );
		return						vec_sld( v2a, v2a, 4 );
	}

	else if (i == SWZ_MASK(2,3,2,2))
	{
		vec_float4	v1a			=	vec_splat( v0a, 2 );
		return						vec_sld( v0a, v1a, 8 );
	}

	else if (i == SWZ_MASK(3,3,2,2))
	{
		vec_float4	v1a			=	vec_mergel( v0a, v0a );
		return						vec_sld( v1a, v1a, 8 );
	}

	else if (i == SWZ_MASK(0,0,3,2))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		vec_float4	v2a			=	vec_splat( v0a, 0 );
		vec_float4	v3a			=	vec_sld( v0a, v1a, 12 );
		return						vec_sld( v2a, v3a, 8 );
	}

	else if (i == SWZ_MASK(1,0,3,2))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		vec_float4	v2a			=	vec_mergel( v1a, v0a );
		vec_float4	v3a			=	vec_sld( v2a, v0a, 8 );
		return						vec_mergeh( v3a, v2a );
	}

	else if (i == SWZ_MASK(2,0,3,2))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		vec_float4	v2a			=	vec_mergel( v1a, v0a );
		return						vec_mergeh( v1a, v2a );
	}

	else if (i == SWZ_MASK(3,0,3,2))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_mergel( v1a, v0a );
		return						vec_sld( v1a, v2a, 8 );
	}

	else if (i == SWZ_MASK(0,1,3,2))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		vec_float4	v2a			=	vec_sld( v0a, v1a, 12 );
		return						vec_sld( v1a, v2a, 8 );
	}

	else if (i == SWZ_MASK(1,1,3,2))
	{
		vec_float4	v1a			=	vec_splat( v0a, 1 );
		vec_float4	v2a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v3a			=	vec_mergel( v1a, v0a );
		return						vec_mergel( v3a, v2a );
	}

	else if (i == SWZ_MASK(2,1,3,2))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		return						vec_mergel( v0a, v1a );
	}

	else if (i == SWZ_MASK(3,1,3,2))
	{
		vec_float4	v1a			=	vec_splat( v0a, 3 );
		vec_float4	v2a			=	vec_sld( v0a, v0a, 4 );
		return						vec_mergeh( v1a, v2a );
	}

	else if (i == SWZ_MASK(0,2,3,2))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		vec_float4	v2a			=	vec_splat( v0a, 0 );
		vec_float4	v3a			=	vec_sld( v0a, v1a, 8 );
		return						vec_sld( v2a, v3a, 12 );
	}

	else if (i == SWZ_MASK(1,2,3,2))
	{
		vec_float4	v1a			=	vec_mergel( v0a, v0a );
		return						vec_sld( v0a, v1a, 4 );
	}

	else if (i == SWZ_MASK(2,2,3,2))
	{
		vec_float4	v1a			=	vec_splat( v0a, 2 );
		return						vec_mergel( v0a, v1a );
	}

	else if (i == SWZ_MASK(3,2,3,2))
	{
		vec_float4	v1a			=	vec_splat( v0a, 2 );
		vec_float4	v2a			=	vec_splat( v0a, 3 );
		return						vec_mergel( v2a, v1a );
	}

	else if (i == SWZ_MASK(0,3,3,2))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_mergel( v1a, v0a );
		return						vec_sld( v2a, v2a, 8 );
	}

	else if (i == SWZ_MASK(1,3,3,2))
	{
		vec_float4	v1a			=	vec_mergel( v0a, v0a );
		vec_float4	v2a			=	vec_splat( v0a, 1 );
		vec_float4	v3a			=	vec_sld( v1a, v1a, 8 );
		return						vec_sld( v2a, v3a, 12 );
	}

	else if (i == SWZ_MASK(2,3,3,2))
	{
		vec_float4	v1a			=	vec_mergel( v0a, v0a );
		return						vec_sld( v1a, v1a, 4 );
	}

	else if (i == SWZ_MASK(3,3,3,2))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		vec_float4	v2a			=	vec_splat( v0a, 3 );
		return						vec_sld( v2a, v1a, 4 );
	}

	else if (i == SWZ_MASK(0,0,0,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_splat( v0a, 0 );
		return						vec_sld( v2a, v1a, 4 );
	}

	else if (i == SWZ_MASK(1,0,0,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_mergeh( v0a, v1a );
		return						vec_sld( v2a, v2a, 8 );
	}

	else if (i == SWZ_MASK(2,0,0,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_mergeh( v0a, v1a );
		vec_float4	v3a			=	vec_sld( v1a, v0a, 12 );
		return						vec_mergeh( v3a, v2a );
	}

	else if (i == SWZ_MASK(3,0,0,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_mergel( v1a, v1a );
		return						vec_sld( v2a, v2a, 4 );
	}

	else if (i == SWZ_MASK(0,1,0,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		vec_float4	v2a			=	vec_sld( v1a, v0a, 4 );
		return						vec_sld( v2a, v2a, 4 );
	}

	else if (i == SWZ_MASK(1,1,0,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_splat( v0a, 1 );
		vec_float4	v3a			=	vec_mergeh( v0a, v1a );
		return						vec_sld( v2a, v3a, 8 );
	}

	else if (i == SWZ_MASK(2,1,0,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_mergeh( v0a, v1a );
		vec_float4	v3a			=	vec_sld( v2a, v1a, 8 );
		return						vec_sld( v1a, v3a, 12 );
	}

	else if (i == SWZ_MASK(3,1,0,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_mergeh( v0a, v1a );
		return						vec_sld( v2a, v1a, 4 );
	}

	else if (i == SWZ_MASK(0,2,0,3))
	{
		vec_float4	v1a			=	vec_splat( v0a, 0 );
		return						vec_mergel( v1a, v0a );
	}

	else if (i == SWZ_MASK(1,2,0,3))
	{
		vec_float4	v1a			=	vec_splat( v0a, 1 );
		vec_float4	v2a			=	vec_sld( v1a, v0a, 4 );
		return						vec_mergel( v2a, v0a );
	}

	else if (i == SWZ_MASK(2,2,0,3))
	{
		vec_float4	v1a			=	vec_splat( v0a, 2 );
		vec_float4	v2a			=	vec_sld( v1a, v0a, 4 );
		return						vec_mergel( v2a, v0a );
	}

	else if (i == SWZ_MASK(3,2,0,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		return						vec_mergel( v1a, v0a );
	}

	else if (i == SWZ_MASK(0,3,0,3))
	{
		vec_float4	v1a			=	vec_splat( v0a, 3 );
		vec_float4	v2a			=	vec_splat( v0a, 0 );
		return						vec_mergel( v2a, v1a );
	}

	else if (i == SWZ_MASK(1,3,0,3))
	{
		vec_float4	v1a			=	vec_splat( v0a, 3 );
		vec_float4	v2a			=	vec_mergeh( v1a, v0a );
		return						vec_sld( v2a, v2a, 12 );
	}

	else if (i == SWZ_MASK(2,3,0,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_splat( v0a, 3 );
		return						vec_sld( v1a, v2a, 4 );
	}

	else if (i == SWZ_MASK(3,3,0,3))
	{
		vec_float4	v1a			=	vec_mergel( v0a, v0a );
		vec_float4	v2a			=	vec_sld( v0a, v0a, 4 );
		return						vec_mergel( v2a, v1a );
	}

	else if (i == SWZ_MASK(0,0,1,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_mergeh( v1a, v0a );
		return						vec_sld( v2a, v1a, 4 );
	}

	else if (i == SWZ_MASK(1,0,1,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		vec_float4	v2a			=	vec_splat( v0a, 3 );
		vec_float4	v3a			=	vec_sld( v1a, v0a, 8 );
		return						vec_sld( v3a, v2a, 4 );
	}

	else if (i == SWZ_MASK(2,0,1,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_sld( v1a, v0a, 8 );
		return						vec_sld( v2a, v1a, 4 );
	}

	else if (i == SWZ_MASK(3,0,1,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		vec_float4	v2a			=	vec_splat( v0a, 3 );
		return						vec_sld( v1a, v2a, 4 );
	}

	else if (i == SWZ_MASK(0,1,1,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_mergeh( v0a, v0a );
		return						vec_sld( v2a, v1a, 4 );
	}

	else if (i == SWZ_MASK(1,1,1,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_splat( v0a, 1 );
		return						vec_sld( v2a, v1a, 4 );
	}

	else if (i == SWZ_MASK(2,1,1,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_splat( v0a, 1 );
		vec_float4	v3a			=	vec_sld( v2a, v1a, 8 );
		return						vec_sld( v1a, v3a, 12 );
	}

	else if (i == SWZ_MASK(3,1,1,3))
	{
		vec_float4	v1a			=	vec_splat( v0a, 1 );
		vec_float4	v2a			=	vec_splat( v0a, 3 );
		vec_float4	v3a			=	vec_sld( v0a, v1a, 8 );
		return						vec_sld( v3a, v2a, 4 );
	}

	else if (i == SWZ_MASK(0,2,1,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		return						vec_mergel( v1a, v0a );
	}

	else if (i == SWZ_MASK(1,2,1,3))
	{
		vec_float4	v1a			=	vec_splat( v0a, 1 );
		return						vec_mergel( v1a, v0a );
	}

	else if (i == SWZ_MASK(2,2,1,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_mergel( v0a, v1a );
		return						vec_sld( v1a, v2a, 12 );
	}

	else if (i == SWZ_MASK(3,2,1,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		vec_float4	v2a			=	vec_mergel( v0a, v1a );
		return						vec_sld( v0a, v2a, 12 );
	}

	else if (i == SWZ_MASK(0,3,1,3))
	{
		vec_float4	v1a			=	vec_splat( v0a, 3 );
		return						vec_mergeh( v0a, v1a );
	}

	else if (i == SWZ_MASK(1,3,1,3))
	{
		vec_float4	v1a			=	vec_splat( v0a, 1 );
		vec_float4	v2a			=	vec_splat( v0a, 3 );
		return						vec_mergel( v1a, v2a );
	}

	else if (i == SWZ_MASK(2,3,1,3))
	{
		vec_float4	v1a			=	vec_mergel( v0a, v0a );
		vec_float4	v2a			=	vec_mergeh( v1a, v0a );
		return						vec_mergel( v2a, v1a );
	}

	else if (i == SWZ_MASK(3,3,1,3))
	{
		vec_float4	v1a			=	vec_splat( v0a, 3 );
		vec_float4	v2a			=	vec_mergeh( v1a, v0a );
		return						vec_mergel( v2a, v1a );
	}

	else if (i == SWZ_MASK(0,0,2,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		vec_float4	v2a			=	vec_splat( v0a, 0 );
		return						vec_sld( v2a, v1a, 8 );
	}

	else if (i == SWZ_MASK(1,0,2,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		vec_float4	v2a			=	vec_sld( v1a, v0a, 4 );
		return						vec_sld( v2a, v1a, 8 );
	}

	else if (i == SWZ_MASK(2,0,2,3))
	{
		vec_float4	v1a			=	vec_splat( v0a, 2 );
		vec_float4	v2a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v3a			=	vec_mergel( v2a, v1a );
		return						vec_sld( v3a, v3a, 4 );
	}

	else if (i == SWZ_MASK(3,0,2,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_mergel( v0a, v1a );
		return						vec_sld( v1a, v2a, 8 );
	}

	else if (i == SWZ_MASK(1,1,2,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_splat( v0a, 1 );
		return						vec_sld( v2a, v1a, 12 );
	}

	else if (i == SWZ_MASK(2,1,2,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_splat( v0a, 2 );
		return						vec_sld( v2a, v1a, 12 );
	}

	else if (i == SWZ_MASK(3,1,2,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		return						vec_sld( v0a, v1a, 12 );
	}

	else if (i == SWZ_MASK(0,2,2,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_mergel( v0a, v0a );
		return						vec_sld( v1a, v2a, 12 );
	}

	else if (i == SWZ_MASK(1,2,2,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 12 );
		return						vec_mergel( v1a, v0a );
	}

	else if (i == SWZ_MASK(2,2,2,3))
	{
		vec_float4	v1a			=	vec_splat( v0a, 2 );
		return						vec_mergel( v1a, v0a );
	}

	else if (i == SWZ_MASK(3,2,2,3))
	{
		vec_float4	v1a			=	vec_mergel( v0a, v0a );
		return						vec_sld( v0a, v1a, 12 );
	}

	else if (i == SWZ_MASK(0,3,2,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		vec_float4	v2a			=	vec_sld( v0a, v1a, 12 );
		return						vec_sld( v2a, v2a, 12 );
	}

	else if (i == SWZ_MASK(1,3,2,3))
	{
		vec_float4	v1a			=	vec_splat( v0a, 3 );
		vec_float4	v2a			=	vec_sld( v0a, v0a, 4 );
		return						vec_mergeh( v2a, v1a );
	}

	else if (i == SWZ_MASK(2,3,2,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		return						vec_sld( v0a, v1a, 8 );
	}

	else if (i == SWZ_MASK(3,3,2,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		vec_float4	v2a			=	vec_splat( v0a, 3 );
		return						vec_sld( v2a, v1a, 8 );
	}

	else if (i == SWZ_MASK(0,0,3,3))
	{
		vec_float4	v1a			=	vec_splat( v0a, 0 );
		vec_float4	v2a			=	vec_splat( v0a, 3 );
		return						vec_sld( v1a, v2a, 8 );
	}

	else if (i == SWZ_MASK(1,0,3,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		vec_float4	v2a			=	vec_splat( v0a, 3 );
		vec_float4	v3a			=	vec_sld( v1a, v0a, 4 );
		return						vec_sld( v3a, v2a, 8 );
	}

	else if (i == SWZ_MASK(2,0,3,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_mergel( v1a, v0a );
		return						vec_sld( v2a, v2a, 4 );
	}

	else if (i == SWZ_MASK(3,0,3,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_splat( v0a, 3 );
		return						vec_sld( v1a, v2a, 8 );
	}

	else if (i == SWZ_MASK(0,1,3,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		vec_float4	v2a			=	vec_splat( v0a, 3 );
		return						vec_sld( v1a, v2a, 8 );
	}

	else if (i == SWZ_MASK(1,1,3,3))
	{
		vec_float4	v1a			=	vec_splat( v0a, 1 );
		vec_float4	v2a			=	vec_splat( v0a, 3 );
		return						vec_sld( v1a, v2a, 8 );
	}

	else if (i == SWZ_MASK(2,1,3,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		vec_float4	v2a			=	vec_mergel( v1a, v0a );
		return						vec_mergel( v0a, v2a );
	}

	else if (i == SWZ_MASK(3,1,3,3))
	{
		vec_float4	v1a			=	vec_splat( v0a, 3 );
		vec_float4	v2a			=	vec_mergeh( v1a, v0a );
		return						vec_sld( v2a, v1a, 8 );
	}

	else if (i == SWZ_MASK(0,2,3,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_mergel( v0a, v1a );
		return						vec_sld( v1a, v2a, 12 );
	}

	else if (i == SWZ_MASK(1,2,3,3))
	{
		vec_float4	v1a			=	vec_splat( v0a, 3 );
		return						vec_sld( v0a, v1a, 4 );
	}

	else if (i == SWZ_MASK(2,2,3,3))
	{
		return						vec_mergel( v0a, v0a );
	}

	else if (i == SWZ_MASK(3,2,3,3))
	{
		vec_float4	v1a			=	vec_splat( v0a, 3 );
		return						vec_mergel( v1a, v0a );
	}

	else if (i == SWZ_MASK(0,3,3,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 4 );
		vec_float4	v2a			=	vec_splat( v0a, 3 );
		return						vec_sld( v1a, v2a, 12 );
	}

	else if (i == SWZ_MASK(1,3,3,3))
	{
		vec_float4	v1a			=	vec_sld( v0a, v0a, 8 );
		vec_float4	v2a			=	vec_splat( v0a, 3 );
		return						vec_sld( v1a, v2a, 12 );
	}
	else if (i == SWZ_MASK(2,3,3,3))
	{
		vec_float4	v1a			=	vec_splat( v0a, 3 );
		return						vec_sld( v0a, v1a, 8 );
	}
	else if (i == SWZ_MASK(3,3,3,3))
	{
		return						vec_splat( v0a, 3 );
	}
	else if (i == SWZ_MASK(0,1,2,3))
	{
		return v0a;
	}
	return	v0a;
#endif
}

#else

MECANIM_FORCE_INLINE vec4f __vpermwi3(vec4f v0a, const unsigned int mask)
{

	unsigned int w = (mask >> 0  ) & 0xff;
	unsigned int z = (mask >> 8  ) & 0xff;
	unsigned int y = (mask >> 16 ) & 0xff;
	unsigned int x = (mask >> 24 ) & 0xff;

	vec_uint4 wmask = V4BuildPermuteMask(x,y,z,w);
	vec_float4 v = vec_perm(v0a, v0a, (vec_uchar16)wmask);
	return v;

}
#endif



MECANIM_FORCE_INLINE vec4f Vmove(vec4f l, vec4f r)
 {
	 static const vec_uint4 vu32 = {0xFFFFFFFF,0,0,0};
	 vec_float4 v = __vsel(l, r, vu32);
	 return v;
 }

template<int SWZ> struct Vswizzle 
{
	static inline vec4f rhs(vec4f r)
	{
		return Vpermute(r, SWZ);
	}

	static inline vec4f lhs(vec4f l, vec4f r)
	{
		vec4f m = Vmove(Vswizzle<SWZ>::rhs(l), r);
		vec4f v = Vswizzle<SWZ>::rhs(m);
		return v;
	}
};
template<> struct Vswizzle<kXYZW> 
{
	static inline vec4f rhs(vec4f r)
	{
		return r;
	}
	static inline vec4f lhs(vec4f l, vec4f r)
	{
		return Vmove(l, r);
	}
};

MECANIM_FORCE_INLINE float Vstoresf(vec4f r)
{
	float f; 
	vec_ste(__vspltw(r, 0), 0, &f); 
	return f;
}

MECANIM_FORCE_INLINE bool Vstoresb(vec4b r)
{
	r=__vspltw(r, 0);
	return !vec_all_eq((vec4f)r, Vzero());
}

// Aligned store
#define Vstorepf(v, base, offset) __stvx((v), (base), (offset))

MECANIM_FORCE_INLINE void Vstorepb(vec4b v, bool* r)
{
	union {
		vec4b v;
		int	   i[4];
	} a; a.v = v;
	r[0] = a.i[0] != 0;
	r[1] = a.i[1] != 0;
	r[2] = a.i[2] != 0;
	r[3] = a.i[3] != 0;
}	

MECANIM_FORCE_INLINE vec4f Vloadsf(float s)
{
	vec4f v = {s,s,s,s};
	return v; 
}

MECANIM_FORCE_INLINE vec4b Vloadsb(bool s) 
{
	vec4b vTrue = (vec4b)vec_splat_u32(0xffffffff);
	
	return s ? vTrue : (vec4b)Vzero(); 
}
	
MECANIM_FORCE_INLINE vec4f Vload4sf(float x, float y, float z, float w)
{
	vec4f v = {x,y,z,w};
	return v; 
}

static MECANIM_FORCE_INLINE vec4b Vload4sb(bool x, bool y, bool z, bool w) 
{
	static const unsigned int false_true[2] = {0,~0};

	vec4b v = (vec4b)(vec_uint4) { false_true[x], false_true[y], false_true[z], false_true[w] };
	return v; 
}

#define Vloadpf(v, offset) __lvx((v), (offset))

#define Vadd(l, r) __vaddfp((l), (r))

#define Vsub( l,  r) __vsubfp((l), (r))

#define Vmul( l,  r) __vmulfp((l), (r))

MECANIM_FORCE_INLINE vec4f Vrcp(vec4f r)
{
	// This function does two iterations of Newton's method! (taken from XMVector)
	vec_float4 Reciprocal = vec_re(r);

	// First refinement iteration (Newton-Raphson) for 1.0 / x
	//     y0 = reciprocal_estimate(x)
	//     y1 = y0 + y0 * (1.0 - x * y0)

	vec_float4 vone = Vone();
	vec_float4 Scale = vec_nmsub(r, Reciprocal, vone);
	vec_float4 Result = vec_madd(Reciprocal, Scale, Reciprocal);

	// Second refinement iteration
	//     y2 = y1 + y1 * (1.0 - x * y1)

	Scale = vec_nmsub(r, Result, vone);
	vec_bint4 Refine = vec_cmpeq(Result, Result);
	Result = vec_madd(Result, Scale, Result);
	return (vec_sel(Reciprocal, Result, Refine));
}

MECANIM_FORCE_INLINE vec4f Vdiv(vec4f l, vec4f r)
{
	// This function does two iterations of Newton's method!
	return Vmul(l, Vrcp(r));
}

#define Vmadd( a,  b,  c) __vmaddfp((a), (b), (c))

#define Vmsub( a,  b,  c) Vneg(__vnmsubfp((a), (b), (c)))

#define Vneg(r) __vxor( (r), __vsignedzero)


// vector sgn: return -1, 1
#define Vsgn(r) __vor(Vone(), __vand(__vsignedzero, (r) ))

// vector sgn: return -1, 0, 1
static MECANIM_FORCE_INLINE vec4f Vsign(vec4f r)
{
	vec4f c = (vec4f)__vcmpeqfp(r, Vzero());
	return __vor( __vand(vec_nor(c,c), Vone()), __vand(__vsignedzero, r ));
}

#define Vinc(r) Vadd( (r), Vone())
#define Vdec(r) Vsub( (r), Vone())
#define Vabs(r) __vandc((r), __vsignedzero)
#define Vmax( l,  r) __vmaxfp((l), (r))
#define Vmin( l,  r) __vminfp((l), (r))

MECANIM_FORCE_INLINE vec4f Vlargest(vec4f r)
{
	r = Vmax(r, Vswizzle<kYZWX>::rhs(r));
	r = Vmax(r, Vswizzle<kZWXY>::rhs(r));
	return r;
}

MECANIM_FORCE_INLINE vec4f Vsmallest(vec4f r)
{
	r = Vmin(r, Vswizzle<kYZWX>::rhs(r));
	r = Vmin(r, Vswizzle<kZWXY>::rhs(r));
	return r;
}

MECANIM_FORCE_INLINE vec4f Vsum(vec4f r)
{
	r = Vadd(r, Vswizzle<kYZWX>::rhs(r) );
	r = Vadd(r, Vswizzle<kZWXY>::rhs(r) );
	return  Vswizzle<kXXXX>::rhs(r);
}

#define Vdot( l,  r) __vmsum4fp( (l), (r) )

MECANIM_FORCE_INLINE vec4f Vrsqrt(vec4f r)
{	
	static const vec4f three = {3.f,3.f,3.f,3.f};
	static const vec4f a = {0.5f,0.5f,0.5f,0.5f};

	vec4f const e = __vrsqrtefp(r);
	return Vmul( Vmul(e, Vsub(three, Vmul( Vmul(e,e),r))), a);
}


#define Vsqrt(r) __vsel( Vmul(r, Vrsqrt(r)), Vzero(), __vcmpeqfp(r, Vzero()))

#define Vcombine(x,y,z,w) __vmrghw(__vmrghw((x), (z)), __vmrghw((y), (w)))

// Vector comparison	
#define Vcmpeq( a,  b) (vec_bint4)__vcmpeqfp((a), (b))

MECANIM_FORCE_INLINE vec4b Vcmpneq( vec4f a,  vec4f b)
{
	vec4f c = (vec4f)Vcmpeq(a, b); 	
	return (vec4b)__vnor(c, c);
}

#define Vcmpgt( a,  b) (vec_bint4)__vcmpgtfp((a), (b))
#define Vcmpge( a,  b) (vec_bint4)__vcmpgefp((a), (b))
#define Vcmplt( a,  b) (vec_bint4)__vcmpgtfp((b), (a))
#define Vcmple( a,  b) (vec_bint4)__vcmpgefp((b), (a))

#define Vsel( c, a, b) __vxor(b, __vand(__vxor(a, b), c))
	
//	vector logics 
#define Vnot(r) __vnor( (r), (r) )
#define Vxnor( a,  b) Vnot(__vxor((a), (b)))
#define Vxor( a,  b) __vxor((a), (b))
#define Vand( a,  b) __vand((a), (b))
#define Vor( a,  b) __vor((a), (b))

MECANIM_FORCE_INLINE bool Vall(vec4b a)
{
	return vec_all_ne((vec4f)a, Vzero());
}

MECANIM_FORCE_INLINE bool Vany(vec4b a)
{
	// Not all words equal to 0
	return vec_any_ne((vec4f)a, Vzero());
}

#endif
