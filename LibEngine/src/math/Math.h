#ifndef __MATH_MATH_H__
#define __MATH_MATH_H__

#include "math/Common.h"

QIMP_BEGIN_NAMESPACE

#ifdef min 
#undef min
#endif

#ifdef max
#undef max
#endif


#if defined(__GNUC__) && defined(__ppc__)
#	include <ppc_intrinsics.h>
#endif

//#ifndef PI
//#define PI 3.14159265358979323846264338327950288419716939937510
//#endif


const float BIGGEST_FLOAT_SMALLER_THAN_ONE = 0.99999994f;
const double BIGGEST_DOUBLE_SMALLER_THAN_ONE = 0.99999999999999989;

inline float FloatMin(float a, float b)
{
	return std::min(a, b);
}

inline float FloatMax(float a, float b)
{
	return std::max(a, b);
}

template<class T>
inline T Abs (T v)
{
	return v < 0 ? -v : v;
}

template<>
inline float Abs (float v)
{
	return v < 0 ? -v : v;
}

template<>
inline double Abs (double v)
{
	return v < 0 ? -v : v;
}

inline int FloorfToInt (float f)
{
	AssertIf (f < INT_MIN || f > INT_MAX);
	return f >= 0 ? (int)f : (int)(f - BIGGEST_FLOAT_SMALLER_THAN_ONE);
}

inline uint32 FloorfToIntPos (float f)
{
	AssertIf (f < 0 || f > UINT_MAX);
	return (uint32)f;
}

inline float Floorf (float f)
{
	// Use std::floor().
	// We are interested in reliable functions that do not lose precision.
	// Casting to int and back to float would not be helpful.
	return floor (f);
}

inline double Floord (double f)
{
	// Use std::floor().
	// We are interested in reliable functions that do not lose precision.
	// Casting to int and back to float would not be helpful.
	return floor (f);
}


inline int CeilfToInt (float f)
{
	AssertIf (f < INT_MIN || f > INT_MAX);
	return f >= 0 ? (int)(f + BIGGEST_FLOAT_SMALLER_THAN_ONE) : (int)(f);
}

inline uint32 CeilfToIntPos (float f)
{
	AssertIf (f < 0 || f > UINT_MAX);
	return (uint32)(f + BIGGEST_FLOAT_SMALLER_THAN_ONE);
}

inline float Ceilf (float f)
{
	// Use std::ceil().
	// We are interested in reliable functions that do not lose precision.
	// Casting to int and back to float would not be helpful.
	return ceil (f);
}

inline double Ceild (double f)
{
	// Use std::ceil().
	// We are interested in reliable functions that do not lose precision.
	// Casting to int and back to float would not be helpful.
	return ceil (f);
}


inline int RoundfToInt (float f)
{
	return FloorfToInt (f + 0.5F);
}

inline uint32 RoundfToIntPos (float f)
{
	return FloorfToIntPos (f + 0.5F);
}

inline float Roundf (float f)
{
	return Floorf (f + 0.5F);
}

inline double Roundf (double f)
{
	return Floord (f + 0.5);
}


///  Fast conversion of float [0...1] to 0 ... 65535
inline int NormalizedToWord (float f)
{
	f = FloatMax (f, 0.0F);
	f = FloatMin (f, 1.0F);
	return RoundfToIntPos (f * 65535.0f);
}

///  Fast conversion of float [0...1] to 0 ... 65535
inline float WordToNormalized (int p)
{
	AssertIf(p < 0 || p > 65535);
	return (float)p / 65535.0F;
}

///  Fast conversion of float [0...1] to 0 ... 255
inline int NormalizedToByte (float f)
{
	f = FloatMax (f, 0.0F);
	f = FloatMin (f, 1.0F);
	return RoundfToIntPos (f * 255.0f);
}

///  Fast conversion of float [0...1] to 0 ... 255
inline float ByteToNormalized (int p)
{
	AssertIf(p < 0 || p > 255);
	return (float)p / 255.0F;
}


inline int Repeat (int t, int length)
{
	return t - (t / length) * length;
}

inline int PingPong (int t, int length)
{
	t = Repeat (t, length * 2);
	t = length - Abs (t - length);
	return t;
}

// Returns float remainder for t / length
inline float RepeatF (float t, float length)
{
	return t - Floorf (t / length) * length;
}

// Returns double remainder for t / length
inline double RepeatD (double t, double length)
{
	return t - floor (t / length) * length;
}

// Returns relative angle on the interval (-pi, pi]
inline float DeltaAngleRad (float current, float target)
{
	float delta = RepeatF ((target - current), 2.0f * (float)PI);
	if (delta > (float)PI)
		delta -= 2.0f * (float)PI;
	return delta;
}

