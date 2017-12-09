#ifndef SIMD_FPU_H
#define SIMD_FPU_H

// vector 4 packed 
struct vec4f
{
	vec4f(float s = 0.0f){m128_f32[0] = m128_f32[1] = m128_f32[2] = m128_f32[3]= s;}
	vec4f(float x, float y, float z, float w){m128_f32[0] = x; m128_f32[1] = y; m128_f32[2] = z; m128_f32[3]= w;}

	float m128_f32[4];
};
typedef vec4f 		vec4fs;	// vector 4 scalar

// vector 4 bool packed
struct vec4b
{
	vec4b(bool s = false){m128_f32[0] = m128_f32[1] = m128_f32[2] = m128_f32[3]= s;}
	vec4b(bool x, bool y, bool z, bool w){m128_f32[0] = x; m128_f32[1] = y; m128_f32[2] = z; m128_f32[3]= w;}

	bool m128_f32[4];
};

typedef vec4b		vec4bs; // vector 4 bool scalar

#define cvec4f(name, x,y,z,w) static const vec4f name((x),(y),(z),(w))
#define cvec4b(name, x,y,z,w) static const vec4b name((x),(y),(z),(w))
#define cvec4fs(name, s)      static const vec4fs name(s)


#define SWZ_MASK(x, y, z, w)	(((w) << 6) | ((z) << 4) | ((y) << 2) | ((x)))
#define SWZ_X(MASK)				(((MASK) >> 0) & 3)
#define SWZ_Y(MASK)				(((MASK) >> 2) & 3)
#define SWZ_Z(MASK)				(((MASK) >> 4) & 3)
#define SWZ_W(MASK)				(((MASK) >> 6) & 3)
#define SWZ_I(MASK, I)			(((MASK) >> (I*2)) & 3)

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

#define Vzero() vec4f(0.f)
#define Vone() vec4f(1.f)
	
static MECANIM_FORCE_INLINE vec4f Vpermute(vec4f v, int mask) 
{
	return vec4f(v.m128_f32[SWZ_X(mask)], v.m128_f32[SWZ_Y(mask)], v.m128_f32[SWZ_Z(mask)], v.m128_f32[SWZ_W(mask)]);
}

#define Vmove(l, r) vec4f(r.m128_f32[0], l.m128_f32[1], l.m128_f32[2], l.m128_f32[3])

