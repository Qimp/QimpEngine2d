#ifndef __MATH_GRADIENT_H__
#define __MATH_GRADIENT_H__

QIMP_BEGIN_NAMESPACE

enum
{
	kGradientMaxNumKeys = 8,
	kOptimizedGradientMaxNumKeys = kGradientMaxNumKeys + kGradientMaxNumKeys, // color keys + alpha keys
};

// Optimized version of gradient
struct OptimizedGradient
{
	static inline uint32 InverseLerpWordOptimized (uint32 from, uint32 rcp, uint32 v)
	{
		Assert((from & 0xffff) == from);
		Assert((v & 0xffff) == v);
		return ((v - from) * rcp)>>16;
	}

	inline Color32 Evaluate(float normalizedTime) const
	{
		Assert((normalizedTime >= 0.0f) && (normalizedTime <= 1.0f));
		Assert(keyCount >= 2);

		uint32 time = NormalizedToWord(normalizedTime);

		// Color blend
		const uint32 numKeys = keyCount;
		time = std::min(std::max((uint32)times[0], time), (uint32)times[keyCount-1]); // TODO: Is this necessary?
		for (uint32 i = 1; i < numKeys; i++)
		{
			const uint32 currTime = times[i];
			if(time <= currTime)
			{
				const uint32 prevTime = times[i-1];
				const uint32 frac = InverseLerpWordOptimized(prevTime, rcp[i], time);
				return Lerp (colors[i-1], colors[i], frac);
			}
		}
		return Color32 (0xff,0xff,0xff,0xff);
	}

	Color32 colors[kOptimizedGradientMaxNumKeys];
	uint32 times[kOptimizedGradientMaxNumKeys];
	uint32 rcp[kOptimizedGradientMaxNumKeys]; // precomputed reciprocals
	uint32 keyCount;
};

// Work in progress (Rename NEW to something else when found..)
class GradientNEW
{
public:
	GradientNEW ();
	~GradientNEW ();

	//DECLARE_SERIALIZE_NO_PPTR (GradientNEW)

	Color32 Evaluate(float time) const;

	struct ColorKey
	{
		//DEFINE_GET_TYPESTRING (GradientColorKey)

		ColorKey () {}
		ColorKey (Colorf color, float time) {m_Color = color; m_Time = time;}
		Colorf	m_Color;
		float		m_Time;
	};

	struct AlphaKey
	{
		//DEFINE_GET_TYPESTRING (GradientAlphaKey)

		AlphaKey () {}
		AlphaKey (float alpha, float time) {m_Alpha = alpha; m_Time = time;}
		float		m_Alpha;
		float		m_Time;
	};

	void SetKeys (ColorKey* colorKeys, unsigned numColorKeys, AlphaKey* alphaKeys, unsigned numAlphaKeys);

	void SetColorKeys (ColorKey* colorKeys, unsigned int numKeys);
	void SetAlphaKeys (AlphaKey* alphaKeys, unsigned int numKeys);

	void SetNumColorKeys (int numColorKeys) { m_NumColorKeys = numColorKeys;};
	void SetNumAlphaKeys (int numAlphaKeys) { m_NumAlphaKeys = numAlphaKeys; };

	int GetNumColorKeys () const { return m_NumColorKeys; }
	int GetNumAlphaKeys () const { return m_NumAlphaKeys; }

	Color32& GetKey (unsigned index) { return m_Keys[index]; }
	const Color32& GetKey (unsigned index) const { return m_Keys[index]; }

	uint16& GetColorTime (unsigned index) { return m_ColorTime[index]; }
	const uint16& GetColorTime (unsigned index) const { return m_ColorTime[index]; }
	
	uint16& GetAlphaTime(unsigned index) { return m_AlphaTime[index]; }
	const uint16& GetAlphaTime(unsigned index) const { return m_AlphaTime[index]; }

	Color32 GetConstantColor () const;
	void SetConstantColor (Color32 color);

