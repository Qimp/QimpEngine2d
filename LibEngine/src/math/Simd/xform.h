#ifndef SIMD_XFORM_H
#define SIMD_XFORM_H

//#include "Runtime/Serialize/TransferFunctions/SerializeTransfer.h"

#include "Runtime/Math/Simd/float4.h"
#include "Runtime/Math/Simd/quaternion.h"

QIMP_BEGIN_NAMESPACE

struct ATTRIBUTE_ALIGN(ALIGN4F) xform  
{		
	//DEFINE_GET_TYPESTRING(xform)

	inline xform():t(float4::zero()),q(quatIdentity()),s(float4::one()){}

	inline xform( xform const& x)
		{ t = x.t; q = x.q; s = x.s; }
	
	inline xform &operator=(xform const& x) 
		{ t = x.t; q = x.q; s = x.s; return *this; }
	
	inline xform(float4 const& t, float4 const& q, float4 const& s)
		{ this->t = t; this->q = q; this->s = s; }

	float4 t;
	float4 q;
	float4 s;

	//template<class TransferFunction>
	//inline void Transfer (TransferFunction& transfer)
	//{
	//	TRANSFER(t);
	//	TRANSFER(q);
	//	TRANSFER(s);
	//}
};

static inline xform xformIdentity()
{
	return xform( float4::zero(), quatIdentity(), float4::one() );
}

static inline float4 xformMulVec(xform const& x, float4 const& v)
{
	return x.t + quatMulVec(x.q, v * x.s );
}

static inline float4 xformInvMulVec(xform const& x, float4 const& v)
{
	return quatMulVec(quatConj(x.q), v - x.t) / x.s;
}

static inline xform xformMul(xform const& a, xform const& b)
{
	return xform( xformMulVec(a, b.t), normalize(quatMul(a.q, b.q)), a.s * b.s);
}

static inline xform xformInvMul(xform const& a, xform const& b)
{
	return xform(xformInvMulVec(a, b.t), normalize( quatMul( quatConj(a.q), b.q)), b.s / a.s);	
}

static inline xform xformMulInv(xform const& a, xform const& b)
{
	const float4 qinv = quatConj(b.q);
	const float4 sinv = rcp(b.s);

	return xform(xformMulVec(a, quatMulVec( qinv, -b.t) * sinv),normalize( quatMul( a.q, qinv)), a.s * sinv);	
}

static inline xform xformBlend(xform const &a, xform const &b, float1 const& w)
{
	return xform(lerp(a.t, b.t, w),quatLerp(a.q, b.q, w),scaleBlend(a.s, b.s, w));
}

static inline bool operator==(xform const& l, xform const& r)
{ 
	return all(l.t == r.t) && all(l.q == r.q) && all(l.s == r.s);
}

static inline xform xformWeight(xform const& x, float1 const& w)
{
	return xform(x.t*w,quatWeight(x.q,w),scaleWeight(x.s,w));
}

static inline xform xformAdd(xform const& a, xform const& b)
{
	return xform(a.t+b.t,qtan2Quat(quat2Qtan(a.q)+quat2Qtan(b.q)),a.s*b.s);
}

static inline xform xformSub(xform const& a, xform const& b)
{
	return xform(a.t-b.t,qtan2Quat(quat2Qtan(a.q)-quat2Qtan(b.q)),a.s/b.s);
}

static inline xform xformBlend(xform const* apXFormArray, float const* apWeightArray, unsigned int aCount)
{
	xform ret;

	ret.t = float4::zero(); 
	ret.q = float4::zero(); 
	ret.s = float4::one(); 

	float sumW = 0;

	unsigned int i;
	for(i = 0; i < aCount ; i++)
	{
		float w = apWeightArray[i]; 
		Math::float1 w1(w);
		sumW += w;
		
		ret.t += apXFormArray[i].t*w1; 
		ret.q += cond(dot(ret.q,apXFormArray[i].q) < float1::zero(),apXFormArray[i].q * -w1,apXFormArray[i].q * w1); 
		ret.s *= scaleWeight(apXFormArray[i].s,w1); 
	}

	float4 q(0,0,0,saturate(1.0f-sumW));
	ret.q = normalize(ret.q+q);

	return ret;
}

static inline xform mirror(xform const& x)
{
	constant_float4(mirrorQ,1,-1,-1,1);

	xform ret = x;		
	ret.t = mirror(ret.t);
	ret.q *= mirrorQ;
	return ret;
}

static inline xform cond(bool c, xform const& a, xform const& b)
{
	return c?a:b;
}

}

#endif