// This template is part of the back-end because some instruction set support some swizzle operation that could be specialized, like xbox vmx rotate instruction that is use in dot product
template<int SWZ> struct Vswizzle 
{
	static MECANIM_FORCE_INLINE vec4f rhs(vec4f r)
	{
		return Vpermute(r, SWZ);
	}

	static MECANIM_FORCE_INLINE vec4f lhs(vec4f l, vec4f r)
	{
		return Vswizzle<SWZ>::rhs(Vmove(Vswizzle<SWZ>::rhs(l), r));
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


// Aligned store, store vector at address base as 4 float
#define Vstorepf(v, base, offset) memcpy( (base)+(offset), &v.m128_f32[0], sizeof(v))

// Return component x as a float
#define Vstoresf(r) r.m128_f32[0]

// Return component x as a bool
#define Vstoresb(r) r.m128_f32[0]		

// Aligned store, store vector at address base as 4 bool 
static MECANIM_FORCE_INLINE void Vstorepb(vec4b v, bool* r)
{
	r[0] = v.m128_f32[0];
	r[1] = v.m128_f32[1];
	r[2] = v.m128_f32[2];
	r[3] = v.m128_f32[3];
}

// Aligned load, load 4 float at address v in vector register
static MECANIM_FORCE_INLINE vec4f Vloadpf(float const* v, int offset)
{
	float const* p = v+offset;
	return vec4f( p[0], p[1], p[2], p[3] );
}

// Load float value in vector register and replicate value in all component
#define Vloadsf(s) vec4f(s)

// Load bool value in vector register and replicate value in all component
#define Vloadsb(s) vec4bs(s)

// Load 4 float value in vector register
#define Vload4sf(x, y, z, w) vec4f(x,y,z,w)

// Load 4 bool value in vector register
#define Vload4sb(x, y, z, w) vec4b(x,y,z,w)

static MECANIM_FORCE_INLINE vec4f Vadd(vec4f l, vec4f r)
{
	return vec4f(l.m128_f32[0]+r.m128_f32[0], l.m128_f32[1]+r.m128_f32[1], l.m128_f32[2]+r.m128_f32[2], l.m128_f32[3]+r.m128_f32[3]);
}

static MECANIM_FORCE_INLINE vec4f Vsub(vec4f l, vec4f r)
{
	return vec4f(l.m128_f32[0]-r.m128_f32[0], l.m128_f32[1]-r.m128_f32[1], l.m128_f32[2]-r.m128_f32[2], l.m128_f32[3]-r.m128_f32[3]);
}

static MECANIM_FORCE_INLINE vec4f Vmul( vec4f l,  vec4f r)
{
	return vec4f(l.m128_f32[0]*r.m128_f32[0], l.m128_f32[1]*r.m128_f32[1], l.m128_f32[2]*r.m128_f32[2], l.m128_f32[3]*r.m128_f32[3]);
}

static MECANIM_FORCE_INLINE vec4f Vdiv( vec4f l,  vec4f r)
{
	return vec4f(l.m128_f32[0]/r.m128_f32[0], l.m128_f32[1]/r.m128_f32[1], l.m128_f32[2]/r.m128_f32[2], l.m128_f32[3]/r.m128_f32[3]);
}

#define Vmadd( a,  b,  c) vec4f(a.m128_f32[0]*b.m128_f32[0] + c.m128_f32[0], a.m128_f32[1]*b.m128_f32[1] + c.m128_f32[1], a.m128_f32[2]*b.m128_f32[2] + c.m128_f32[2], a.m128_f32[3]*b.m128_f32[3] + c.m128_f32[3])
#define Vmsub( a,  b,  c) vec4f(a.m128_f32[0]*b.m128_f32[0] - c.m128_f32[0], a.m128_f32[1]*b.m128_f32[1] - c.m128_f32[1], a.m128_f32[2]*b.m128_f32[2] - c.m128_f32[2], a.m128_f32[3]*b.m128_f32[3] - c.m128_f32[3])
#define Vneg(r)  vec4f(-r.m128_f32[0], -r.m128_f32[1], -r.m128_f32[2], -r.m128_f32[3])

// Vector sgn: return -1, 1
#define Vsgn(r) vec4f( r.m128_f32[0] < 0 ? -1.f : 1.f, r.m128_f32[1] < 0 ? -1.f : 1.f, r.m128_f32[2] < 0 ? -1.f : 1.f, r.m128_f32[3] < 0 ? -1.f : 1.f)

// Vector sgn: return -1, 0, 1
static MECANIM_FORCE_INLINE vec4f Vsign(vec4f r)
{
	return vec4f( r.m128_f32[0] < 0 ? -1.f : r.m128_f32[0] > 0 ? 1.f : 0.f,
		r.m128_f32[1] < 0 ? -1.f : r.m128_f32[1] > 0 ? 1.f : 0.f, 
		r.m128_f32[2] < 0 ? -1.f : r.m128_f32[2] > 0 ? 1.f : 0.f, 
		r.m128_f32[3] < 0 ? -1.f : r.m128_f32[3] > 0 ? 1.f : 0.f);
}

#define Vinc(r) Vadd( (r), Vone())
#define Vdec(r) Vsub( (r), Vone())
#define Vabs(r) vec4f( abs(r.m128_f32[0]), abs(r.m128_f32[1]), abs(r.m128_f32[2]), abs(r.m128_f32[3]))
#define Vmax( l,  r) vec4f( l.m128_f32[0] > r.m128_f32[0] ? l.m128_f32[0] : r.m128_f32[0], l.m128_f32[1] > r.m128_f32[1] ? l.m128_f32[1] : r.m128_f32[1], l.m128_f32[2] > r.m128_f32[2] ? l.m128_f32[2] : r.m128_f32[2], l.m128_f32[3] > r.m128_f32[3] ? l.m128_f32[3] : r.m128_f32[3])
#define Vmin( l,  r) vec4f( l.m128_f32[0] < r.m128_f32[0] ? l.m128_f32[0] : r.m128_f32[0], l.m128_f32[1] < r.m128_f32[1] ? l.m128_f32[1] : r.m128_f32[1], l.m128_f32[2] < r.m128_f32[2] ? l.m128_f32[2] : r.m128_f32[2], l.m128_f32[3] < r.m128_f32[3] ? l.m128_f32[3] : r.m128_f32[3])

// Return the largest of the 4 component
static MECANIM_FORCE_INLINE vec4fs Vlargest(vec4f r)
{
	r = Vmax(r, Vswizzle<kYZWX>::rhs(r));
	r = Vmax(r, Vswizzle<kZWXY>::rhs(r));
	return r.m128_f32[0];
}

// Return the smallest of the 4 component
static MECANIM_FORCE_INLINE vec4fs Vsmallest(vec4f r)
{
	r = Vmin(r, Vswizzle<kYZWX>::rhs(r));
	r = Vmin(r, Vswizzle<kZWXY>::rhs(r));
	return r.m128_f32[0];;
}

static MECANIM_FORCE_INLINE vec4fs Vsum(vec4f r)
{
	r = Vadd(r, Vswizzle<kYZWX>::rhs(r) );
	r = Vadd(r, Vswizzle<kZWXY>::rhs(r) );
	r = Vswizzle<kXXXX>::rhs(r);
	return r.m128_f32[0];
}

#define Vdot( l,  r) Vsum( Vmul((l), (r)) )
	
#define Vsqrt(r) vec4f( sqrt(r.m128_f32[0]), sqrt(r.m128_f32[1]), sqrt(r.m128_f32[2]), sqrt(r.m128_f32[3]))

static MECANIM_FORCE_INLINE vec4f Vrsqrt(vec4f r)
{
	vec4f const e = Vdiv(vec4f(1.f), Vsqrt(r));
	return Vmul(Vmul(e, Vsub(vec4f(3.0f), Vmul(Vmul(e,e),r))), vec4f(.5f));
}

static MECANIM_FORCE_INLINE vec4f Vrcp(vec4f r)
{
	return Vdiv(vec4f(1.f), r );
}


// Merge 4 vector low bytes
#define Vcombine(x,y,z,w) vec4f(x.m128_f32[0], y.m128_f32[0], z.m128_f32[0], w.m128_f32[0])

// Vector comparison	
#define Vcmpeq( a,  b) vec4b(a.m128_f32[0] == b.m128_f32[0], a.m128_f32[1] == b.m128_f32[1], a.m128_f32[2] == b.m128_f32[2], a.m128_f32[3] == b.m128_f32[3])
#define Vcmpneq( a,  b) vec4b(a.m128_f32[0] != b.m128_f32[0], a.m128_f32[1] != b.m128_f32[1], a.m128_f32[2] != b.m128_f32[2], a.m128_f32[3] != b.m128_f32[3])
#define Vcmpgt( a,  b) vec4b(a.m128_f32[0] > b.m128_f32[0], a.m128_f32[1] > b.m128_f32[1], a.m128_f32[2] > b.m128_f32[2], a.m128_f32[3] > b.m128_f32[3])
#define Vcmpge( a,  b) vec4b(a.m128_f32[0] >= b.m128_f32[0], a.m128_f32[1] >= b.m128_f32[1], a.m128_f32[2] >= b.m128_f32[2], a.m128_f32[3] >= b.m128_f32[3])
#define Vcmplt( a,  b) vec4b(a.m128_f32[0] < b.m128_f32[0], a.m128_f32[1] < b.m128_f32[1], a.m128_f32[2] < b.m128_f32[2], a.m128_f32[3] < b.m128_f32[3])
#define Vcmple( a,  b) vec4b(a.m128_f32[0] <= b.m128_f32[0], a.m128_f32[1] <= b.m128_f32[1], a.m128_f32[2] <= b.m128_f32[2], a.m128_f32[3] <= b.m128_f32[3])

#define Vsel( c,  a,  b) vec4f(c.m128_f32[0] ? a.m128_f32[0] : b.m128_f32[0], c.m128_f32[1] ? a.m128_f32[1] : b.m128_f32[1], c.m128_f32[2] ? a.m128_f32[2] : b.m128_f32[2], c.m128_f32[3] ? a.m128_f32[3] : b.m128_f32[3])

//	vector logics 
#define Vnot(r) vec4b(!r.m128_f32[0], !r.m128_f32[1], !r.m128_f32[2], !r.m128_f32[3])
#define Vxnor( a,  b) vec4b(!(a.m128_f32[0] ^ b.m128_f32[0]), !(a.m128_f32[1] ^ b.m128_f32[1]), !(a.m128_f32[2] ^ b.m128_f32[2]), !(a.m128_f32[3] ^ b.m128_f32[3]))
#define Vxor( a,  b) vec4b(a.m128_f32[0] ^ b.m128_f32[0], a.m128_f32[1] ^ b.m128_f32[1], a.m128_f32[2] ^ b.m128_f32[2], a.m128_f32[3] ^ b.m128_f32[3])
#define Vand( a,  b) vec4b(a.m128_f32[0] && b.m128_f32[0], a.m128_f32[1] && b.m128_f32[1], a.m128_f32[2] && b.m128_f32[2], a.m128_f32[3] && b.m128_f32[3])
#define Vor( a,  b) vec4b(a.m128_f32[0] || b.m128_f32[0], a.m128_f32[1] || b.m128_f32[1], a.m128_f32[2] || b.m128_f32[2], a.m128_f32[3] || b.m128_f32[3])
#define Vall(a) (a.m128_f32[0] && a.m128_f32[1] && a.m128_f32[2] && a.m128_f32[3])
#define Vany(a) (a.m128_f32[0] || a.m128_f32[1] || a.m128_f32[2] || a.m128_f32[3])

#endif // SIMD_FPU_H