	void SwapColorKeys (int i, int j);
	void SwapAlphaKeys (int i, int j);

	void InitializeOptimized(OptimizedGradient& g); 

private:
	static inline uint32 InverseLerpWord (uint32 from, uint32 to, uint32 v)
	{
		Assert((from & 0xffff) == from);
		Assert((to & 0xffff) == to);
		Assert((v & 0xffff) == v);
		Assert (from <= to);
		uint32 nom = (v - from) << 16;
		uint32 den = std::max<uint32>(to - from, 1);
		uint32 res = nom / den;
		return res;
	}

	static inline uint32 LerpByte(uint32 u0, uint32 u1, uint32 scale)
	{
		Assert((u0 & 0xff) == u0);
		Assert((u1 & 0xff) == u1);
		//Assert((scale & 0xff) == scale);
		return u0 + (((u1 - u0) * scale) >> 8) & 0xff;
	}
	
	void ValidateColorKeys();
	void ValidateAlphaKeys();

	Color32 m_Keys[kGradientMaxNumKeys];
	uint16 m_ColorTime[kGradientMaxNumKeys]; 
	uint16 m_AlphaTime[kGradientMaxNumKeys];
	uint8 m_NumColorKeys;
	uint8 m_NumAlphaKeys;
};

inline Color32 GradientNEW::Evaluate(float normalizedTime) const
{
	Assert((normalizedTime >= 0.0f) && (normalizedTime <= 1.0f));
	Assert(m_NumColorKeys >= 2);
	Assert(m_NumAlphaKeys >= 2);
	
	Color32 color = Color32 (0xff,0xff,0xff,0xff);
	const uint32 time = NormalizedToWord(normalizedTime);
	
	// Color blend
	const uint32 numColorKeys = m_NumColorKeys;
	const uint32 timeColor = std::min(std::max((uint32)m_ColorTime[0], time), (uint32)m_ColorTime[numColorKeys-1]);
	for (uint32 i = 1; i < numColorKeys; i++)
	{
		const uint32 currTime = m_ColorTime[i];
		if(timeColor <= currTime)
		{
			const uint32 prevTime = m_ColorTime[i-1];
			const uint32 frac = InverseLerpWord(prevTime, currTime, timeColor) >> 8; // frac is byte
			color = Lerp (m_Keys[i-1], m_Keys[i], frac);
			break;
		}
	}
	
	// Alpha blend
	const uint32 numAlphaKeys = m_NumAlphaKeys;
	const uint32 timeAlpha = std::min(std::max((uint32)m_AlphaTime[0], time), (uint32)m_AlphaTime[numAlphaKeys-1]);
	for (uint32 i = 1; i < numAlphaKeys; i++)
	{
		const uint32 currTime = m_AlphaTime[i];
		if(timeAlpha <= currTime)
		{
			const uint32 prevTime = m_AlphaTime[i-1];
			const uint32 frac = InverseLerpWord(prevTime, currTime, timeAlpha) >> 8; // frac is byte
			color.a = LerpByte(m_Keys[i-1].a, m_Keys[i].a, frac);
			break;
		}
	}
	
	return color;
}

/// Simple class to interpolate between colors.
template<int size>
class GradientDeprecated
 {
	public:

		/// Get a color
		Color32 &operator[] (int i) { Assert (i < 0 || i >= size); return m_Colors[i]; }
		/// Get a color
		const Color32 &operator[] (int i) const { Assert (i < 0 || i >= size);  return m_Colors[i]; }

		/// Get the color value at a given position
		/// @param position a position in unnormalized 16.16 bit fixed
		Color32 GetFixed (uint32 position) const {
			Assert ((position >> 16) >= size - 1);
			return Lerp (m_Colors[position >> 16], m_Colors[(position >> 16) + 1], (position >> 8) & 255);
		}
		
	private:
		/// The array of colors this interpolator works through
		Color32 m_Colors[size];
};

QIMP_END_NAMESPACE

#endif //__MATH_GRADIENT_H__