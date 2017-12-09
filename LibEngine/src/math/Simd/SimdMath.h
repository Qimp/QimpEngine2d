#pragma once

//#include "Runtime/Utilities/Prefetch.h"
//#include "Runtime/Misc/CPUInfo.h"

#if SUPPORTS_VMX

#		define VMX_0X	0
#		define VMX_0Y	1
#		define VMX_0Z	2
#		define VMX_0W	3

#		define VMX_1X	4
#		define VMX_1Y	5
#		define VMX_1Z	6
#		define VMX_1W	7

#		define V4BuildPermuteMask(a,b,c,d)  {	\
					(((a)<<2)|((a)<<10)|((a)<<18)|((a)<<26))+0x00010203,\
					(((b)<<2)|((b)<<10)|((b)<<18)|((b)<<26))+0x00010203,\
					(((c)<<2)|((c)<<10)|((c)<<18)|((c)<<26))+0x00010203,\
					(((d)<<2)|((d)<<10)|((d)<<18)|((d)<<26))+0x00010203} 


#	if PLATFORM_XENON
#		include <Xtl.h>
#		define ALIGN16 __declspec(align(16))
		typedef __vector4 Simd128;

#		define vec_splat __vspltw
#		define vec_ste(vec, off, addr) __stvebx(vec, addr, off)

#		define V4Splat(v0, i) __vspltw((v0), (i))

#	elif PLATFORM_PS3
#		include <ppu_intrinsics.h>
#		define ALIGN16 __attribute__((aligned(16)))
#		define __forceinline __attribute__((always_inline))
		typedef vec_float4 Simd128;
		static const vec_float4 __vsignedzero = {-0.f,-0.f,-0.f,-0.f};


#		define __vzero()					((vec_float4)vec_splat_u32(0))

#		define __lvx(base, offset)			vec_lvx(offset, base)
#		define __lvlx(base, offset)			vec_lvlx(offset, base)
#		define __lvrx(base, offset)			vec_lvrx(offset, base)

#		define __stvx(value, base, offset)	vec_stvx((value), (offset), (float*)(base))
#		define __stvlx(value, base, offset) vec_stvlx((value), (offset), (float*)(base))
#		define __stvrx(value, base, offset) vec_stvrx((value), (offset), (float*)(base))

#		define __vmrglw(v0, v1)				vec_mergel((vec_float4)(v0), (vec_float4)(v1))
#		define __vmrghw(v0, v1)				vec_mergeh((vec_float4)(v0), (vec_float4)(v1))

#		define __vmulfp(a, b)				vec_madd( a, b, __vsignedzero)

#		define __vand						vec_and
#		define __vandc						vec_andc
#		define __vor						vec_or
#		define __vnor						vec_nor
#		define __vxor						vec_xor
#		define __vspltw						vec_splat
#		define __vmaddfp					vec_madd
#		define __vaddfp						vec_add
#		define __vsubfp						vec_sub
#		define __vperm						vec_perm
#		define __vnmsubfp					vec_nmsub
#		define __vminfp						vec_min
#		define __vmaxfp						vec_max
#		define __vrsqrtefp					vec_rsqrte
#		define __vsel						vec_sel
#		define __vrefp						vec_re

#		define __vcmpeqfp					vec_vcmpeqfp
#		define __vcmpgtfp					vec_vcmpgtfp
#		define __vcmpgefp					vec_vcmpgefp

		__forceinline static Simd128 __vmsum3fp(Simd128 v0, Simd128 v1)
		{
			const Simd128 m0 = vec_madd(v0, v1, __vsignedzero);
			const Simd128 m1 = vec_splat(m0, 0);
			const Simd128 m2 = vec_splat(m0, 1);
			const Simd128 m3 = vec_splat(m0, 2);
			return vec_add(vec_add(m1, m2), m3);
		}
		__forceinline static Simd128 __vmsum4fp(Simd128 v0, Simd128 v1)
		{
			const Simd128 m0 = vec_madd(v0, v1, __vsignedzero);
			const Simd128 m1 = vec_sld(m0, m0, 8);
			const Simd128 m2 = vec_add(m0, m1);
			const Simd128 m3 = vec_sld(m2, m2, 4);
			return vec_add(m2, m3);
		}

