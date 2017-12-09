#ifndef SIMD_MATH_H
#define SIMD_MATH_H

#include <cmath>

// Standard macro define in cmath
#ifndef M_EPSF
#define M_EPSF     1e-6f
#endif
#ifndef M_PIf
#define M_PIf       3.14159265358979323846f
#endif
#ifndef M_PI_2f
#define M_PI_2f     1.57079632679489661923f
#endif
#ifndef M_PI_4f
#define M_PI_4f     0.785398163397448309616f
#endif
#ifndef M_1_PIf
#define M_1_PIf     0.318309886183790671538f
#endif
#ifndef M_2_PIf
#define M_2_PIf     0.636619772367581343076f
#endif
#ifndef M_DEG_2_RADf
#define M_DEG_2_RADf  0.0174532925f
#endif
#ifndef M_RAD_2_DEGf
#define M_RAD_2_DEGf  57.295779513f
#endif

#include "Runtime/Math/Simd/float4.h"
#include "Runtime/Math/Simd/bool4.h"

QIMP_BEGIN_NAMESPACE

static inline bool all(bool4 const& r);
static inline bool any(bool4 const& r);
template <typename T> static inline T clamp(T const& v, T const& a, T const& b);
static inline float cond(bool c, float const& a, float const& b);
static inline int cond(bool c, int const& a, int const& b);
static inline float cubic(float const& a, float const& b, float const& c, float const& d, float const& u);
static inline float4 cubic(float4 const& a, float4 const& b, float4 const& c, float4 const& d, float4 const& u);
static inline float4 cross(float4 const& a, float4 const& b);
static inline float degrees(float const& deg);
static inline float4 degrees(float4 const& deg);
static inline float1 dot(float4 const& l, float4 const& r);
static inline float1 dot(float4 const& r);
static inline float1 length(float4 const& r);
static inline float lerp(float const& a, float const& b, float x);
static inline float1 lerp(float1 const& a, float1 const& b, float1 const& x);
static inline float4 lerp(float4 const& a, float4 const& b, float1 const& x);
static inline float4 lerp(float4 const& a, float4 const& b, float4 const& x);
template <typename T> static inline T maximum(T const& a, T const& b);
template <typename T> static inline T minimum(T const& a, T const& b);
static inline float1 maximum(float4 const& r);
static inline float1 minimum(float4 const& r);
static inline float4 normalize(float4 const& r);
static inline float radians(float const& deg);
static inline float4 radians(float4 const& deg);
static inline float4 rcp(float4 const& r);
static inline float1 rcp(float1 const& r);
static inline float4 rsqrt(float4 const& r );
static inline float saturate(float const& r);
static inline float1 saturate(float1 const& r);
static inline float4 saturate(float4 const& r);
static inline float4 scaleIdentity();
static inline float4 scaleWeight(float4 const& s, float1 const& w);
static inline void sincos(float4 const& u, float4& s, float4& c);
static inline void sincos(float1 const& u, float1& s, float1& c);
static inline void sincose(float4 const& u, float4& s, float4& c);
static inline void sincose(float1 const& u, float1& s, float1& c);
static inline float sgn(float const& r);
static inline float1 sgn(float1 const& r);
static inline float sign(float const& r);
static inline float4 sign(float4 const& r);
static inline float smoothstep( float min, float max, float x);
static inline float smoothpulse( float minmin, float minmax, float maxmin, float maxmax, float x);
static inline float1 sqrt(float1 const& r);
static inline float4 sqrt(float4 const& r);
static inline float1 sum(float4 const& r);	
static inline float4 vector(float4 const& v);
static inline float unrollangle(float angleRef, float angle);
static inline float4 load(float const* v);
static inline void store(float4 const& v, float* r);
static inline void store(bool4 const& v, bool* r);


static inline float abs(const float &r)
{
	return std::abs(r);
}

static inline float cos(float const& theta)
{
	return std::cos(theta);
}

static inline float rcp(const float &r)
{
	return 1.f/r;
}

static inline float rsqrt(const float& r)
{
	return 1.f/std::sqrt(r);
}

