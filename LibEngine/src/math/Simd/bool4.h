#ifndef SIMD_BOOL4_H
#define SIMD_BOOL4_H


#include "Runtime/Math/Simd/intrinsic.h"
#include "Runtime/Math/Simd/bool1.h"

QIMP_BEGIN_NAMESPACE

template<typename T> struct vecexp4;

struct ATTRIBUTE_ALIGN(ALIGN4F) bool4
{		
	typedef bool		scalar_type;
	typedef vec4b		packed_type;

	packed_type	v;

	MECANIM_FORCE_INLINE bool4() {}

	MECANIM_FORCE_INLINE bool4(const bool4 &r):v(r.v) { }

	MECANIM_FORCE_INLINE bool4(const packed_type &r):v(r) { }

	explicit MECANIM_FORCE_INLINE bool4(bool s):v(Vloadsb(s)) {  }

	explicit MECANIM_FORCE_INLINE bool4(bool1 const& r):v(r.s) {  }

	MECANIM_FORCE_INLINE bool4(bool x, bool y, bool z, bool w):v(Vload4sb(x,y,z,w)) { }

	MECANIM_FORCE_INLINE bool4 &operator=(const bool4 &r) { v = r.v; return *this; }

	MECANIM_FORCE_INLINE bool4 &operator=(bool s) { v = Vloadsb(s); return *this; }		

	MECANIM_FORCE_INLINE bool4 operator!() const { bool4 r = Vnot(v); return r; }
};

static MECANIM_FORCE_INLINE bool4 operator==(bool4 const& l, bool4 const& r)
{ 
	return bool4(Vxnor(l.v, r.v) ); 
}

static MECANIM_FORCE_INLINE bool4 operator!=(bool4 const& l, bool4 const& r)
{ 
	return bool4(Vxor(l.v, r.v) ); 
}

static MECANIM_FORCE_INLINE bool4 operator&&(bool4 const& l, bool4 const& r)
{ 
	return bool4(Vand(l.v, r.v) ); 
}

static MECANIM_FORCE_INLINE bool4 operator||(bool4 const& l, bool4 const& r)
{ 
	return bool4(Vor(l.v, r.v) ); 
}

}

#endif
