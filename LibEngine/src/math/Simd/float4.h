#ifndef SIMD_FLOAT4_H
#define SIMD_FLOAT4_H

//#include "Runtime/Serialize/TransferFunctions/SerializeTransfer.h"

#include "Runtime/Math/Simd/intrinsic.h"

#include "Runtime/Math/Simd/bool4.h"
#include "Runtime/Math/Simd/float1.h"

QIMP_BEGIN_NAMESPACE

template<typename T> struct ATTRIBUTE_ALIGN(ALIGN4F) vecexp4 : T
{
	typedef T						value_type;
	typedef typename T::scalar_type	scalar_type;
	typedef typename T::packed_type	packed_type;

	MECANIM_FORCE_INLINE vecexp4() {}

	MECANIM_FORCE_INLINE vecexp4(vecexp4 const& e):value_type(e) { }

	template<typename R> MECANIM_FORCE_INLINE vecexp4(const vecexp4<R> &e):value_type(e){	}

	MECANIM_FORCE_INLINE vecexp4(scalar_type const& x, scalar_type const& y, scalar_type const& z, scalar_type const& w):value_type(x,y,z,w) { }

	MECANIM_FORCE_INLINE vecexp4(value_type const& r):value_type(r) { }	
	
	//template<typename R1, typename R2, typename R3, typename R4> MECANIM_FORCE_INLINE vecexp4(const vecexp1<R1> &x, const vecexp1<R2> &y, const vecexp1<R3> &z, const vecexp1<R4> &w) { 
	//	value_type::v = Vcombine( x.eval(), y.eval(), z.eval(), w.eval() );
	//}
	MECANIM_FORCE_INLINE vecexp4(const float1 &x, const float1 &y, const float1 &z, const float1 &w) { 
		value_type::v = Vcombine( x.eval(), y.eval(), z.eval(), w.eval() );
	}

	MECANIM_FORCE_INLINE vecexp4 &operator=(const vecexp4 &l)	{ value_type::operator =(l); return *this; }

	template<typename R> MECANIM_FORCE_INLINE vecexp4 &operator=(const vecexp4<R> &e) { 
		value_type::v = e.eval(); return *this; 
	}

	template<typename R> MECANIM_FORCE_INLINE vecexp4 &operator=(const vecexp1<R> &e) { 
		value_type::v = e.eval(); return *this; 
	}

	MECANIM_FORCE_INLINE const vecexp4 &operator+() const	{ return *this; }
	MECANIM_FORCE_INLINE vecexp4 operator-() const { return vecexp4(Vneg( value_type::eval() )); }
};

template<typename SCALAR, typename RHS_VECTOR, typename LHS_VECTOR, int RHS_MASK, int LHS_MASK> struct ATTRIBUTE_ALIGN(ALIGN4F) swizzle1
{
	typedef SCALAR			scalar_type;
	typedef LHS_VECTOR		packed_type;
	typedef RHS_VECTOR		rhs_packed_type;

	enum{
		RHS_SWZ = RHS_MASK,
		LHS_SWZ = LHS_MASK
	};

	
	packed_type v;
	
	MECANIM_FORCE_INLINE swizzle1(packed_type vector):v(vector) {}

	MECANIM_FORCE_INLINE swizzle1 &operator=(const scalar_type &s) { v = Vswizzle<LHS_SWZ>::lhs(v, Vloadsf(s)); return *this; }

	MECANIM_FORCE_INLINE scalar_type tofloat() { return Vstoresf( Vswizzle<RHS_SWZ>::rhs(v) ); }

	MECANIM_FORCE_INLINE rhs_packed_type eval()const{ return Vswizzle<RHS_SWZ>::rhs(v); }
private:


	MECANIM_FORCE_INLINE swizzle1 &operator=(const swizzle1 &s) {return *this;}
};

template<typename SCALAR, typename VECTOR, int MASK> struct ATTRIBUTE_ALIGN(ALIGN4F) swizzle
{
	typedef SCALAR			scalar_type;
	typedef VECTOR			packed_type;

	enum{
		RHS_SWZ = MASK
	};

	MECANIM_FORCE_INLINE swizzle(packed_type const& vector):v(vector) {}

	MECANIM_FORCE_INLINE packed_type eval()const{ return Vswizzle<RHS_SWZ>::rhs(v); }

protected:
	packed_type v;
};

