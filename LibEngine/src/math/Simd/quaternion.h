#ifndef SIMD_QUATERNION_H
#define SIMD_QUATERNION_H

#include "Runtime/Math/Simd/math.h"

QIMP_BEGIN_NAMESPACE

static inline float4 quatIdentity()
{
	cvec4f(id, 0,0,0,1);	
	return float4(id);
}

static inline float4 quatConj(float4 const& q)
{
	cvec4f(conj, -1,-1,-1,1);	
	return float4(Vmul(q.eval(), conj));
}

static inline float4 quatMul(float4 const& a, float4 const& b)
{		
	return quatConj(a.zxyw()*b.yzxw() - a.ywzx()*b.zywx() - a.wyxz()*b.xwyz() - a.xzwy()*b.wxzy());
}

static inline float4 quatMulVec(float4 const& q, float4 const& v1)
{
	const float4 v = Math::vector(v1 + v1);
	const float4 qv = q*v;
	return q.w()*(cross(q, v) + q.w()*v) + q*(qv + qv.yzxw() + qv.zxyw()) + quatConj(v1);
}

static inline float4 quatLerp(float4 const& p, float4 const& q, float1 const& blend)
{
   return normalize(p + blend*(q*sgn(dot(p, q)) - p));
}

static inline float4 quatArcRotate(float4 const& a, float4 const& b)
{
	float4 q = cross(a, b);
	q.w() = dot(a, b) + Math::sqrt( float1(dot(a)*dot(b)) );
	return q;
}

static inline float4 quatArcRotateX(float4 const& n)
{
	return float4(float1::zero(),-n.z(),n.y(),n.x()+float1::one());
}

static inline float4 quatXcos(float4 const &qn)
{
	const float4 qw = qn.w()*qn - float4(0, 0, 0, .5f);
	const float4 u = qn.x()*qn + float4(1, 1, -1, -1) * qw.wzyx();
	return u + u;
}

static inline float4 quatYcos(float4 const &qn)
{
	const float4 qw = qn.w()*qn - float4(0, 0, 0, .5f);
	const float4 v = qn.y()*qn + float4(-1, 1, 1, -1)*qw.zwxy();
	return v + v;
}

static inline float4 quatZcos(float4 const &qn)
{
	const float4 qw = qn.w()*qn - float4(0, 0, 0, .5f);
	const float4 w = qn.z()*qn + float4(1, -1, 1, -1)*qw.yxwz();

	return w + w;
}

static inline float4 quatEulerToQuat(float4 const& euler)
{
	float4 s, c; sincos( float1(0.5f)*euler, s, c);

	const float4 t = float4(s.x()*c.z(), s.x()*s.z(), c.x()*s.z(), c.x()*c.z());

	constant_float4( mask, -1.f, 1.f, -1.f, 1.f);
	return c.y()*t + s.y()*mask*t.zwxy();
}

static inline float4 quatQuatToEuler(float4 const& q)
{
	float4 euler;

	const float4 x = q.x()*q;
	const float4 y = q.y()*q;
	const float1 discr = x.z() - y.w();

	if(discr >= float1(.5f - M_EPSF)) 
	{
		float1 _y = x.w() - y.z();
		float1 _x = -x.z() - y.w();
		
		euler = float4( atan2( _y.tofloat(), _x.tofloat() ), -M_PI_2f, 0.f, 0.f);
	} 
	else 
	{
		const float4 w = q.wwwz()*q.wwzz() - float4(.5f, 0.f, 0.f, 0.f);
		if(discr <= float1(M_EPSF - .5f)) 
		{
			float1 _y = x.y() - w.z();
			float1 _x = y.y() + w.x();
			euler = float4( atan2( _y.tofloat(), _x.tofloat() ), M_PI_2f, 0.f, 0.f);
		} 
		else 
		{
			float1 _yX = x.w() + y.z();
			float1 _xX = w.w() + w.x();
			float1 discr2 = discr + discr;
			float1 _yZ = x.y() + w.z();
			float1 _xZ = x.x() + w.x();

			euler = float4( atan2( _yX.tofloat(), _xX.tofloat() ), -asin( discr2.tofloat() ), atan2( _yZ.tofloat(), _xZ.tofloat() ), 0.f);
		}
	}
	return euler;
}