// Returns true if the distance between f0 and f1 is smaller than epsilon
inline bool CompareApproximately (float f0, float f1, float epsilon = 0.000001F)
{
	float dist = (f0 - f1);
	dist = Abs (dist);
	return dist < epsilon;
}

/// CopySignf () returns x with its sign changed to y's.
inline float CopySignf (float x, float y)
{
	union
	{
		float f;
		uint32 i;
	} u, u0, u1;
	u0.f = x; u1.f = y;
	uint32 a    = u0.i;
	uint32 b    = u1.i;
	int32 mask = 1 << 31;
	uint32 sign = b & mask;
	a &= ~mask;
	a |= sign;

	u.i = a;
	return u.f;
}

inline int CompareFloatRobustSignUtility (float A)
{
	// The sign bit of a number is the high bit.
	union
	{
		float f;
		int i;
	} u;
	u.f = A;
	return (u.i) & 0x80000000;
}

inline bool CompareFloatRobust (float f0, float f1, int maxUlps = 10)
{
	// After adjusting floats so their representations are lexicographically
	// ordered as twos-complement integers a very small positive number
	// will compare as 'close' to a very small negative number. If this is
	// not desireable, and if you are on a platform that supports
	// subnormals (which is the only place the problem can show up) then
	// you need this check.
	// The check for A == B is because zero and negative zero have different
	// signs but are equal to each other.
	if (CompareFloatRobustSignUtility(f0) != CompareFloatRobustSignUtility(f1))
		return f0 == f1;

	union
	{
		float f;
		int i;
	} u0, u1;
	u0.f = f0;
	u1.f = f1;
	int aInt = u0.i;
	// Make aInt lexicographically ordered as a twos-complement int
	if (aInt < 0)
		aInt = 0x80000000 - aInt;
	// Make bInt lexicographically ordered as a twos-complement int
	int bInt = u1.i;
	if (bInt < 0)
		bInt = 0x80000000 - bInt;

	// Now we can compare aInt and bInt to find out how far apart A and B
	// are.
	int intDiff = Abs (aInt - bInt);
	if (intDiff <= maxUlps)
		return true;
	return false;
}

// Returns the t^2
template<class T>
T Sqr (const T& t)
{
	return t * t;
}

#define kDeg2Rad (2.0F * PI / 360.0F)
#define kRad2Deg (1.f / kDeg2Rad)

inline float Deg2Rad (float deg)
{
	// TODO : should be deg * kDeg2Rad, but can't be changed, 
	// because it changes the order of operations and that affects a replay in some RegressionTests
	return deg / 360.0f * 2.0f * (float)PI;
}

inline float Rad2Deg (float rad)
{
	// TODO : should be rad * kRad2Deg, but can't be changed, 
	// because it changes the order of operations and that affects a replay in some RegressionTests
	return rad / 2.0f / (float)PI * 360.0f;
}

inline float Lerp (float from, float to, float t)
{
	return to * t + from * (1.0F - t);
}

inline bool IsNAN (float value)
{
#if defined __APPLE_CC__
	return value != value;
#elif _MSC_VER
	return _isnan(value) != 0;
#else
	return isnan (value);
#endif
}

inline bool IsNAN (double value)
{
#if defined __APPLE_CC__
	return value != value;
#elif _MSC_VER
	return _isnan(value) != 0;
#else
	return std::isnan (value);
#endif
}

inline bool IsPlusInf(float value)		{ return value == std::numeric_limits<float>::infinity (); }
inline bool IsMinusInf(float value)		{ return value == -std::numeric_limits<float>::infinity ();	}

inline bool IsFinite(const float& value)
{
	// Returns false if value is NaN or +/- infinity
	uint32 intval = *reinterpret_cast<const uint32*>(&value);
	return (intval & 0x7f800000) != 0x7f800000;
}

inline bool IsFinite(const double& value)
{
	// Returns false if value is NaN or +/- infinity
	uint64 intval = *reinterpret_cast<const uint64*>(&value);
	return (intval & 0x7ff0000000000000LL) != 0x7ff0000000000000LL;
}

inline float InvSqrt (float p) { return 1.0F / sqrt (p); }
inline float Sqrt (float p) { return sqrt (p); }

/// - Almost highest precision sqrt
/// - Returns 0 if value is 0 or -1
/// inline float FastSqrt (float value)

/// - Almost highest precision inv sqrt
/// - if value == 0 or -0 it returns 0.
/// inline float FastInvSqrt (float value)

/// - Low precision inv sqrt approximately
/// - if value == 0 or -0 it returns nan or undefined
/// inline float FastestInvSqrt (float value)

inline float FastSqrt (float value)
{
	return sqrtf (value);
}