struct ATTRIBUTE_ALIGN(ALIGN4F) vec4 
{
	typedef float			scalar_type;
	typedef vec4f			packed_type;
	typedef vec4f const&	const_reference_packed_type;
	typedef vec4f&			reference_packed_type;

	enum{
		RHS_SWZ = kXYZW
	};
	
	MECANIM_FORCE_INLINE vec4() {}

	MECANIM_FORCE_INLINE vec4(scalar_type x, scalar_type y, scalar_type z, scalar_type w):v(Vload4sf(x,y,z,w)) {  }

	MECANIM_FORCE_INLINE vec4(scalar_type s):v(Vloadsf(s)) {  }

	MECANIM_FORCE_INLINE vec4(packed_type vector):v(vector){ }

	template<typename T> MECANIM_FORCE_INLINE vec4(const T& vector):v(vector.eval()){ }

	MECANIM_FORCE_INLINE vec4 &operator=(const vec4 &l) { 
		//if(!IsFinite())
		//{
		//	int a=0;
		//}
		v = l.v; 
		return *this; 
	}

	MECANIM_FORCE_INLINE packed_type eval()const{ return v; }

	inline bool IsFinite ()const
	{
		return ::IsFinite( x().tofloat() ) & ::IsFinite( y().tofloat() ) & ::IsFinite( z().tofloat() ) & ::IsFinite( w().tofloat() );
	}

	MECANIM_FORCE_INLINE vecexp1< swizzle1<scalar_type, packed_type, packed_type, kXXXX, kXYZW> > x()const { return vecexp1< swizzle1<scalar_type, packed_type, packed_type, kXXXX, kXYZW> >(v); }	
	MECANIM_FORCE_INLINE vecexp1< swizzle1<scalar_type, packed_type, packed_type, kYYYY, kYXZW> > y()const { return vecexp1< swizzle1<scalar_type, packed_type, packed_type, kYYYY, kYXZW> >(v); }	
	MECANIM_FORCE_INLINE vecexp1< swizzle1<scalar_type, packed_type, packed_type, kZZZZ, kZYXW> > z()const { return vecexp1< swizzle1<scalar_type, packed_type, packed_type, kZZZZ, kZYXW> >(v); }	
	MECANIM_FORCE_INLINE vecexp1< swizzle1<scalar_type, packed_type, packed_type, kWWWW, kWYZX> > w()const { return vecexp1< swizzle1<scalar_type, packed_type, packed_type, kWWWW, kWYZX> >(v); }	

	MECANIM_FORCE_INLINE vecexp1< swizzle1<scalar_type, packed_type, reference_packed_type, kXXXX, kXYZW> > x() { return vecexp1< swizzle1<scalar_type, packed_type, reference_packed_type, kXXXX, kXYZW> >(v); }	
	MECANIM_FORCE_INLINE vecexp1< swizzle1<scalar_type, packed_type, reference_packed_type, kYYYY, kYXZW> > y() { return vecexp1< swizzle1<scalar_type, packed_type, reference_packed_type, kYYYY, kYXZW> >(v); }	
	MECANIM_FORCE_INLINE vecexp1< swizzle1<scalar_type, packed_type, reference_packed_type, kZZZZ, kZYXW> > z() { return vecexp1< swizzle1<scalar_type, packed_type, reference_packed_type, kZZZZ, kZYXW> >(v); }	
	MECANIM_FORCE_INLINE vecexp1< swizzle1<scalar_type, packed_type, reference_packed_type, kWWWW, kWYZX> > w() { return vecexp1< swizzle1<scalar_type, packed_type, reference_packed_type, kWWWW, kWYZX> >(v); }	
																 