static inline float sin(float const& theta)
{
	return std::sin(theta);
}

static inline void sincos(float const& u, float& s, float& c)
{
	s = sin(u);
	c = cos(u);		
}

static inline float tan(float const& theta)
{
	return std::tan(theta);
}

static inline float atan(float const& t)
{
	return std::atan(t);
}

static inline  float sqrt(const float& r)
{
	return std::sqrt(r);
}	

static inline float modf(float x, float &ip)
{
	return std::modf(x, &ip); 
}

static inline float fmod(float x, float y)
{
	return std::fmod(x,y);
}

static inline  float pow(const float& x,const float& y)
{
	return std::pow(x,y);
}	

template <typename T> static inline vecexp4<vec4> abs(vecexp4<T> const& x)
{
	return vecexp4<vec4>( Vabs( x.eval() ) );
}

template <typename T> static inline vecexp1<T> abs(vecexp1<T> const& x)
{
	return vecexp1<T>( Vabs( x.eval() ) );
}

static inline float1 abs(float1 const& x)
{
	return float1( Vabs( x.eval() ) );
}

static inline bool all(bool4 const& r)
{
	return Vall(r.v); 
}
static inline bool any(bool4 const& r)
{
	return Vany(r.v); 
}

// x clamped to the range [a, b] as follows:
// Returns a if x is less than a.
// Returns b if x is greater than b.
// Returns x otherwise.
template <typename T> static inline T clamp(T const& v, T const& a, T const& b)
{
	return minimum(b, maximum(a, v));
}

template <typename L, typename R> static inline vecexp4<vec4> cond(bool4 const& c, vecexp4<L> const& l, vecexp4<R> const& r)
{	
	return vecexp4<vec4>( Vsel(c.v, l.eval(), r.eval()) ); 
}

template <typename L, typename R> static inline vecexp4<vec4> cond(bool1 const& c, vecexp4<L> const& l, vecexp4<R> const& r)
{
	return vecexp4<vec4>( Vsel(c.s, l.eval(), r.eval()) ); 
}

template <typename L, typename R> static inline vecexp1<vec4> cond(bool1 const& c, vecexp1<L> const& l, vecexp1<R> const& r)
{
	return vecexp4<vec4>( Vsel(c.s, l.eval(), r.eval()) ); 
}

static inline float cond(bool c, float const& a, float const& b)
{
	return c ? a : b;
}

static inline int cond(bool c, int const& a, int const& b)
{
	return int(b + (-int(c) & (a - b)));
}

static inline int cond(bool c, long int const& a, long int const& b)
{
	typedef long int long_int;
	return long_int(b + (-long_int(c) & (a - b)));
}

static inline unsigned long cond(bool c, unsigned long const& a, unsigned long const& b)
{
	return b + (- long(c) & (a - b));
}

static inline unsigned int cond(bool c, unsigned int const& a, unsigned int const& b)
{
	return b + (- int(c) & (a - b));
}

// De Casteljau construction of bezier
static inline float cubic(float const& a, float const& b, float const& c, float const& d, float const& u)
{
	const float ab = lerp(a,b,u);
	const float bc = lerp(b,c,u);
	const float cd = lerp(c,d,u);
	const float abc = lerp(ab,bc,u);
	const float bcd = lerp(bc,cd,u);
	return lerp(abc, bcd, u);
}

static inline float4 cubic(float4 const& a, float4 const& b, float4 const& c, float4 const& d, float4 const& u)
{
	const float4 ab = lerp(a,b,u);
	const float4 bc = lerp(b,c,u);
	const float4 cd = lerp(c,d,u);
	const float4 abc = lerp(ab,bc,u);
	const float4 bcd = lerp(bc,cd,u);
	return lerp(abc, bcd, u);
}

static inline float4 cross(float4 const& a, float4 const& b)
{
	return float4(a.yzxw()*b.zxyw() - a.zxyw()*b.yzxw());		
}

static inline float degrees(float const& rad)
{
	return M_RAD_2_DEGf*rad;
}

