#ifndef __MATH_RAND_H__
#define __MATH_RAND_H__

QIMP_BEGIN_NAMESPACE

/*
Some random generator timings:
MacBook Pro w/ Core 2 Duo 2.4GHz. Times are for gcc 4.0.1 (OS X 10.6.2) / VS2008 SP1 (Win XP SP3),
in milliseconds for this loop (4915200 calls):

 for (int j = 0; j < 100; ++j)
   for (int i = 0; i < 128*128*3; ++i)
     data[i] = (rnd.get() & 0x3) << 6;

                  gcc   vs2008    Size
C's rand():       57.0  109.3 ms     1
Mersenne Twister: 56.0   37.4 ms  2500
Unity 2.x LCG:    11.1    9.2 ms     4
Xorshift 128:     15.0   17.8 ms    16
Xorshift 32:      20.6   10.7 ms     4
WELL 512:         43.6   55.1 ms    68 
*/


// Xorshift 128 implementation
// Xorshift paper: http://www.jstatsoft.org/v08/i14/paper
// Wikipedia: http://en.wikipedia.org/wiki/Xorshift
class Rand {
public:
	
	Rand (uint32 seed = 0)
	{
		SetSeed (seed);
	}
	
	uint32 Get ()
	{
		uint32 t;
		t = x ^ (x << 11);
		x = y; y = z; z = w;
		return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
	}

	inline static float GetFloatFromInt (uint32 value)
	{
		// take 23 bits of integer, and divide by 2^23-1
		return float(value & 0x007FFFFF) * (1.0f / 8388607.0f);
	}

	inline static uint8 GetByteFromInt (uint32 value)
	{
		// take the most significant byte from the 23-bit value
		return uint8(value >> (23 - 8));
	}
	
	// random number between 0.0 and 1.0
	float GetFloat ()
	{
		return GetFloatFromInt (Get ());
	}

	// random number between -1.0 and 1.0
	float GetSignedFloat ()
	{
	    return GetFloat() * 2.0f - 1.0f;
	}

	void SetSeed (uint32 seed)
	{
		x = seed;
		y = x * 1812433253U + 1;
		z = y * 1812433253U + 1;
		w = z * 1812433253U + 1;
	}
	
	uint32 GetSeed () const { return x; }
	
private:
	uint32 x, y, z, w;
};

QIMP_END_NAMESPACE

#endif//__MATH_RAND_H__