	MECANIM_FORCE_INLINE vecexp4< swizzle<scalar_type, packed_type, kWXZY> > wxzy()const { return vecexp4< swizzle<scalar_type, packed_type, kWXZY> >(v); }	
	MECANIM_FORCE_INLINE vecexp4< swizzle<scalar_type, packed_type, kXZWY> > xzwy()const { return vecexp4< swizzle<scalar_type, packed_type, kXZWY> >(v); }
	MECANIM_FORCE_INLINE vecexp4< swizzle<scalar_type, packed_type, kXWYZ> > xwyz()const { return vecexp4< swizzle<scalar_type, packed_type, kXWYZ> >(v); }
	MECANIM_FORCE_INLINE vecexp4< swizzle<scalar_type, packed_type, kWYXZ> > wyxz()const { return vecexp4< swizzle<scalar_type, packed_type, kWYXZ> >(v); }
	MECANIM_FORCE_INLINE vecexp4< swizzle<scalar_type, packed_type, kZYWX> > zywx()const { return vecexp4< swizzle<scalar_type, packed_type, kZYWX> >(v); }
	MECANIM_FORCE_INLINE vecexp4< swizzle<scalar_type, packed_type, kYWZX> > ywzx()const { return vecexp4< swizzle<scalar_type, packed_type, kYWZX> >(v); }
	MECANIM_FORCE_INLINE vecexp4< swizzle<scalar_type, packed_type, kYZXW> > yzxw()const { return vecexp4< swizzle<scalar_type, packed_type, kYZXW> >(v); }
	MECANIM_FORCE_INLINE vecexp4< swizzle<scalar_type, packed_type, kZXYW> > zxyw()const { return vecexp4< swizzle<scalar_type, packed_type, kZXYW> >(v); }
	MECANIM_FORCE_INLINE vecexp4< swizzle<scalar_type, packed_type, kZWXY> > zwxy()const { return vecexp4< swizzle<scalar_type, packed_type, kZWXY> >(v); }
	MECANIM_FORCE_INLINE vecexp4< swizzle<scalar_type, packed_type, kWWWZ> > wwwz()const { return vecexp4< swizzle<scalar_type, packed_type, kWWWZ> >(v); }
	MECANIM_FORCE_INLINE vecexp4< swizzle<scalar_type, packed_type, kWWZZ> > wwzz()const { return vecexp4< swizzle<scalar_type, packed_type, kWWZZ> >(v); }
	MECANIM_FORCE_INLINE vecexp4< swizzle<scalar_type, packed_type, kWZYX> > wzyx()const { return vecexp4< swizzle<scalar_type, packed_type, kWZYX> >(v); }
	MECANIM_FORCE_INLINE vecexp4< swizzle<scalar_type, packed_type, kYXWZ> > yxwz()const { return vecexp4< swizzle<scalar_type, packed_type, kYXWZ> >(v); }

protected:
	packed_type			v;
};		

struct ATTRIBUTE_ALIGN(ALIGN4F) float4 : vecexp4<vec4>
{		
	//DEFINE_GET_TYPESTRING(float4)

	typedef vec4				value_type;
	typedef vec4::scalar_type	scalar_type;
	typedef vec4::packed_type	packed_type;

	MECANIM_FORCE_INLINE float4() {}

	MECANIM_FORCE_INLINE float4(float4 const& vector):vecexp4<value_type>(vector.v) { }
	
	explicit MECANIM_FORCE_INLINE float4(scalar_type s):vecexp4<value_type>(s) { }

	explicit MECANIM_FORCE_INLINE float4(packed_type const& vector):vecexp4<value_type>(vector) { }

	template<typename R> MECANIM_FORCE_INLINE float4(const vecexp4<R> &r):vecexp4<value_type>(r)	{ }

	MECANIM_FORCE_INLINE float4(scalar_type x, scalar_type y, scalar_type z, scalar_type w):vecexp4<value_type>(x,y,z,w) { }

	MECANIM_FORCE_INLINE float4(const float1 &x, const float1 &y, const float1 &z, const float1 &w):vecexp4<value_type>(x,y,z,w) { }

	MECANIM_FORCE_INLINE float4 &operator=(const float4 &r) { vecexp4<vec4>::operator =(r); return *this; }