inline float FastInvSqrt( float f ) 
{
	// The Newton iteration trick used in FastestInvSqrt is a bit faster on
	// Pentium4 / Windows, but lower precision. Doing two iterations is precise enough,
	// but actually a bit slower.
	if (fabs(f) == 0.0f)
		return f;
	return 1.0f / sqrtf (f);
}

inline float FastestInvSqrt( float f )
{
	union
	{
		float f;
		int i;
	} u;
	float fhalf = 0.5f*f;
	u.f = f;
	int i = u.i;
	i = 0x5f3759df - (i>>1);
	u.i = i;
	f = u.f;
	f = f*(1.5f - fhalf*f*f);
	// f = f*(1.5f - fhalf*f*f); // uncommenting this would be two iterations
	return f;
}

inline float Sin (float f)
{
	return sinf (f);
}

inline float Pow (float f, float f2)
{
	return powf (f, f2);
}

inline float Cos (float f)
{
	return cosf (f);
}

inline float Sign (float f)
{
#if defined(_XBOX)
	return __fsel(f, 1.0f, -1.0f);
#else
	if (f < 0.0F)
		return -1.0F;
	else
		return 1.0;
#endif
}

static void HalfToFloat(uint16 src, float& dest)
{
	// Integer alias
	uint32& bits = *reinterpret_cast<uint32*>(&dest);

	// Based on Fabian Giesen's public domain half_to_float_fast3
	static const uint32 magic = { 113 << 23 };
	const float& magicFloat = *reinterpret_cast<const float*>(&magic);
	static const uint32 shiftedExp = 0x7c00 << 13; // exponent mask after shift

	// Mask out sign bit
	bits = src & 0x7fff;
	if (bits)
	{
		// Move exponent + mantissa to correct bits
		bits <<= 13;
		uint32 exponent = bits & shiftedExp;
		if (exponent == 0)
		{
			// Handle denormal
			bits += magic;
			dest -= magicFloat;
		}
		else if (exponent == shiftedExp) // Inf/NaN
			bits += (255 - 31) << 23;
		else
			bits += (127 - 15) << 23;
	}

	// Copy sign bit
	bits |= (src & 0x8000) << 16;
}


// On non-C99 platforms log2 is not available, so approximate it.
// #if (AX_TARGET_PLATFORM == AX_PLATFORM_WIN32 || AX_TARGET_PLATFORM == AX_PLATFORM_ANDROID)
#define kNaturalLogarithm2 0.693147180559945309417
#define Log2(x) (logf(x) / kNaturalLogarithm2)
//#else
//#define Log2(x) log2f(x)
//#endif



template<class T>
inline T	Clamp(const T val, const T minV, const T maxV) { return ((val) < (minV) ? (minV) : ((val > maxV) ? (maxV) : (val))); };

int 		NextPow2 ( int a );
uint32		ILog2(uint32 v);
float		Normalize(float value, float min, float max);
float		Map(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp = false);
float		Dist(float x1, float y1, float x2, float y2);
float		DistSquared(float x1, float y1, float x2, float y2);
bool		InRange(float t, float min, float max);

float		RadToDeg(float radians);
float		DegToRad(float degrees);
float 		LerpDegrees(float currentAngle, float targetAngle, float pct);
float 		LerpRadians(float currentAngle, float targetAngle, float pct);
float 		AngleDifferenceDegrees(float currentAngle, float targetAngle);
float 		AngleDifferenceRadians(float currentAngle, float targetAngle);
float 		AngleSumRadians(float currentAngle, float targetAngle);
float		Wrap(float value, float from, float to);
float		WrapRadians(float angle, float from = -PI, float to=+PI);
float		WrapDegrees(float angle, float from = -180, float to=+180);

template<typename Type>
Type		InterpolateCosine(Type y1, Type y2, float pct);
template<typename Type>
Type		InterpolateCubic(Type y0, Type y1, Type y2, Type y3, float pct);
template<typename Type>
Type		InterpolateCatmullRom(Type y0, Type y1, Type y2, Type y3, float pct);
template<typename Type>
Type		InterpolateHermite(Type y0, Type y1, Type y2, Type y3, float pct);
template<typename Type>
Type		InterpolateHermite(Type y0, Type y1, Type y2, Type y3, float pct, float tension, float bias);


//--------------------------------------------------
template<typename Type>
Type		InterpolateCosine(Type y1, Type y2, float pct)
{
	float pct2;

	pct2 = (1-cos(pct*PI))/2;
	return(y1*(1-pct2)+y2*pct2);
}

