#ifndef SIMD_XENON
#define SIMD_XENON

#include <vectorintrinsics.h>
#include <xnamath.h>

typedef __vector4	vec4f;	// vector 4 packed
typedef __vector4	vec4fs;	// vector 4 scalar
typedef __vector4	vec4b;	// vector 4 bool packed
typedef __vector4	vec4bs;	// vector 4 bool scalar

#define SWZ_MASK(x, y, z, w)	VPERMWI_CONST(x, y, z, w)

#define cvec4f(name, x,y,z,w) static const vec4f name = {(x),(y),(z),(w)}
#define cvec4b(name, x,y,z,w) static const vec4b name = {(x),(y),(z),(w)}
#define cvec4fs(name, s) static const vec4fs name = {(s),(s),(s),(s)}

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
#define Vone() __vupkd3d(__vspltisw(0), VPACK_D3DCOLOR)

#define Vpermute(v, mask) __vpermwi( (v), (mask) )

 MECANIM_FORCE_INLINE vec4f Vmove(vec4f l, vec4f r)
 {
	 static const XMVECTORU32 vu32 = {0xFFFFFFFF,0,0,0};
	 return __vsel(l, r, vu32.v);
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
		return Vswizzle<SWZ>::rhs(m);
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
	float f; __stvewx(__vspltw(r, 0), &f, 0); return f;
}

MECANIM_FORCE_INLINE bool Vstoresb(vec4f r)
{
	union {
		vec4f v;
		int	  i[4];
	} a; a.v = r;
	return a.i[0] != 0;
}

// Aligned store
#define Vstorepf(v, base, offset) __stvx((v), (base), (offset))

MECANIM_FORCE_INLINE void Vstorepb(vec4f v, bool* r)
{
	union {
		vec4f v;
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

MECANIM_FORCE_INLINE vec4f Vloadsb(bool s) 
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
	
MECANIM_FORCE_INLINE vec4f Vload4sf(float x, float y, float z, float w)
{
	vec4f v = {x,y,z,w};
	return v; 
}

static MECANIM_FORCE_INLINE vec4f Vload4sb(bool x, bool y, bool z, bool w) 
{
	union {
		int		b;
		float	f;
	} static const false_true[2] = {
		0,~0
	};

	vec4f v = {false_true[x].f,false_true[y].f,false_true[z].f,false_true[w].f};
	return v; 
}

#define Vloadpf(v, offset) __lvx( (v), (offset))

#define Vadd(l, r) __vaddfp((l), (r))

#define Vsub( l,  r) __vsubfp((l), (r))

#define Vmul( l,  r) __vmulfp((l), (r))

MECANIM_FORCE_INLINE vec4f Vrcp(vec4f r)
{
	// This function does two iterations of Newton's method!
	return XMVectorReciprocal(r);	
}

MECANIM_FORCE_INLINE vec4f Vdiv(vec4f l, vec4f r)
{
	// This function does two iterations of Newton's method!
	return XMVectorDivide(l, r);
}

#define Vmadd( a,  b,  c) __vmaddfp((a), (b), (c))

#define Vmsub( a,  b,  c) Vneg(__vnmsubfp((a), (b), (c)))

static const vec4f sign_constant = {-0.f,-0.f,-0.f,-0.f};

#define Vneg(r) __vxor( (r), sign_constant)


// vector sgn: return -1, 1
#define Vsgn(r) __vor(Vone(), __vand(sign_constant, (r) ))

// vector sgn: return -1, 0, 1
static MECANIM_FORCE_INLINE vec4f Vsign(vec4f r)
{
	vec4f c = __vcmpeqfp(r, Vzero());
	return __vor( __vand(__vnor(c,c), Vone()), __vand(sign_constant, r ));
}

#define Vinc(r) Vadd( (r), Vone())
#define Vdec(r) Vsub( (r), Vone())
#define Vabs(r) __vandc((r), sign_constant)
#define Vmax( l,  r) __vmaxfp((l), (r))
#define Vmin( l,  r) __vminfp((l), (r))

MECANIM_FORCE_INLINE vec4fs Vlargest(vec4f r)
{
	r = Vmax(r, Vswizzle<kYZWX>::rhs(r));
	r = Vmax(r, Vswizzle<kZWXY>::rhs(r));
	return r;
}

MECANIM_FORCE_INLINE vec4fs Vsmallest(vec4f r)
{
	r = Vmin(r, Vswizzle<kYZWX>::rhs(r));
	r = Vmin(r, Vswizzle<kZWXY>::rhs(r));
	return r;
}

MECANIM_FORCE_INLINE vec4fs Vsum(vec4f r)
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
#define Vcmpeq( a,  b) __vcmpeqfp((a), (b))

MECANIM_FORCE_INLINE vec4f Vcmpneq( vec4f a,  vec4f b)
{
	vec4f c = __vcmpeqfp(a, b); 	
	return __vnor(c, c);
}

#define Vcmpgt( a,  b) __vcmpgtfp((a), (b))
#define Vcmpge( a,  b) __vcmpgefp((a), (b))
#define Vcmplt( a,  b) __vcmpgtfp((b), (a))
#define Vcmple( a,  b) __vcmpgefp((b), (a))

#define Vsel( c, a, b) __vxor(b, __vand(__vxor(a, b), c))
	
//	vector logics 
#define Vnot(r) __vnor( (r), (r) )
#define Vxnor( a,  b) Vnot(__vxor((a), (b)))
#define Vxor( a,  b) __vxor((a), (b))
#define Vand( a,  b) __vand((a), (b))
#define Vor( a,  b) __vor((a), (b))

MECANIM_FORCE_INLINE bool Vall(vec4b a)
{
	// All words equal
	static const XMVECTORU32 vu32 = {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};
	const int All = 0x00000080;
	unsigned int compareResult;
	__vcmpequwR(a, vu32.v, &compareResult);
	return (compareResult & All) == All;
}

MECANIM_FORCE_INLINE bool Vany(vec4b a)
{
	// Not all words equal to 0
	const int All = 0x00000080;
	unsigned int compareResult;
	__vcmpequwR(a, Vzero(), &compareResult);
	return (compareResult & All) == 0;
}

#endif
