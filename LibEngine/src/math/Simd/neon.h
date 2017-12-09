#ifndef SIMD_NEON_H
#define SIMD_NEON_H

#include <arm_neon.h>

typedef float32x4_t vec4f;
typedef float32x4_t vec4fs;
typedef uint32x4_t vec4b;
typedef uint32x4_t vec4bs;

#define SWZ_MASK(x, y, z, w)	(((w) << 6) | ((z) << 4) | ((y) << 2) | ((x)))
#define SWZ_X(MASK)				(((MASK) >> 0) & 3)
#define SWZ_Y(MASK)				(((MASK) >> 2) & 3)
#define SWZ_Z(MASK)				(((MASK) >> 4) & 3)
#define SWZ_W(MASK)				(((MASK) >> 6) & 3)

//VPERMWI_CONST(x, y, z, w)
#if PLATFORM_WINRT
#define cvec4f(name, x,y,z,w) static const vec4f name = Vload4sf(x, y, z, w)
#define cvec4b(name, x,y,z,w) static const vec4b name = Vload4sb(x, y, z, w)
#define cvec4fs(name, s) static const vec4fs name = Vloadsf(s)
#else
#define cvec4f(name, x,y,z,w) static const vec4f name = {(x),(y),(z),(w)}
#define cvec4b(name, x,y,z,w) static const vec4b name = {(x),(y),(z),(w)}
#define cvec4fs(name, s) static const vec4fs name = {(s),(s),(s),(s)}
#endif

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

#define Vzero() vdupq_n_f32(0.0f)
#define Vone() vdupq_n_f32(1.0f)

#define Vfalse() vdupq_n_u32(0)
#define Vtrue() vdupq_n_f32(0xFFFFFFFF)