static inline float4 degrees(float4 const& rad)
{
	return float1(M_RAD_2_DEGf)*rad;
}

static inline float1 degrees(float1 const& rad)
{
	return float1(M_RAD_2_DEGf)*rad;
}

static inline float1 dot(float4 const& l, float4 const& r)
{
	return float1( Vdot(l.eval(), r.eval()) );
}

static inline float1 dot(float4 const& r)
{
	return float1( Vdot(r.eval(), r.eval()) );
}

static inline float1 length(float4 const& r)
{
	return float1(Vsqrt( Vdot(r.eval(), r.eval()) ));
}

static inline float lerp(float const& a, float const& b, float x)
{
	return a + x*(b - a);
}

static inline float1 lerp(float1 const& a, float1 const& b, float1 const& x)
{
	return a + x*(b - a);
}

static inline float4 lerp(float4 const& a, float4 const& b, float1 const& x)
{
	return a + x*(b - a);
}

static inline float4 lerp(float4 const& a, float4 const& b, float4 const& x)
{
	return a + x*(b - a);
}

template <typename T> static inline T maximum(T const& a, T const& b)
{
	return cond(a > b, a, b);
}

static inline float1 maximum(float4 const& r)
{
	return float1( Vlargest(r.eval()) );
}

template <typename T> static inline T minimum(T const& a, T const& b)
{
	return cond(a < b, a, b);
}	

static inline float1 minimum(float4 const& r)
{
	return float1( Vsmallest(r.eval()) );
}

static inline float4 normalize(float4 const& r)
{
	return float4( Vmul(r.eval(), Vrsqrt(Vdot(r.eval(), r.eval()) ) ));
}	

static inline float radians(float const& deg)
{
	return M_DEG_2_RADf*deg;
}

static inline float4 radians(float4 const& deg)
{
	return float1(M_DEG_2_RADf)*deg;
}

static inline float4 rcp(float4 const& r)
{
	return float4(Vrcp(r.eval()));
}	

static inline float1 rcp(float1 const& r)
{
	return float1(Vrcp(r.eval()));
}

static inline float4 rsqrt(float4 const& r)
{
	return float4(Vrsqrt(r.eval()));
}		

static inline float saturate(float const& r)
{
	return clamp(r, 0.f, 1.f);
}

static inline float1 saturate(float1 const& r)
{
	return float1(Vmin( Vmax(r.eval(), Vzero()), Vone()));	
}

static inline float4 saturate(float4 const& r)
{
	return float4(Vmin( Vmax(r.eval(), Vzero()), Vone()));		
}

static inline float4 scaleIdentity()
{
	return float4::one();
}

static inline float4 scaleWeight(float4 const& s, float1 const& w)
{
	float w1 = w.tofloat();
	return float4(pow( s.x().tofloat(), w1), pow( s.y().tofloat(), w1), pow( s.z().tofloat(), w1), pow( s.w().tofloat(), w1));
}

static inline float4 scaleBlend(float4 const& sa, float4 const& sb,float1 const& w)
{
	const float4 saw = scaleWeight(sa,float1(1.f)-w);
	const float4 sbw = scaleWeight(sb,w);
	return saw * sbw;
}

static inline float sgn(float const& r)
{
	return cond(r >= 0.f, 1.f, -1.f);
}

static inline float1 sgn(float1 const& r)
{
	return float1(Vsgn(r.eval()));
}

template <typename T> static inline vecexp4<vec4> sgn(vecexp4<T> const& x)
{
	return vecexp4<vec4>( Vsgn( x.eval()) );
}

template <typename T> static inline vecexp1<vec4> sgn(vecexp1<T> const& x)
{
	return vecexp1<vec4>( Vsgn( x.eval() ) );
}

static inline float sign(float const& r)
{
	return cond( r > 0, 1.f, cond( r < 0, -1.f, 0.f));
}

static inline float4 sign(float4 const& r)
{
	return float4(Vsign(r.eval()));
}

static inline float4 smoothClamp(float4 const& v, float4 const& m, float1 const& r)
{
	return cond(v-m>float1::zero(),m+r*((v-m)/(v-m+r)),v);
}

