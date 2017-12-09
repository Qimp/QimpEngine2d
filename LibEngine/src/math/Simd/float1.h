#ifndef SIMD_FLOAT1_H
#define SIMD_FLOAT1_H

//#include "Runtime/Serialize/TransferFunctions/SerializeTransfer.h"

#include "Runtime/Math/Simd/intrinsic.h"

#include "Runtime/Math/Simd/bool1.h"

QIMP_BEGIN_NAMESPACE

struct float1;

template<typename T> struct ATTRIBUTE_ALIGN(ALIGN4F) vecexp1 : T
{
	typedef T				        value_type;
	typedef typename T::scalar_type	scalar_type;
	typedef typename T::packed_type	packed_type;

	MECANIM_FORCE_INLINE vecexp1() {}

	MECANIM_FORCE_INLINE vecexp1(vecexp1 const& e):value_type(e) { }
	
	explicit MECANIM_FORCE_INLINE vecexp1(scalar_type s):value_type(s){ }

	explicit MECANIM_FORCE_INLINE vecexp1(packed_type vector):value_type(vector) { }

	MECANIM_FORCE_INLINE vecexp1(value_type const& r):value_type(r) { }
	
	MECANIM_FORCE_INLINE vecexp1(scalar_type x, scalar_type y, scalar_type z, scalar_type w):value_type(x,y,z,w) { }

	template<typename R> MECANIM_FORCE_INLINE vecexp1(const vecexp1<R> &e){ 
		value_type::v = Vswizzle<value_type::LHS_SWZ>::lhs(value_type::v, e.eval() );
	}

	inline bool IsFinite ()const
	{
		return IsFinite(Vstoresf(value_type::v));
	}

	MECANIM_FORCE_INLINE vecexp1 &operator=(const vecexp1 &e)	{ 
		//if(!IsFinite())
		//{
		//	int a=0;
		//}
		value_type::v = Vswizzle<value_type::LHS_SWZ>::lhs(value_type::v, e.eval() ); 
		return *this;
	}

	template<typename R> MECANIM_FORCE_INLINE vecexp1 &operator=(const vecexp1<R> &e){ 
		//if(!IsFinite())
		//{
		//	int a=0;
		//}
		value_type::v = Vswizzle<value_type::LHS_SWZ>::lhs(value_type::v, e.eval() ); 
		return *this;
	}
	
	MECANIM_FORCE_INLINE vecexp1 &operator=(scalar_type s) { 
		value_type::operator =(s); 
		return *this; }
	
	MECANIM_FORCE_INLINE const vecexp1 &operator+() const	{ return *this; }
	MECANIM_FORCE_INLINE vecexp1 operator-() const { return vecexp1(Vneg( value_type::eval() ) ); }

	template<typename R>MECANIM_FORCE_INLINE vecexp1 &operator+=(const vecexp1<R> &r) { 
		value_type::v = Vswizzle<value_type::LHS_SWZ>::lhs(value_type::v, Vadd( value_type::eval(), r.eval() )); return *this; 
	}

	template<typename R> MECANIM_FORCE_INLINE vecexp1 &operator-=(const vecexp1<R> &r) { 
		value_type::v = Vswizzle<value_type::LHS_SWZ>::lhs(value_type::v, Vsub( value_type::eval(), r.eval() )); return *this; 
	}
	
	template<typename R> MECANIM_FORCE_INLINE vecexp1 &operator*=(const vecexp1<R> &r) { 
		value_type::v = Vswizzle<value_type::LHS_SWZ>::lhs(value_type::v, Vmul(value_type::eval(), r.eval())); return *this; 
	}

	template<typename R> MECANIM_FORCE_INLINE vecexp1 &operator/=(const vecexp1<R> &r) { 
		value_type::v = Vswizzle<value_type::LHS_SWZ>::lhs(value_type::v, Vdiv(value_type::eval(), r.eval())); return *this; 
	}
};	

struct ATTRIBUTE_ALIGN(ALIGN4F) vec1 
{
	typedef float			scalar_type;
	typedef vec4f			packed_type;
	typedef vec4f const&	const_reference_packed_type;
	typedef vec4f&			reference_packed_type;

	enum{
		RHS_SWZ = kXYZW,
		LHS_SWZ = kXYZW
	};
	
	MECANIM_FORCE_INLINE vec1() {}

	MECANIM_FORCE_INLINE vec1(scalar_type scalar):v(Vloadsf(scalar)) {  }

	MECANIM_FORCE_INLINE vec1(packed_type vector):v(vector){ }

	template<typename T> MECANIM_FORCE_INLINE vec1(const T& vector) { v = vector.eval(); }

	MECANIM_FORCE_INLINE scalar_type tofloat() const { return Vstoresf( v ); }

	MECANIM_FORCE_INLINE vec1 &operator=(const vec1 &l) { v = l.v; return *this; }

	MECANIM_FORCE_INLINE packed_type eval()const{ return v; }

protected:
	packed_type			v;
};

struct ATTRIBUTE_ALIGN(ALIGN4F) float1 : public vecexp1<vec1>
{
	//DEFINE_GET_TYPESTRING(float1)
	
	typedef vec1				value_type;
	typedef vec1::scalar_type	scalar_type;
	typedef vec1::packed_type	packed_type;

	MECANIM_FORCE_INLINE float1() {}
	
	MECANIM_FORCE_INLINE explicit float1(scalar_type s):vecexp1<value_type>(s)	{}	