//--------------------------------------------------
template<typename Type>
Type		InterpolateCubic(Type y0, Type y1, Type y2, Type y3, float pct)
{
	Type a0,a1,a2,a3;
	float pct2;

	pct2 = pct*pct;
	a0 = y3 - y2 - y0 + y1;
	a1 = y0 - y1 - a0;
	a2 = y2 - y0;
	a3 = y1;

	return(a0*pct*pct2+a1*pct2+a2*pct+a3);
}

//--------------------------------------------------
template<typename Type>
Type		InterpolateCatmullRom(Type y0, Type y1, Type y2, Type y3, float pct)
{
	Type a0,a1,a2,a3;
	float pct2 = pct*pct;
	a0 = -0.5*y0 + 1.5*y1 - 1.5*y2 + 0.5*y3;
	a1 = y0 - 2.5*y1 + 2*y2 - 0.5*y3;
	a2 = -0.5*y0 + 0.5*y2;
	a3 = y1;
	return(a0*pct*pct2 + a1*pct2 + a2*pct +a3);
}

// laurent de soras
//--------------------------------------------------
template<typename Type>
inline Type InterpolateHermite(Type y0, Type y1, Type y2, Type y3, float pct)
{
	const Type c = (y2 - y0) * 0.5f;
	const Type v = y1 - y2;
	const Type w = c + v;
	const Type a = w + v + (y3 - y1) * 0.5f;
	const Type b_neg = w + a;

	return ((((a * pct) - b_neg) * pct + c) * pct + y1);
}

//--------------------------------------------------
template<typename Type>
Type		InterpolateHermite(Type y0, Type y1, Type y2, Type y3, float pct, float tension, float bias)
{
	float pct2,pct3;
	Type m0,m1;
	Type a0,a1,a2,a3;

	pct2 = pct * pct;
	pct3 = pct2 * pct;
	m0  = (y1-y0)*(1+bias)*(1-tension)/2;
	m0 += (y2-y1)*(1-bias)*(1-tension)/2;
	m1  = (y2-y1)*(1+bias)*(1-tension)/2;
	m1 += (y3-y2)*(1-bias)*(1-tension)/2;
	a0 =  Type(2*pct3 - 3*pct2 + 1);
	a1 =  Type(pct3 - 2*pct2 + pct);
	a2 =  Type(pct3 -   pct2);
	a3 =  Type(-2*pct3 + 3*pct2);

	return(a0*y1+a1*m0+a2*m1+a3*y2);
}


template <class T>
inline T Saturate (const T& t)
{
	return t < 0 ? 0 : (t > 1 ? 1 : t);
}

template <>
inline float Saturate<float> (const float& t)
{
	return t < 0.0f ? 0.0f : (t > 1.0f ? 1.0f : t);
}

inline float SmoothStep (float from, float to, float t) 
{
	t = Saturate(t);
	t = -2.0f * t*t*t + 3.0f * t*t;
	return to * t + from * (1.0f - t);
}
// Rounds value down.
// Note: base must be power of two value.
inline uint32 RoundDown (uint32 value, int32 base)
{
	return value & (-base);
}
// Rounds value up.
// Note: base must be power of two value.
inline uint32 RoundUp (uint32 value, int32 base)
{
	return (value + base - 1) & (-base);
}

template<class T>
inline T* Stride (T* p, size_t offset)
{
	return reinterpret_cast<T*>((char*)p + offset);
}

inline float UVDecode(uint8 value0, uint8 value1)
{
	uint8 x = uint8(value0);
	uint8 y = uint8(value1);

	return (y + x / 256.0f)*0.5f - 64.0f;
}

inline float UV2Decode(uint8 value0, uint8 value1)
{
	uint16 value = (uint16)value0 | ((uint16)value1 << 8);
	return value / 65535.0f;
}

inline void UV2Encode(float value, uint8* value0, uint8* value1)
{
	uint16 v = int(std::min(std::max(0.0f, value), 1.0f) * 65535.0f);
	*value0 = int8(v);
	*value1 = int8(v >> 8);
}

//根据x,y的正负，返回一个0~2pi的值。（默认的atan返回-pi/2~pi/2的值）
inline float AtanEx(float x, float y)
{
	if (fabs(x) < EPSILON)
	{
		Assert(fabs(y) > EPSILON);
		if (y > 0)
			return (float)PI*0.5f;
		else
			return (float)PI*1.5f;
	}
	else if (fabs(y) < EPSILON)
	{
		if (x > 0)
			return 0.0f;
		else
			return (float)PI;
	}

	float dir = atan(fabs(y / x));
	if (x < 0 && y > 0)
		dir = (float)PI - dir;
	else if (x < 0 && y < 0)
		dir = (float)PI + dir;
	else if (x > 0 && y < 0)
		dir = (float)PI * 2 - dir;
	return dir;
}

QIMP_END_NAMESPACE

#endif//__MATH_MATH_H__