#	endif

	
#	if PLATFORM_PS3
		typedef vec_uchar16 Simd128Mask;
#	else
		typedef Simd128 Simd128Mask;
#	endif
		
		typedef ALIGN16 struct Simd128i { union { int i[4]; Simd128 v; };} Simd128i;

	// Load / Save
#	define V4Load(base, offset)					__lvx((base), sizeof(base)*(offset))
#	define V4LoadUnaligned(base, offset)			__vor(__lvlx((base), sizeof(base)*(offset)), __lvrx((base), (sizeof(base)*(offset)) + 16))
#	define V4Store(value, base, offset)			__stvx(value, (base), sizeof(base)*(offset))
#	define V4StoreUnaligned(value, base, offset)	__stvlx(value, (float*)(base), sizeof(base)*(offset)); __stvrx(value, (float*)(base), (sizeof(base)*(offset)) + 16 )

	// Math functions
#	define V4Zero()					__vzero()
#	define V4Add(v0, v1)			__vaddfp((v0), (v1))
#	define V4Sub(v0, v1)			__vsubfp((v0), (v1))
#	define V4Mul(v0, v1)			__vmulfp((v0), (v1))
#	define V4MulAdd(v0, v1, v2)		__vmaddfp((v0), (v1), (v2))
#	define V4Min(v0, v1)			__vminfp((v0), (v1))
#	define V4Max(v0, v1)			__vmaxfp((v0), (v1))
#	define V4Rcp(v0)				__vrefp((v0))
#	define V4Rsqrt(v0)				__vrsqrtefp((v0))
#	define V3Dot(v0, v1)			__vmsum3fp((v0), (v1))
#	define V4Dot(v0, v1)			__vmsum4fp((v0), (v1))

	// Shuffling / Permuting / Splatting / Merging
#	define V4Splat(v0, i)			__vspltw((v0), (i))
#	define V4MergeL(v0, v1)			__vmrglw((v0), (v1))
#	define V4MergeH(v0, v1)			__vmrghw((v0), (v1))

	__forceinline static Simd128 V3Cross(Simd128 v0, Simd128 v1)
	{
		const static Simd128i maskYZXW = V4BuildPermuteMask(VMX_0Y, VMX_0Z, VMX_0X, VMX_0W);
		const Simd128Mask p = (Simd128Mask)maskYZXW.v;
		const Simd128 m0 = __vperm(v1, v1, p);
		const Simd128 m1 = __vperm(v0, v0, p);
		const Simd128 m2 = __vmulfp(v0, m0);
		const Simd128 m3 = __vnmsubfp(m1, v1, m2);
		return __vperm(m3, m3, p);
	}



#elif SUPPORTS_SSE

#	if (AX_TARGET_PLATFORM == AX_PLATFORM_WIN32)
#		include <intrin.h>
#		define ALIGN16 __declspec(align(16))
#	else
#		include <xmmintrin.h>
#		define ALIGN16 __attribute__((aligned(16)))
#		define __forceinline inline __attribute__((always_inline))
#	endif

	typedef __m128 Simd128;

	// Load / Save
#	define V4Load(base, offset)						_mm_load_ps((base)+(offset))
#	define V4LoadUnaligned(base, offset)			_mm_loadu_ps((base)+(offset))
#	define V4Store(value, base, offset)				_mm_store_ps((base)+(offset), value)
#	define V4StoreUnaligned(value, base, offset)	_mm_storeu_ps((base)+(offset), value)

	// Math functions
