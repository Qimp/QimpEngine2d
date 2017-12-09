#ifndef SIMD_INTRINSIC_H
#define SIMD_INTRINSIC_H

/* Here the Math library back-end interface
	When you declare a function always returns results by values, you want to be sure that simd register stay in register, otherwise you may get poor performance if the CPU need to push back the register into memory
	Vector data is declared purely, ex: typedef __m128 vec4f. most compile won't recognize encapsulated vector type in class and thus generate more temporary and push back vector in memory. 

	to support a new platform you need at least to support this function set
	
	typedef __m128	vec4f;	// vector 4 float packed 
	typedef __m128	vec4fs;	// vector 4 float scalar
	typedef __m128	vec4b;	// vector 4 bool packed
	typedef __m128	vec4bs;	// vector 4 bool scalar

	#define Vzero()
	#define Vone()
	#define Vpermute(v, mask) 
	#define Vmove(l, r)

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


	// Aligned store, store vector at adress base as 4 float
	#define Vstorepf(v, base, offset)

	// Return component x as a float
	#define Vstoresf(r)

	// Return component x as a bool
	#define Vstoresb(r)				

	// Aligned store, store vector at adress base as 4 bool 
	#define Vstorepb(vec4f v, bool* r)	

	// Aligned load, load 4 float at adress v in vector register
	#define Vloadpf(v, offset)				

	// Load float value in vector register and replicate value in all component
	#define Vloadsf(s)

	// Load bool value in vector register and replicate value in all component
	#define Vloadsb(s)	

	// Load 4 float value in vector register
	#define Vload4sf(x, y, z, w)

	// Load 4 bool value in vector register
	#define Vload4sb( x, y, z, w) 

	#define Vadd(l, r)
	#define Vsub( l,  r)
	#define Vmul( l,  r)
	#define Vdiv( l,  r)
	#define Vmadd( a,  b,  c)
	#define Vmsub( a,  b,  c)
	#define Vneg(r)

	// Vector sgn: return -1, 1
	#define Vsgn(r)

	// Vector sgn: return -1, 0, 1
	#define Vsign(r)

	#define Vinc(r)
	#define Vdec(r)
	#define Vabs(r)
	#define Vmax( l,  r)
	#define Vmin( l,  r)

	// Return the largest of the 4 component
	#define Vlargest(r)

	// Return the smallest of the 4 component
	#define Vsmallest(r)
	#define Vsum(r)
	#define Vdot( l,  r)
	#define Vsqrt(r)

	#define Vrsqrt(r)
	#define Vrcp(r)

	// Merge 4 vector low bytes
	#define Vcombine(x,y,z,w)

	// Vector comparison	
	#define Vcmpeq( a,  b)
	#define Vcmpneq( a,  b)
	#define Vcmpgt( a,  b)
	#define Vcmpge( a,  b)
	#define Vcmplt( a,  b)
	#define Vcmple( a,  b)

	#define Vsel( c,  a,  b)
	
	//	vector logics 
	#define Vnot(r)
	#define Vxnor( a,  b)
	#define Vxor( a,  b)
	#define Vand( a,  b)
	#define Vor( a,  b)
	#define Vall(a)
	#define Vany( a)

*/
#if defined(__INTEL_COMPILER) || defined(__ICL) || defined(_MSC_VER)
	#include <cstddef>
	#define ATTRIBUTE_ALIGN(a)		__declspec(align(a))
	#define ALIGN4F					16
	#define MECANIM_FORCE_INLINE	__forceinline
#elif defined(__GNUC__)
	#include <cstddef>
	#if ((__GNUC__ >= 3) && (__GNUC_MINOR__ >= 1)) || (__GNUC__ >= 4)
        #ifdef _DEBUG
            #ifndef MECANIM_FORCE_INLINE
                #define MECANIM_FORCE_INLINE		inline 
            #endif 
        #else
            #ifndef MECANIM_FORCE_INLINE
                #define MECANIM_FORCE_INLINE		inline __attribute__((always_inline))
            #endif
        #endif
	#endif

	#define ATTRIBUTE_ALIGN(a)			__attribute__ ((aligned(a)))
	#define ALIGN4F						16
#endif

#ifndef MECANIM_FORCE_INLINE	
	#define MECANIM_FORCE_INLINE		inline
#endif

#ifndef ATTRIBUTE_ALIGN
	#define ATTRIBUTE_ALIGN(a)
#endif

#ifndef ALIGN4F
	#define ALIGN4F						16
#endif

// #if FORCE_FPU
// 	#include "math/Simd/fpu.h"
// #elif PLATFORM_XENON
// 	#include "math/Simd/xenon.h"
// #elif PLATFORM_PS3
// 	#include "math/Simd/ppu.h"
// #elif (AX_TARGET_PLATFORM == AX_PLATFORM_WIN32) && SUPPORTS_SSE
// 	#include "math/Simd/sse.h"
// #elif PLATFORM_OSX
// 	#include "math/Simd/sse.h"
// #elif SUPPORTS_NEON && (AX_TARGET_PLATFORM != AX_PLATFORM_ANDROID)
// 	#include "math/Simd/neon.h"
// #else
	#include "math/Simd/fpu.h"
//#endif

#endif