// get unit quaternion from rotation matrix
static inline float4 quatMatrixToQuat(float4 const& u, float4 const& v, float4 const& w)
{
	float4 q;
	if(u.x() >= float1::zero()) 
	{
		const float1 t = v.y() + w.z();
		if(t >= float1::zero()) 
		{			
			float1 x(v.z() - w.y());
			float1 y(w.x() - u.z());
			float1 z(u.y() - v.x());
			float1 ww(float1::one() + u.x() + t);
			q = float4(x, y, z, ww);
			// Android doesn't like this expression, it does generate the wrong assembly
			//q = float4(v.z() - w.y(), w.x() - u.z(), u.y() - v.x(), float1::one() + u.x() + t);
		} 
		else 
		{
			float1 x(float1::one() + u.x() - t);
			float1 y(u.y() + v.x());
			float1 z(w.x() + u.z());
			float1 ww(v.z() - w.y());
			q = float4(x, y, z, ww);
			// Android doesn't like this expression, it does generate the wrong assembly
			//q = float4(float1::one() + u.x() - t, u.y() + v.x(), w.x() + u.z(), v.z() - w.y());
		}
	} 
	else 
	{
		const float1 t = v.y() - w.z();
		if(t >= float1::zero()) 
		{
			float1 x(u.y() + v.x());
			float1 y(float1::one() - u.x() + t);
			float1 z(v.z() + w.y());
			float1 ww(w.x() - u.z());
			q = float4(x, y, z, ww);
			// Android doesn't like this expression, it does generate the wrong assembly
			//q = float4(u.y() + v.x(), float1::one() - u.x() + t, v.z() + w.y(), w.x() - u.z());
		} 
		else 
		{
			float1 x(w.x() + u.z());
			float1 y(v.z() + w.y());
			float1 z(float1::one() - u.x() - t);
			float1 ww(u.y() - v.x());
			q = float4(x, y, z, ww);
			// Android doesn't like this expression, it does generate the wrong assembly
			//q = float4(w.x() + u.z(), v.z() + w.y(), float1::one() - u.x() - t, u.y() - v.x());
		}
	}
	return normalize(q);
}

static inline float4 quatProjOnYPlane(float4 const& q)
{
	constant_float4(yMask, 0,1,0,0);
	constant_float4(ywMask, 0,1,0,1);

	const float4 lQAlignUp = quatArcRotate(quatYcos(q), yMask );
	
	return normalize( quatMul(lQAlignUp,q) * ywMask);
}

static inline float4 quatClamp(const float4 &q,float maxAngle)
{
	float4 ret = q;

	float1 halfCosMaxAnlge = float1(Math::cos(0.5f*maxAngle));

	float4 qn = normalize(q);
	qn = cond(qn.w() < float1::zero(), -qn, qn);

	if(qn.w() < halfCosMaxAnlge)
	{
		float1 fact = (halfCosMaxAnlge - qn.w()) / halfCosMaxAnlge;

		ret = qn * (float1::one() - fact); 
		ret.w() = lerp(halfCosMaxAnlge, float1::one(), fact);
	}

	return ret;
}

static inline float4 quatWeight(float4 const& q, float1 const& w)
{
   return normalize(float4(q.x()*w,q.y()*w,q.z()*w,q.w()));
}

static inline float4 quat2Qtan(float4 const& q)
{
	float1 w = q.w();	
	w = cond(w == float1::zero(), float1(M_EPSF), w);
	return Math::vector(q/w);
}

static inline float4 qtan2Quat(float4 const& q)
{
	float4 qn = q;
	qn.w() = float1::one();
	return normalize(qn);
}

static inline float4 ZYRoll2Quat(float4 const& zyroll)
{
	return normalize(float4(zyroll.x(),zyroll.y()+zyroll.x()*zyroll.z(),zyroll.z()-zyroll.x()*zyroll.y(), float1::one())); 
}

static inline float4 quat2ZYRoll(float4 const& q)
{
	const float4 qtan = quat2Qtan(q);
	const float1 qtanx = qtan.x();
	const float1 x2p1 = float1::one()+qtanx*qtanx;
	return float4(qtanx,(qtan.y()-qtanx*qtan.z())/x2p1,(qtan.z()+qtanx*qtan.y())/x2p1,float1::zero());	
}

static inline float4 RollZY2Quat(float4 const& zyroll)
{
	return normalize(float4(zyroll.x(),zyroll.y()-zyroll.x()*zyroll.z(),zyroll.z()+zyroll.x()*zyroll.y(),float1::one())); 
}

static inline float4 quat2RollZY(float4 const& q)
{
	const float4 qtan = quat2Qtan(q);
	const float1 qtanx = qtan.x();
	const float1 x2p1 = float1::one()+qtanx*qtanx;
	return float4(qtanx,(qtan.y()+qtanx*qtan.z())/x2p1,(qtan.z()-qtanx*qtan.y())/x2p1,float1::zero());	
}

}

#endif