	template<typename R> MECANIM_FORCE_INLINE float4 &operator=(const vecexp4<R> &r) { vecexp4<vec4>::operator =(r); return *this; }
	template<typename R> MECANIM_FORCE_INLINE float4 &operator=(const vecexp1<R> &r) { vecexp4<vec4>::operator =(r); return *this; }

	template<typename R> MECANIM_FORCE_INLINE float4 &operator+=(const vecexp4<R> &r) { value_type::v = Vadd(value_type::v, r.eval()); return *this; }
	template<typename R> MECANIM_FORCE_INLINE float4 &operator+=(const vecexp1<R> &r) { value_type::v = Vadd(value_type::v, r.eval()); return *this; }
	MECANIM_FORCE_INLINE  float4 &operator+=(float r) { value_type::v = Vadd(value_type::v, Vloadsf(r)); return *this; }

	template<typename R> MECANIM_FORCE_INLINE float4 &operator-=(const vecexp4<R> &r) { value_type::v = Vsub(value_type::v, r.eval()); return *this; }
	template<typename R> MECANIM_FORCE_INLINE float4 &operator-=(const vecexp1<R> &r) { value_type::v = Vsub(value_type::v, r.eval()); return *this; }
	MECANIM_FORCE_INLINE  float4 &operator-=(float r) { value_type::v = Vsub(value_type::v, Vloadsf(r)); return *this; }

	template<typename R> MECANIM_FORCE_INLINE float4 &operator*=(const vecexp4<R> &r) { value_type::v = Vmul(value_type::v, r.eval()); return *this; }
	template<typename R> MECANIM_FORCE_INLINE float4 &operator*=(const vecexp1<R> &r) { value_type::v = Vmul(value_type::v, r.eval()); return *this; }
	MECANIM_FORCE_INLINE  float4 &operator*=(float r) { value_type::v = Vmul(value_type::v, Vloadsf(r)); return *this; }

	
	template<typename R> MECANIM_FORCE_INLINE float4 &operator/=(const vecexp4<R> &r) { value_type::v = Vdiv(value_type::v, r.eval()); return *this; }
	template<typename R> MECANIM_FORCE_INLINE float4 &operator/=(const vecexp1<R> &r) { value_type::v = Vdiv(value_type::v, r.eval()); return *this; }
	MECANIM_FORCE_INLINE  float4 &operator/=(float r) { value_type::v = Vdiv(value_type::v, Vloadsf(r)); return *this; }

	// prefix decrement
	MECANIM_FORCE_INLINE float4 &operator++() { value_type::v = Vinc(value_type::v); return *this; }
	// postfix increment
	MECANIM_FORCE_INLINE float4 operator++(int) { float4 r = *this; value_type::v = Vinc(value_type::eval() ); return r; }

	// prefix decrement
	MECANIM_FORCE_INLINE  float4 &operator--() { value_type::v = Vdec(value_type::v); return *this; }
	// postfix decrement
	MECANIM_FORCE_INLINE float4 operator--(int) { float4 r = *this; value_type::v = Vdec(value_type::eval() ); return r; }


	static float4 zero()	{return float4(Vzero()); }    // 0
	static float4 one()		{return float4(Vone());}     // 1


	//template<class TransferFunction>
	//MECANIM_FORCE_INLINE void Transfer (TransferFunction& transfer)
	//{
	//	/////@TODO: This is wrong. It will not work for SafeBinaryRead
	//	///////     Probably other places in the code too!
	//	
	//	float ATTRIBUTE_ALIGN(ALIGN4F) buf[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	//	if(transfer.IsReading())
	//	{	
	//		transfer.Transfer(buf[0], "x");
	//		transfer.Transfer(buf[1], "y");
	//		transfer.Transfer(buf[2], "z");
	//		transfer.Transfer(buf[3], "w");

	//		v = Vloadpf(buf, 0);				
	//	}
	//	else if(transfer.IsWriting())
	//	{
	//		Vstorepf(v, buf, 0);

	//		transfer.Transfer(buf[0], "x");
	//		transfer.Transfer(buf[1], "y");
	//		transfer.Transfer(buf[2], "z");
	//		transfer.Transfer(buf[3], "w");
	//	}
	//	else
	//	{
	//		transfer.Transfer(buf[0], "x");
	//		transfer.Transfer(buf[1], "y");
	//		transfer.Transfer(buf[2], "z");
	//		transfer.Transfer(buf[3], "w");
	//	}
	//}
};