static inline float smoothstep( float min, float max, float x)
{
	x = Math::clamp(x, min, max);
	return -2.f * Math::pow((x-min)/(max-min), 3.f) + 3.f * Math::pow((x-min)/(max-min), 2.f);
}

static inline float smoothpulse( float minmin, float minmax, float maxmin, float maxmax, float x)
{
	return smoothstep(minmin,minmax,x) - smoothstep(maxmin,maxmax,x); 
}

static inline float1 sqrt(float1 const& r)
{
	return float1(Vsqrt(r.eval()));
}

static inline float4 sqrt(float4 const& r)
{
	return float4(Vsqrt(r.eval()));
}

static inline float1 sum(float4 const& r)
{
	return float1(Vsum(r.eval()));
}

static inline float1 triangleAngle(Math::float1 const& aLen, Math::float1 const& aLen1, Math::float1 const& aLen2)
{ 
	Math::float1 c = clamp<float1>((aLen1*aLen1 + aLen2*aLen2 - aLen*aLen) / (aLen1*aLen2) / float1(2.f), -float1::one() , float1::one());
	return Math::float1(acos(c.tofloat()));
}

static inline float4 vector(float4 const& v)
{
	constant_float4( mask, 1.f,1.f,1.f,0.f);
	return v*mask;
}	

static inline float4 vector(float const& x, float const& y, float const& z)
{
	return float4(x, y, z, 0);
}

static inline float unrollangle(float angleRef, float angle)
{
	float i;
	float f = Math::modf( (angleRef-angle)/(2.f*M_PIf), i);
	return angle + ( (i+(Math::abs(f) > 0.5f ? Math::sgn(f) * 1 : 0)) * 2.f * M_PIf);
}

static inline float4 doubleAtan(float4 const& v)
{
	float ATTRIBUTE_ALIGN(ALIGN4F) av[4];
	
	store(v, av);

	return float4(2.0f*atan(av[0]),2.0f*atan(av[1]),2.0f*atan(av[2]),2.0f*atan(av[3]));
}


// between range [-pi/2, pi/2] the maximum error is 8.186e-4
static inline vec4f cos_estimate(vec4f x)
{
	// cos(x) = 1 - (c2*x^2) + (c4*x^4) - (c6*x^6)
	// cos(x) = 1 + (-c2*x^2) + (c4*x^4) + (-c6*x^6) // 3 mul and 3 mul add
	// let's bake sign into constant to remove some complexity
	cvec4fs(c2, -0.5f);
	cvec4fs(c4, 4.166666666667e-2f);
	cvec4fs(c6, -1.38888889e-3f);
	
	// Use horner form to reduce the polynomial instruction count
	// cos(x) = 1 + x^2*(c2 + x^2*(c4 + x^2*(c6))) // 1 mul and 3 mul add
	vec4f x2 = Vmul(x,x);
	return Vmadd(Vmadd(Vmadd(c6, x2, c4), x2, c2), x2, Vone());
}


// between range [-pi/2, pi/2] the maximum error is 9.1e-5
static inline vec4f sin_estimate(vec4f x)
{
	// sin(x) = x - (c3*x^3) + (c5*x^5) - (c7*x^7)
	// sin(x) = x + (-c3*x^3) + (c5*x^5) + (-c7*x^7) // 4 mul and 3 mul add
	// let's bake sign into constant to remove some complexity
	cvec4fs(c3, -0.166666567325592041015625f);
	cvec4fs(c5, 8.33220803e-3f);
	cvec4fs(c7, -1.95168955e-4f);
	
	// Use horner form to reduce the polynomial instruction count
	// sin(x) = x * ( 1 + x^2*(c3 + x^2*(c5 + x^2*c7))) // 2 mul and 3 mul add
	vec4f x2 = Vmul(x,x);
	return Vmul(x, Vmadd(Vmadd(Vmadd(c7, x2, c5), x2, c3), x2, Vone()));
}

static inline float4 sin_est(float4 const& x)
{
	return float4( sin_estimate( x.eval() ) );
}

