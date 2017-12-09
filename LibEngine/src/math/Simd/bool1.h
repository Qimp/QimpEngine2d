#ifndef SIMD_BOOL1_H
#define SIMD_BOOL1_H

#include "Runtime/Math/Simd/intrinsic.h"

QIMP_BEGIN_NAMESPACE

struct ATTRIBUTE_ALIGN(ALIGN4F) bool1 
{
	enum {
		RHS_SWZ	= kXYZW
	};

	typedef bool   scalar_type;
	typedef vec4bs packed_type;

	packed_type	s;

	MECANIM_FORCE_INLINE bool1()
		{}

	MECANIM_FORCE_INLINE explicit bool1(packed_type x) : s(x)
		{}

	MECANIM_FORCE_INLINE explicit bool1(scalar_type x) : s(Vloadsb(x))
		{}

	MECANIM_FORCE_INLINE operator scalar_type() const
		{ return Vstoresb(s); }
	
	MECANIM_FORCE_INLINE bool1 &operator=(const bool1 &r)
		{ s = r.s; return *this; }

	// unary operators
	MECANIM_FORCE_INLINE bool1 operator!() const
		{ bool1 r = bool1(Vnot(s)); return r; }
};

}

#endif