// vecexp4 Arithemtic
template<typename LHS, typename RHS> static MECANIM_FORCE_INLINE vecexp4<vec4> operator+(const vecexp4<LHS> &l, const vecexp4<RHS> &r)
{ 
	return vecexp4<vec4>( Vadd( l.eval(), r.eval() ));
}

template<typename LHS, typename RHS> static MECANIM_FORCE_INLINE vecexp4<vec4> operator+(const vecexp4<LHS> &l, const vecexp1<RHS> &r)
{ 
	return vecexp4<vec4>( Vadd( l.eval(), r.eval() ));
}

template<typename LHS, typename RHS> static MECANIM_FORCE_INLINE vecexp4<vec4> operator+(const vecexp1<LHS> &l, const vecexp4<RHS> &r)
{ 
	return vecexp4<vec4>( Vadd( l.eval(), r.eval() ));
}

template<typename LHS, typename RHS> static MECANIM_FORCE_INLINE vecexp4<vec4> operator-(const vecexp4<LHS> &l, const vecexp4<RHS> &r)
{ 
	return vecexp4<vec4>( Vsub( l.eval(), r.eval() ));
}

template<typename LHS, typename RHS> static MECANIM_FORCE_INLINE vecexp4<vec4> operator-(const vecexp4<LHS> &l, const vecexp1<RHS> &r)
{ 
	return vecexp4<vec4>( Vsub( l.eval(), r.eval() ));
}

template<typename LHS, typename RHS> static MECANIM_FORCE_INLINE vecexp4<vec4> operator-(const vecexp1<LHS> &l, const vecexp4<RHS> &r)
{ 
	return vecexp4<vec4>( Vsub( l.eval(), r.eval() ));
}

template<typename LHS, typename RHS> static MECANIM_FORCE_INLINE vecexp4<vec4> operator*(const vecexp4<LHS> &l, const vecexp4<RHS> &r)
{ 
	return vecexp4<vec4>( Vmul( l.eval(), r.eval() ));
}

template<typename LHS, typename RHS> static MECANIM_FORCE_INLINE vecexp4<vec4> operator*(const vecexp4<LHS> &l,const vecexp1<RHS> &r)
{ 
	return vecexp4<vec4>( Vmul( l.eval(), r.eval() ));
}

template<typename LHS, typename RHS> static MECANIM_FORCE_INLINE vecexp4<vec4> operator*(const vecexp1<LHS> &l, const vecexp4<RHS> &r)
{ 
	return vecexp4<vec4>( Vmul( l.eval(), r.eval() ));
}

template<typename LHS, typename RHS> static MECANIM_FORCE_INLINE vecexp4<vec4> operator/(const vecexp4<LHS> &l, const vecexp4<RHS> &r)
{ 
	return vecexp4<vec4>( Vdiv( l.eval(), r.eval() )); 
}
template<typename LHS, typename RHS> static MECANIM_FORCE_INLINE vecexp4<vec4> operator/(const vecexp4<LHS> &l, const vecexp1<RHS> &r)
{ 
	return vecexp4<vec4>( Vdiv( l.eval(), r.eval() ));   
}

template<typename LHS, typename RHS> static MECANIM_FORCE_INLINE vecexp4<vec4> operator/(const vecexp1<LHS> &l, const vecexp4<RHS> &r)
{ 
	return vecexp4<vec4>( Vdiv( l.eval(), r.eval() ));
}


// vecexp4 logic
template<typename LHS, typename RHS> static MECANIM_FORCE_INLINE bool4 operator<(const vecexp4<LHS> &l, const vecexp4<RHS> &r)
{ 
	return bool4(Vcmplt(l.eval(), r.eval())); 
}
template<typename LHS> static MECANIM_FORCE_INLINE bool4 operator<(const vecexp4<LHS> &l, const float1 &r)
{ 
	return bool4(Vcmplt(l.eval(), r.eval())); 
}
template<typename RHS> static MECANIM_FORCE_INLINE bool4 operator<(const float1 &l, const vecexp4<RHS> &r)
{ 
	return bool4(Vcmplt(l.eval(), r.eval()));
}