static inline float4 cos_est(float4 const& x)
{
	return float4( cos_estimate( x.eval() ) );
}

static inline void sincos(float4 const& u, float4& s, float4& c)
{
	float ATTRIBUTE_ALIGN(ALIGN4F) sv[4];
	float ATTRIBUTE_ALIGN(ALIGN4F) cv[4];
	float ATTRIBUTE_ALIGN(ALIGN4F) uv[4];
	
	store(u, uv);

	sincos(uv[0], sv[0], cv[0]);
	sincos(uv[1], sv[1], cv[1]);
	sincos(uv[2], sv[2], cv[2]);
	sincos(uv[3], sv[3], cv[3]);

	s = load(sv);
	c = load(cv);
}

static inline void sincos(float1 const& u, float1& s, float1& c)
{
	float sv;
	float cv;
	
	sincos(u.tofloat(), sv, cv);

	s = float1(sv);
	c = float1(cv);
}

static inline void sincos_est(float4 const& u, float4& s, float4& c)
{
	s = sin_est(u);
	c = cos_est(u);
}

static inline void sincos_est(float1 const& u, float1& s, float1& c)
{
	s = float1( sin_estimate( u.eval() ) );
	c = float1( cos_estimate( u.eval() ) );
}

static inline float4 tan(float4 const& x)
{
	vec4f x2,x3;

	// Compute x^2 and x^3
	//
	x2 = Vmul(x.eval(),x.eval());
	x3 = Vmul(x2,x.eval());	    

	// Compute both the sin and cos of the angles
	// using a polynomial expression:
	//   cx = 1.0f + x2 * (C0 * x2 + C1), and
	//   sx = xl + x3 * S0
	//
	cvec4fs(c0, 0.0097099364f);
	cvec4fs(c1, -0.4291161787f);
	cvec4fs(s0, -0.0957822992f);

	vec4f ct2 = Vmadd(c0,x2,c1);
    
	vec4f cx = Vmadd(ct2,x2, Vone());
	vec4f sx = Vmadd(s0,x3, x.eval());

	return float4(Vdiv(sx,cx));
}

static inline float4 atan(float4 const& x)
{
	//x - (x^3)/3 + (x^5)/5 - (x^7)/7 + ...
	
	cvec4fs(c3, 3.f);
	cvec4fs(c5, 5.f);
	cvec4fs(c7, 7.f);
	vec4f x2 = Vmul(x.eval(),x.eval());
	vec4f x3 = Vmul(x2,x.eval());
	vec4f x5 = Vmul(x3,x2);
	vec4f x7 = Vmul(x5,x2);

	return float4(Vsub( x.eval(), Vadd(Vdiv( x3, c3), Vsub(Vdiv(x5,c5), Vdiv(x7,c7)))));
}

static inline float halfTan(float a)
{
	//float x = Math::fmod(0.5f*abs(a)+M_PI_2,float(M_PI));
	float x1 = (0.5f*abs(a)+M_PI_2f);
	return tan(clamp(sign(a)*(x1-M_PI_2f),-M_PI_2f+M_EPSF,M_PI_2f-M_EPSF));
}

static inline float4 halfTan(float4 const& a)
{	
	static const float4 nM_PI_2(-M_PI_2f+M_EPSF);
	static const float4 pM_PI_2( M_PI_2f+M_EPSF);

	float4 x = float1(0.5f) * abs(a) + float1(M_PI_2f);		
	return tan( Math::clamp<float4>( sign(a) * (x-float4(M_PI_2f)), nM_PI_2, pM_PI_2  ));
}

static inline float4 mirror(float4 const& t)
{
	constant_float4(mirrorT,-1,1,1,1);
	return t * mirrorT;
}

static inline float4 load(float const* v)
{
	return float4(Vloadpf(v, 0));
}

static inline void store(float4 const& v, float* r)
{
	Vstorepf(v.eval(), r, 0);
}	

static inline void store(bool4 const& v, bool* r)
{
	Vstorepb(v.v, r);
}	

}

#endif