#	define V4Zero()				_mm_setzero_ps()
#	define V4Add(v0, v1)		_mm_add_ps((v0), (v1))
#	define V4Sub(v0, v1)		_mm_sub_ps((v0), (v1))
#	define V4Mul(v0, v1)		_mm_mul_ps((v0), (v1))
#	define V4MulAdd(v0, v1, v2)	_mm_add_ps(_mm_mul_ps((v0), (v1)), (v2))
#	define V4Min(v0, v1)		_mm_min_ps((v0), (v1))
#	define V4Max(v0, v1)		_mm_max_ps((v0), (v1))
#	define V4Rcp(v0)			_mm_rcp_ps((v0))
#	define V4Rsqrt(v0)			_mm_rsqrt_ps((v0))

	__forceinline static Simd128 V3Dot(Simd128 v0, Simd128 v1)
	{
		const Simd128 m0 = _mm_mul_ps(v0, v1);
		const Simd128 m1 = _mm_shuffle_ps(m0, m0, _MM_SHUFFLE(0,0,0,0));
		const Simd128 m2 = _mm_shuffle_ps(m0, m0, _MM_SHUFFLE(1,1,1,1));
		const Simd128 m3 = _mm_shuffle_ps(m0, m0, _MM_SHUFFLE(2,2,2,2));
		return _mm_add_ps(_mm_add_ps(m1, m2), m3);
	}
	__forceinline static Simd128 V4Dot(Simd128 v0, Simd128 v1)
	{
		const Simd128 m0 = _mm_mul_ps(v0, v1);
		const Simd128 m1 = _mm_shuffle_ps(m0, m0, _MM_SHUFFLE(2,3,0,1));
		const Simd128 m2 = _mm_add_ps(m0, m1);
		const Simd128 m3 = _mm_shuffle_ps(m2, m2, _MM_SHUFFLE(1,0,3,2));
		return _mm_add_ps(m2, m3);
	}
	__forceinline static Simd128 V3Cross(Simd128 v0, Simd128 v1)
	{
		const Simd128 m0 = _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(3,0,2,1));
		const Simd128 m1 = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(3,0,2,1));
		const Simd128 m2 = _mm_mul_ps(v1, m1);
		const Simd128 m3 = _mm_sub_ps(_mm_mul_ps(m0, v0), m2);
		return _mm_shuffle_ps(m3, m3, _MM_SHUFFLE(3,0,2,1));
	}

	// Shuffling / Permuting / Splatting / Merging
#	define V4Splat(v0, i)			_mm_shuffle_ps((v0), (v0), _MM_SHUFFLE(i,i,i,i))

	// Attention! : these are done after PPC big-endian specs.
#	define V4MergeL(v0, v1)			_mm_unpackhi_ps((v0), (v1))			
#	define V4MergeH(v0, v1)			_mm_unpacklo_ps((v0), (v1))
#endif

// Matrix & Quaternion types
struct ALIGN16 SimdMatrix3x4 { Simd128 m00, m10, m20; };
struct ALIGN16 SimdMatrix4x4 { Simd128 m00, m10, m20, m30;};
typedef Simd128 SimdQuaternion;

#if SUPPORTS_VMX

__forceinline static void V3StoreUnaligned(Simd128 value, float* volatile base, const UInt32 offset)
{
	const Simd128 X =  vec_splat(value, 0);
	const Simd128 Y =  vec_splat(value, 1);
	const Simd128 Z =  vec_splat(value, 2);
	vec_ste(X, 0, base+offset);
	vec_ste(Y, 4, base+offset);
	vec_ste(Z, 8, base+offset);
}

#else 

__forceinline static void V3StoreUnaligned(Simd128 value, float*  base, const UInt32 offset)
{
	typedef union {
		UInt32 u[4];
		__m128i v;
	} m128u;

	static const m128u store_mask={0xffffffff,0xffffffff,0xffffffff,0};
	_mm_maskmoveu_si128(*(__m128i *)&value, store_mask.v, (char*)(base+offset));
}

#endif