template<typename LHS, typename RHS> static MECANIM_FORCE_INLINE bool4 operator<=(const vecexp4<LHS> &l, const vecexp4<RHS> &r)
{ 
	return bool4(Vcmple(l.eval(), r.eval())); 
}
template<typename LHS> static MECANIM_FORCE_INLINE bool4 operator<=(const vecexp4<LHS> &l, const float1 &r)
{ 
	return bool4(Vcmple(l.eval(), r.eval()));
}
template<typename RHS> static MECANIM_FORCE_INLINE bool4 operator<=(const float1 &l, const vecexp4<RHS> &r)
{ 
	return bool4(Vcmple(l.eval(), r.eval()));
}

template<typename LHS, typename RHS> static MECANIM_FORCE_INLINE bool4 operator==(const vecexp4<LHS> &l, const vecexp4<RHS> &r)
{ 
	return bool4( Vcmpeq(l.eval(), r.eval()) ); 
}
template<typename LHS, typename RHS> static MECANIM_FORCE_INLINE bool4 operator==(const vecexp4<LHS> &l, const vecexp1<RHS> &r)
{ 
	return bool4( Vcmpeq(l.eval(), r.eval()) ); 
}
template<typename LHS, typename RHS> static MECANIM_FORCE_INLINE bool4 operator==(const vecexp1<LHS> &l, const vecexp4<RHS> &r)
{ 
	return bool4( Vcmpeq(l.eval(), r.eval()) ); 
}

template<typename LHS, typename RHS> static MECANIM_FORCE_INLINE bool4 operator!=(const vecexp4<LHS> &l, const vecexp4<RHS> &r)
{ 
	return bool4( Vcmpneq(l.eval(), r.eval())  ); 
}
template<typename LHS, typename RHS> static MECANIM_FORCE_INLINE bool4 operator!=(const vecexp1<LHS> &l, const vecexp4<RHS> &r)
{ 
	return bool4( Vcmpneq(l.eval(), r.eval()) ); 
}
template<typename LHS, typename RHS> static MECANIM_FORCE_INLINE bool4 operator!=(const vecexp4<LHS> &l, const vecexp1<RHS> &r)
{ 
	return bool4( Vcmpneq(l.eval(), r.eval()) ); 
}

template<typename LHS, typename RHS> static MECANIM_FORCE_INLINE bool4 operator>=(const vecexp4<LHS> &l, const vecexp4<RHS> &r)
{ 
	return bool4( Vcmpge(l.eval(), r.eval()) ); 
}
template<typename LHS, typename RHS> static MECANIM_FORCE_INLINE bool4 operator>=(const vecexp1<LHS> &l, const vecexp4<RHS> &r)
{ 
	return bool4( Vcmpge(l.eval(), r.eval()) ); 
}
template<typename LHS, typename RHS> static MECANIM_FORCE_INLINE bool4 operator>=(const vecexp4<LHS> &l, const vecexp1<RHS> &r)
{ 
	return bool4( Vcmpge(l.eval(), r.eval()) ); 
}

template<typename LHS, typename RHS> static MECANIM_FORCE_INLINE bool4 operator>(const vecexp4<LHS> &l, const vecexp4<RHS> &r)
{ 
	return bool4( Vcmpgt(l.eval(), r.eval()) ); 
}
template<typename LHS, typename RHS> static MECANIM_FORCE_INLINE bool4 operator>(const vecexp4<LHS> &l, const vecexp1<RHS> &r)
{ 
	return bool4( Vcmpgt(l.eval(), r.eval()) );
}
template<typename LHS, typename RHS> static MECANIM_FORCE_INLINE bool4 operator>(const vecexp1<LHS> &l, const vecexp4<RHS> &r)
{ 
	return bool4( Vcmpgt(l.eval(), r.eval()) );
}

#define constant_float4(name, x,y,z,w)					\
	cvec4f(c##name, x,y,z,w);							\
	Math::float4 const name(c##name);             \

}

#endif
