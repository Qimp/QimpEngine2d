#ifndef SIMD_SSE_H
#define SIMD_SSE_H

#include <xmmintrin.h>
#include <emmintrin.h>

typedef __m128	vec4f;	// vector 4 packed
typedef __m128	vec4fs;	// vector 4 scalar
typedef __m128	vec4b;	// vector 4 bool packed
typedef __m128	vec4bs;	// vector 4 bool scalar

#define SWZ_MASK(x, y, z, w)	_MM_SHUFFLE(w,z,y,x)

#define cvec4f(name, x,y,z,w) static const vec4f name = {x,y,z,w}
#define cvec4b(name, x,y,z,w) static const vec4b name = {x,y,z,w}
#define cvec4fs(name, s) static const vec4f name = {s,s,s,s}

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

#define Vzero() _mm_setzero_ps()

#define Vone() _mm_set1_ps(1.f)

#define Vpermute(v, mask) _mm_shuffle_ps( (v), (v), (mask) )

#define Vmove(l, r) _mm_move_ss( (l), (r) )


template<int SWZ> struct Vswizzle 
{
	static MECANIM_FORCE_INLINE vec4f rhs(vec4f r)
	{
		return Vpermute(r, SWZ);
	}

	static MECANIM_FORCE_INLINE vec4f lhs(vec4f l, vec4f r)
	{
		vec4f m = Vmove(Vswizzle<SWZ>::rhs(l), r);
		return Vswizzle<SWZ>::rhs(m);
	}
};

template<> struct Vswizzle<kXYZW> 
{
	static MECANIM_FORCE_INLINE vec4f rhs(vec4f r)
	{
		return r;
	}
	static MECANIM_FORCE_INLINE vec4f lhs(vec4f l, vec4f r)
	{
		return Vmove(l, r);
	}
};

static MECANIM_FORCE_INLINE float Vstoresf(vec4f r)
{
	float f; _mm_store_ss(&f, r); return f;
}

#define Vstoresb(r) ( _mm_movemask_ps((r)) & 0x1<<0) != 0

// Aligned store
#define Vstorepf(v, base, offset) _mm_store_ps((base)+(offset), (v))

static MECANIM_FORCE_INLINE void Vstorepb(vec4f v, bool* r)
{
	r[0] = ( _mm_movemask_ps(v) & 0x1<<0) != 0;
	r[1] = ( _mm_movemask_ps(v) & 0x1<<1) != 0;
	r[2] = ( _mm_movemask_ps(v) & 0x1<<2) != 0;
	r[3] = ( _mm_movemask_ps(v) & 0x1<<3) != 0;
}

#define Vloadsf(s) _mm_set1_ps(s)

static MECANIM_FORCE_INLINE vec4f Vloadsb(bool s) 
{
	union {
		int		b[4];
		vec4f		v;
	} static const false_true[2] = {
		{0,0,0,0},
		{~0,~0,~0,~0}
	};

	return false_true[s].v; 
}
	
#define Vload4sf(x, y, z, w) _mm_set_ps(w, z, y, x)

static MECANIM_FORCE_INLINE vec4f Vload4sb(bool x, bool y, bool z, bool w) 
{
	union {
		int		b;
		float	f;
	} static const false_true[2] = {
		0,~0
	};

	return _mm_set_ps(false_true[w].f, false_true[z].f, false_true[y].f, false_true[x].f); 
}

#define Vloadpf(v, offset) _mm_load_ps( (v)+(offset))

#define Vadd(l, r) _mm_add_ps((l), (r))

#define Vsub( l,  r) _mm_sub_ps((l), (r))

#define Vmul( l,  r) _mm_mul_ps((l), (r))

#define Vdiv( l,  r) _mm_div_ps((l), (r))

#define Vmadd( a,  b,  c) _mm_add_ps(_mm_mul_ps((a), (b)), (c))

#define Vmsub( a,  b,  c) _mm_sub_ps(_mm_mul_ps((a), (b)), (c))


static MECANIM_FORCE_INLINE vec4f Vneg(vec4f r)
{
	static const vec4f sign_constant = {-0.f,-0.f,-0.f,-0.f};
	return _mm_xor_ps( (r), sign_constant);
}