	MECANIM_FORCE_INLINE explicit float1(packed_type const& v):vecexp1<value_type>(v) { }

	MECANIM_FORCE_INLINE float1(const float1 &f):vecexp1<value_type>(f) { }

	template<typename R> MECANIM_FORCE_INLINE float1(const vecexp1<R> &e){ value_type::v = e.eval(); }

	MECANIM_FORCE_INLINE float1 &operator=(const float1 &f) { value_type::v = f.eval(); return *this; }

	template<typename R> MECANIM_FORCE_INLINE float1 &operator=(const vecexp1<R> &e) { value_type::v = e.eval(); return *this; }

	template<typename R>MECANIM_FORCE_INLINE float1 &operator+=(const vecexp1<R> &r) { 
		value_type::v = Vadd( value_type::eval(), r.eval() ); return *this; 
	}

	template<typename R> MECANIM_FORCE_INLINE float1 &operator-=(const vecexp1<R> &r) { 
		value_type::v = Vsub( value_type::eval(), r.eval() ); return *this; 
	}
	
	template<typename R> MECANIM_FORCE_INLINE float1 &operator*=(const vecexp1<R> &r) { 
		value_type::v = Vmul(value_type::eval(), r.eval() ); return *this; 
	}

	template<typename R> MECANIM_FORCE_INLINE float1 &operator/=(const vecexp1<R> &r) { 
		value_type::v = Vdiv(value_type::eval(), r.eval() ); return *this; 
	}


	MECANIM_FORCE_INLINE float1 &operator++() { value_type::v = Vinc(value_type::v); return *this; }
	MECANIM_FORCE_INLINE float1 operator++(int) { float1 r = *this; value_type::v = Vinc(value_type::v); return r; }

	MECANIM_FORCE_INLINE float1 &operator--() { value_type::v = Vdec(value_type::v); return *this; }
	MECANIM_FORCE_INLINE float1 operator--(int) { float1 r = *this; value_type::v = Vdec(value_type::v); return r; }

	static float1 zero()	{return float1(Vzero()); }    // 0
	static float1 one()		{return float1(Vone());}     // 1
	
	//template<class TransferFunction>
	//MECANIM_FORCE_INLINE void Transfer (TransferFunction& transfer)
	//{
	//	/////@TODO: This is wrong. It will not work for SafeBinaryRead
	//	///////     Probably other places in the code too!
	//	
	//	float x;
	//	if(transfer.IsReading())
	//	{	
	//		transfer.Transfer(x, "x");
	//		*this = float1(x);
	//	}
	//	else if(transfer.IsWriting())
	//	{
	//		x = tofloat();
	//		
	//		transfer.Transfer(x, "x");
	//	}
	//	else
	//	{
	//		transfer.Transfer(x, "x");
	//	}
	//}
};

// vecexp1 Arithemtic
template<typename LHS, typename RHS> static MECANIM_FORCE_INLINE vecexp1<vec1> operator+(const vecexp1<LHS> &l, const vecexp1<RHS> &r)
{ 
	return vecexp1<vec1>( Vadd( l.eval(), r.eval() ));
}

template<typename LHS, typename RHS> static MECANIM_FORCE_INLINE vecexp1<vec1> operator-(const vecexp1<LHS> &l, const vecexp1<RHS> &r)
{ 
	return vecexp1<vec1>( Vsub( l.eval(), r.eval() ));
}

template<typename LHS, typename RHS> static MECANIM_FORCE_INLINE vecexp1<vec1> operator*(const vecexp1<LHS> &l, const vecexp1<RHS> &r)
{ 
	return vecexp1<vec1>( Vmul( l.eval(), r.eval() ));
}

template<typename LHS, typename RHS> static MECANIM_FORCE_INLINE vecexp1<vec1> operator/(const vecexp1<LHS> &l, const vecexp1<RHS> &r)
{ 
	return vecexp1<vec1>( Vdiv( l.eval(), r.eval() ));
}

// vecexp1 logic
template <typename LHS, typename RHS> static MECANIM_FORCE_INLINE bool1 operator<(const vecexp1<LHS> &l, const vecexp1<RHS> &r)
{ 
	return bool1( Vcmplt( l.eval(), r.eval() ) ); 
}
template <typename LHS, typename RHS> static MECANIM_FORCE_INLINE bool1 operator<=(const vecexp1<LHS> &l, const vecexp1<RHS> &r)
{ 
	return bool1( Vcmple( l.eval(), r.eval() ) ); 
}
template <typename LHS, typename RHS> static MECANIM_FORCE_INLINE bool1 operator==(const vecexp1<LHS> &l, const vecexp1<RHS> &r)
{ 
	return bool1( Vcmpeq( l.eval(), r.eval() ) ); 
}
template <typename LHS, typename RHS> static MECANIM_FORCE_INLINE bool1 operator!=(const vecexp1<LHS> &l, const vecexp1<RHS> &r)
{ 
	return bool1( Vcmpneq( l.eval(), r.eval() ) ); 
}
template <typename LHS, typename RHS> static MECANIM_FORCE_INLINE bool1 operator>=(const vecexp1<LHS> &l, const vecexp1<RHS> &r)
{ 
	return bool1( Vcmpge( l.eval(), r.eval() )); 
}
template <typename LHS, typename RHS> static MECANIM_FORCE_INLINE bool1 operator>(const vecexp1<LHS> &l, const vecexp1<RHS> &r)
{ 
	return bool1( Vcmpgt( l.eval(), r.eval() ) ); 
}

}


#endif