//#define Vpermute(v, mask) v
//#define Vmove(l, r) vextq_f32(l, r, 0)
MECANIM_FORCE_INLINE vec4f Vmove(vec4f l, vec4f r)
{
	uint32x4_t sel = Vfalse();
	sel = vsetq_lane_u32(0xFFFFFFFF,sel,0);
	return vbslq_f32(sel, r, l);
}
template<int SWZ> struct Vswizzle
{
	static MECANIM_FORCE_INLINE vec4f rhs(vec4f r)
	{
		uint32_t lanes[4];
		uint32x4_t u = vreinterpretq_u32_f32(r);
		uint32x4_t result;

		lanes[0] = vgetq_lane_u32(u, 0);
		lanes[1] = vgetq_lane_u32(u, 1);
		lanes[2] = vgetq_lane_u32(u, 2);
		lanes[3] = vgetq_lane_u32(u, 3);

		result = vdupq_n_u32(lanes[SWZ_X(SWZ)]);
		result = vsetq_lane_u32(lanes[SWZ_Y(SWZ)], result, 1);
		result = vsetq_lane_u32(lanes[SWZ_Z(SWZ)], result, 2);
		result = vsetq_lane_u32(lanes[SWZ_W(SWZ)], result, 3);

		return vreinterpretq_f32_u32(result);
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
template<> struct Vswizzle<kXXXX>
{
	static MECANIM_FORCE_INLINE vec4f rhs(vec4f r)
	{
		return vdupq_lane_f32(vget_low_f32(r),0);
	}
	static MECANIM_FORCE_INLINE vec4f lhs(vec4f l, vec4f r)
	{
		vec4f m = Vmove(rhs(l), r);
		return rhs(m);
	}
};
template<> struct Vswizzle<kYYYY>
{
	static MECANIM_FORCE_INLINE vec4f rhs(vec4f r)
	{
		return vdupq_lane_f32(vget_low_f32(r),1);;
	}
	static MECANIM_FORCE_INLINE vec4f lhs(vec4f l, vec4f r)
	{
		vec4f m = Vmove(rhs(l), r);
		return rhs(m);
	}
};
template<> struct Vswizzle<kZZZZ>
{
	static MECANIM_FORCE_INLINE vec4f rhs(vec4f r)
	{
		return vdupq_lane_f32(vget_high_f32(r),0);
	}
	static MECANIM_FORCE_INLINE vec4f lhs(vec4f l, vec4f r)
	{
		vec4f m = Vmove(rhs(l), r);
		return rhs(m);
	}
};
template<> struct Vswizzle<kWWWW>
{
	static MECANIM_FORCE_INLINE vec4f rhs(vec4f r)
	{
		return vdupq_lane_f32(vget_high_f32(r),1);
	}
	static MECANIM_FORCE_INLINE vec4f lhs(vec4f l, vec4f r)
	{
		vec4f m = Vmove(rhs(l), r);
		return rhs(m);
	}
};

static MECANIM_FORCE_INLINE float Vstoresf(vec4f r)
{
	return vgetq_lane_f32(r, 0);
}

static MECANIM_FORCE_INLINE bool Vstoresb(vec4b r)
{
	return (vgetq_lane_u32(r, 0) > 0) ? true : false;
}

// Aligned store
#define Vstorepf(v, base, offset) vst1q_f32((float32_t*)((base)+(offset)),v);

static MECANIM_FORCE_INLINE void Vstorepb(const vec4b v, bool* r)
{
	uint32_t u;
    vst1q_lane_u32(&u, v, 0);
    r[0] = (u > 0) ? true : false;
    vst1q_lane_u32(&u, v, 1);
    r[1] = (u > 0) ? true : false;
    vst1q_lane_u32(&u, v, 2);
    r[2] = (u > 0) ? true : false;
    vst1q_lane_u32(&u, v, 3);
    r[3] = (u > 0) ? true : false;
}

static MECANIM_FORCE_INLINE vec4f Vloadsf(float s)
{
	return vmovq_n_f32(s);
}

static MECANIM_FORCE_INLINE vec4b Vloadsb(bool s)
{
	const uint32_t false_true[2] = { 0, 0xFFFFFFFF };
	return vdupq_n_u32(false_true[s ? 1 : 0]);
}

static MECANIM_FORCE_INLINE vec4f Vload4sf(float x, float y, float z, float w)
{
	float32x4_t result;
	result = vdupq_n_f32(x);
	result = vsetq_lane_f32(y, result, 1);
	result = vsetq_lane_f32(z, result, 2);
	result = vsetq_lane_f32(w, result, 3);
	return result;
}

static MECANIM_FORCE_INLINE vec4b Vload4sb(bool x, bool y, bool z, bool w) 
{
	const uint32_t val[4] =
	{
		x ? 0xffffffff : 0x00,
		y ? 0xffffffff : 0x00,
		z ? 0xffffffff : 0x00,
		w ? 0xffffffff : 0x00
	};

	return vld1q_u32(&val[0]);
}

static MECANIM_FORCE_INLINE vec4f Vloadpf(float const* buf, int offset)
{
	return vld1q_f32((float32_t const*)buf + offset);
}

#define Vadd(l, r) vaddq_f32(l, r)
#define Vsub(l, r) vsubq_f32(l, r)
#define Vmul(l, r) vmulq_f32(l, r)


// return a*b+c : be aware that vmlaq does a+b*c
#define Vmadd(a, b, c) vmlaq_f32(c, a, b)
// return a*b-c : be aware that vmlaq does a-b*c
#define Vmsub(a, b, c) Vneg(vmlsq_f32(c, a, b))

static MECANIM_FORCE_INLINE vec4f Vneg(vec4f r)
{
	uint32x4_t sign_constant = vdupq_n_u32(0x80000000);
	uint32x4_t negated = veorq_u32(vreinterpretq_u32_f32(r), sign_constant);
	return vreinterpretq_f32_u32(negated);
}

// vector sgn: return -1, 1
static MECANIM_FORCE_INLINE vec4f Vsgn(vec4f r)
{
	uint32x4_t sign_constant = vdupq_n_u32(0x80000000);
	uint32x4_t signs = vandq_u32(vreinterpretq_u32_f32(r), sign_constant);
	uint32x4_t ones = vdupq_n_u32 (0x3f800000);

	return vreinterpretq_f32_u32(vorrq_u32(signs,ones));
/*	float32x4_t ones = Vone();
	float32x4_t nones = Vneg(ones);
	uint32x4_t cmp = vcltq_f32(r,Vzero());
	return vbslq_f32(cmp,nones,ones);*/
}

// vector sgn: return -1, 0, 1
static MECANIM_FORCE_INLINE vec4f Vsign(vec4f r)
{
	uint32x4_t sign_constant = vdupq_n_u32(0x80000000);
	uint32x4_t signs = vandq_u32(vreinterpretq_u32_f32(r), sign_constant);
	uint32x4_t ones = vdupq_n_u32 (0x3f800000);

	return vreinterpretq_f32_u32(vorrq_u32( signs, vandq_u32( vmvnq_u32( vceqq_f32( r, Vzero())), ones)));
}

#define Vinc(r) Vadd( (r), Vone())
#define Vdec(r) Vsub( (r), Vone())

static MECANIM_FORCE_INLINE vec4f Vabs(vec4f r)
{
	return vabsq_f32(r);
}

#define Vmax( l,  r) vmaxq_f32(l, r)
#define Vmin( l,  r) vminq_f32(l, r)

static MECANIM_FORCE_INLINE vec4fs Vlargest(vec4f r)
{
	float32x2_t temp = vpmax_f32 ( vget_high_f32(r), vget_low_f32(r) );
	temp = vpmax_f32(temp, temp);
	return vcombine_f32(temp,temp);
}

static MECANIM_FORCE_INLINE vec4fs Vsmallest(vec4f r)
{
	float32x2_t temp = vpmin_f32 ( vget_high_f32(r), vget_low_f32(r) );
	temp = vpmin_f32(temp, temp);
	return vcombine_f32(temp,temp);
}

static MECANIM_FORCE_INLINE vec4fs Vsum(vec4f r)
{
	float32x2_t temp = vpadd_f32 ( vget_high_f32(r), vget_low_f32(r) );
	temp = vpadd_f32(temp, temp);
	return vcombine_f32(temp,temp);
}

#define Vdot( l,  r) Vsum( Vmul((l), (r)) )

#define Vsqrt(r) Vmul(r,Vrsqrt(r))

static MECANIM_FORCE_INLINE vec4f Vrcp(vec4f r)
{
	float32x4_t inv = vrecpeq_f32(r);
	float32x4_t step = vrecpsq_f32(r, inv);
	return vmulq_f32(step, inv);
}

static MECANIM_FORCE_INLINE vec4f Vdiv(const vec4f l, const vec4f r)
{
	return Vmul(l, Vrcp(r));
}

static MECANIM_FORCE_INLINE vec4f Vcombine(vec4f x, vec4f y, vec4f z, vec4f w)
{
	float32x2x2_t temp1 = vtrn_f32(vget_high_f32(x), vget_high_f32(y));
	float32x2x2_t temp2 = vtrn_f32(vget_high_f32(z), vget_high_f32(w));
	return vcombine_f32(temp1.val[0], temp2.val[0]);
}

// Vector comparison	
#define Vcmpeq( a,  b) vceqq_f32(a, b)
#define Vcmpneq( a,  b) Vnot(vceqq_f32(a, b))
#define Vcmpgt( a,  b) vcgtq_f32(a, b)
#define Vcmpge( a,  b) vcgeq_f32(a, b)
#define Vcmplt( a,  b) vcltq_f32(a, b)
#define Vcmple( a,  b) vcleq_f32(a, b)

static MECANIM_FORCE_INLINE vec4f Vsel(vec4b c,  vec4f a,  vec4f b)
{
	return vbslq_f32(c, a, b);
}

//	vector logics 
#define Vnot(r) vmvnq_u32(r)
#define Vxnor(a, b) Vnot(veorq_u32(a, b))
#define Vxor(a, b) veorq_u32(a, b)
#define Vand(a, b) vandq_u32(a, b)
#define Vor(a, b) vorrq_u32(a, b)

static MECANIM_FORCE_INLINE bool Vall(const vec4b a)
{
	uint32_t u[4];

	vst1q_lane_u32(&u[0], a, 0);
	vst1q_lane_u32(&u[1], a, 1);
	vst1q_lane_u32(&u[2], a, 2);
	vst1q_lane_u32(&u[3], a, 3);

	return (u[0] & u[1] & u[2] & u[3]);
};

static MECANIM_FORCE_INLINE bool Vany(const vec4b a)
{
	uint32_t u[4];

	vst1q_lane_u32(&u[0], a, 0);
	vst1q_lane_u32(&u[1], a, 1);
	vst1q_lane_u32(&u[2], a, 2);
	vst1q_lane_u32(&u[3], a, 3);

	return (u[0] | u[1] | u[2] | u[3]);
};


static MECANIM_FORCE_INLINE vec4f Vrsqrt(vec4f r)
{
	float32x4_t e = vrsqrteq_f32(r);
	float32x4_t s = vmulq_f32(e, r);
	float32x4_t v = vrsqrtsq_f32(s, e);
    
	e = vmulq_f32(e,v);
	s = vmulq_f32(e, r);
	v = vrsqrtsq_f32(s, e);
    
	return Vsel( Vcmpeq(r, Vzero()), Vzero(), vmulq_f32(e,v));
}


#endif