// vector sgn: return -1, 1
static MECANIM_FORCE_INLINE vec4f Vsgn(vec4f r)
{
	static const vec4f sign_constant = {-0.f,-0.f,-0.f,-0.f};	
	return _mm_or_ps(Vone(), _mm_and_ps(sign_constant, (r) ));
}

// vector sgn: return -1, 0, 1
static MECANIM_FORCE_INLINE vec4f Vsign(vec4f r)
{
	static const vec4f sign_constant = {-0.f,-0.f,-0.f,-0.f};	
	return _mm_or_ps( _mm_and_ps( _mm_cmpneq_ps(r, Vzero()), Vone()), _mm_and_ps(sign_constant, r ));
}

#define Vinc(r) Vadd( (r), Vone())
#define Vdec(r) Vsub( (r), Vone())

static MECANIM_FORCE_INLINE vec4f Vabs(vec4f r)
{
	static const vec4f sign_constant = {-0.f,-0.f,-0.f,-0.f};	
	return _mm_andnot_ps(sign_constant, (r));
}

#define Vmax( l,  r) _mm_max_ps((l), (r))
#define Vmin( l,  r) _mm_min_ps((l), (r))

static MECANIM_FORCE_INLINE vec4fs Vlargest(vec4f r)
{
	r = Vmax(r, Vswizzle<kYZWX>::rhs(r));
	r = Vmax(r, Vswizzle<kZWXY>::rhs(r));
	return r;
}

static MECANIM_FORCE_INLINE vec4fs Vsmallest(vec4f r)
{
	r = Vmin(r, Vswizzle<kYZWX>::rhs(r));
	r = Vmin(r, Vswizzle<kZWXY>::rhs(r));
	return r;
}

static MECANIM_FORCE_INLINE vec4fs Vsum(vec4f r)
{
	r = Vadd(r, Vswizzle<kYZWX>::rhs(r) );
	r = Vadd(r, Vswizzle<kZWXY>::rhs(r) );
	return  Vswizzle<kXXXX>::rhs(r);
}

#define Vdot( l,  r) Vsum( Vmul((l), (r)) )
#define Vsqrt(r) _mm_sqrt_ps((r))

static MECANIM_FORCE_INLINE vec4f Vrsqrt(vec4f r)
{
	vec4f const e = _mm_rsqrt_ps(r);
	return Vmul(Vmul(e, Vsub(_mm_set1_ps(3.0f), Vmul(Vmul(e,e),r))), _mm_set1_ps(.5f));
}

static MECANIM_FORCE_INLINE vec4f Vrcp(vec4f r)
{
	vec4f e = _mm_rcp_ps( r );
	return Vsub( Vadd(e, e), Vmul(r, Vmul(e, e)));
}

#define Vcombine(x,y,z,w) _mm_movelh_ps(_mm_unpacklo_ps( (x), (y) ), _mm_unpacklo_ps((z), (w)))

// Vector comparison	
#define Vcmpeq( a,  b) _mm_cmpeq_ps((a), (b))
#define Vcmpneq( a,  b) _mm_cmpneq_ps((a), (b))
#define Vcmpgt( a,  b) _mm_cmpgt_ps((a), (b))
#define Vcmpge( a,  b) _mm_cmpge_ps((a), (b))
#define Vcmplt( a,  b) _mm_cmplt_ps((a), (b))
#define Vcmple( a,  b) _mm_cmple_ps((a), (b))

static MECANIM_FORCE_INLINE vec4f Vsel( vec4f c,  vec4f a,  vec4f b)
{
	return _mm_xor_ps(b, _mm_and_ps(_mm_xor_ps(a, b), c));
}
	
//	vector logics 
#define Vnot(r) _mm_cmpeq_ps( (r), Vzero() )
#define Vxnor( a,  b) Vnot(_mm_xor_ps((a), (b)))
#define Vxor( a,  b) _mm_xor_ps((a), (b))
#define Vand( a,  b) _mm_and_ps((a), (b))
#define Vor( a,  b) _mm_or_ps((a), (b))
#define Vall(a) (_mm_movemask_ps((a)) & 0xf) == 0xf
#define Vany( a) _mm_movemask_ps((a)) != 0

#endif
