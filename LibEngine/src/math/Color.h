#ifndef __MATH_COLOR_H__
#define __MATH_COLOR_H__


QIMP_BEGIN_NAMESPACE

class Colorf
{
	public:
	float	r, g, b, a;

	//DEFINE_GET_TYPESTRING_IS_ANIMATION_CHANNEL (ColorRGBA)
	const static Colorf Zero;	// r=g=b=a=0
	const static Colorf Black;	// r=g=b=0, a=255
	const static Colorf Red;
	const static Colorf Green;
	const static Colorf Blue;
	const static Colorf Yellow;
	const static Colorf Magenta;
	const static Colorf Cyan;
	const static Colorf White;
	const static Colorf LtGrey;
	const static Colorf MdGrey;
	const static Colorf DkGrey;

	Colorf () {}
	
	Colorf (float inR, float inG, float inB, float inA = 1.0F) : r(inR), g(inG), b(inB), a(inA) {}
	explicit Colorf (const float* c) : r(c[0]), g(c[1]), b(c[2]), a(c[3]) {}

	//template<class TransferFunction> 
	//void Transfer (TransferFunction& transfer);

	void Set (float inR, float inG, float inB, float inA) {r = inR; g = inG; b = inB; a = inA;}
	
	void SetHex (uint32 hex)
	{
		Set(float( hex & 255)			/ 255.0f,
			float((hex >> 8 ) & 255)	/ 255.0f,
			float((hex >> 16) & 255)	/ 255.0f,
			float( hex >> 24)			/ 255.0f);
	}

	uint32 GetHex () const
	{
		uint32 hex = (NormalizedToByte(a) << 24) | (NormalizedToByte(b) << 16) | (NormalizedToByte(g) << 8) | NormalizedToByte(r);
		return hex;
	}
	
	float AverageRGB () const {return (r+g+b)*(1.0F / 3.0F);}
	float GreyScaleValue () const { return r * 0.30f + g * 0.59f  + b * 0.11f; }
	
	Colorf& operator = (const Colorf& in) { Set (in.r, in.g, in.b, in.a); return *this; }

	bool Equals(const Colorf& inRGB) const
	{
		return (r == inRGB.r && g == inRGB.g && b == inRGB.b && a == inRGB.a);
	}
	
	bool NotEquals(const Colorf& inRGB) const
	{
		return (r != inRGB.r || g != inRGB.g || b != inRGB.b || a != inRGB.a);
	}

	float* GetPtr ()				{return &r;}
	const float* GetPtr () const	{return &r;}

	Colorf& operator += (const Colorf &inRGBA)
	{
		r += inRGBA.r; g += inRGBA.g; b += inRGBA.b; a += inRGBA.a;
		return *this;
	}

	Colorf& operator *= (const Colorf &inRGBA)
	{
		r *= inRGBA.r; g *= inRGBA.g; b *= inRGBA.b; a *= inRGBA.a;
		return *this;
	}

private:
	// intentionally undefined
	bool operator == (const Colorf& inRGB) const;
	bool operator != (const Colorf& inRGB) const;
};


inline Colorf operator + (const Colorf& inC0, const Colorf& inC1)
{
	return Colorf (inC0.r + inC1.r, inC0.g + inC1.g, inC0.b + inC1.b, inC0.a + inC1.a);
}

inline Colorf operator - (const Colorf& inC0, const Colorf& inC1)
{
	return Colorf(inC0.r - inC1.r, inC0.g - inC1.g, inC0.b - inC1.b, inC0.a - inC1.a);
}

inline Colorf operator * (const Colorf& inC0, const Colorf& inC1)
{
	return Colorf (inC0.r * inC1.r, inC0.g * inC1.g, inC0.b * inC1.b, inC0.a * inC1.a);
}

inline Colorf operator * (float inScale, const Colorf& inC0)
{
	return Colorf (inC0.r * inScale, inC0.g * inScale, inC0.b * inScale, inC0.a * inScale);
}

inline Colorf operator * (const Colorf& inC0, float inScale)
{
	return Colorf (inC0.r * inScale, inC0.g * inScale, inC0.b * inScale, inC0.a * inScale);
}

inline Colorf Lerp (const Colorf& c0, const Colorf& c1, float t)
{
	return (1.0f - t) * c0 + t * c1; 
}

#define ARGB2ABGR(u) ((u & 0xff000000) | ((u & 0x00ff0000) >> 16) | (u & 0x0000ff00) | ((u & 0x000000ff) << 16))
#define ABGR2ARGB(u) ((u & 0xff000000) | ((u & 0x00ff0000) >> 16) | (u & 0x0000ff00) | ((u & 0x000000ff) << 16))
#define RGBA2ABGR(u) ((u & 0xff000000) >> 24 | ((u & 0x00ff0000) >> 8) | (u & 0x0000ff00) << 8 | ((u & 0x000000ff) << 24))

class Color32
{
public:
	uint8	r, g, b, a;

	const static Color32 Zero;	// r=g=b=a=0
	const static Color32 Black;	// r=g=b=0, a=255
	const static Color32 Red;
	const static Color32 Green;
	const static Color32 Blue;
	const static Color32 Yellow;
	const static Color32 Magenta;
	const static Color32 Cyan;
	const static Color32 White;
	const static Color32 LtGrey;
	const static Color32 MdGrey;
	const static Color32 DkGrey;

	//DEFINE_GET_TYPESTRING_IS_ANIMATION_CHANNEL (ColorRGBA)
	
	Color32 ()								{}
	
	Color32 (uint8 inR, uint8 inG, uint8 inB, uint8 inA)		{ r = inR; g = inG; b = inB; a = inA; }
	Color32 (uint32 c)						{ *(uint32*)this = c; }
	void Set (uint8 inR, uint8 inG, uint8 inB, uint8 inA)		{ r = inR; g = inG; b = inB; a = inA; }

	Color32 operator = (const Color32& c)	{ *(uint32*)this = *((uint32*)&c); return *this;}
	
	Color32 (const Colorf& c) { Set (c); }
	
	operator Colorf() const
	{
		float inv255 = 1.0f / 255.0f;
		return Colorf (r*inv255, g*inv255, b*inv255, a*inv255);
	}
	
	void operator = (const Colorf& c)
	{
		Set (c);
	}
	
	uint32 GetUint32 () { return *(uint32*)this; }

		void Set (const Colorf& c)
		{
			r = NormalizedToByte(c.r);
			g = NormalizedToByte(c.g);
			b = NormalizedToByte(c.b);
			a = NormalizedToByte(c.a);
		}

		uint8& operator [] (long i) {return GetPtr () [i];}
		const uint8& operator [] (long i)const {return GetPtr () [i];}

		bool operator == (const Color32& inRGB) const
		{
			return (r == inRGB.r && g == inRGB.g && b == inRGB.b && a == inRGB.a) ? true : false;
		}

		bool operator != (const Color32& inRGB) const
		{
			return (r != inRGB.r || g != inRGB.g || b != inRGB.b || a != inRGB.a) ? true : false;
		}

		uint8* GetPtr ()		{return &r;}
		const uint8* GetPtr ()const	{return &r;}

		inline Color32 operator * (float scale) const
		{
			return Color32((uint8)(r * scale + 0.5f), (uint8)(g * scale + 0.5f),
				(uint8)(b * scale + 0.5f), (uint8)(a * scale + 0.5f));
		}

		inline Color32 operator * (int scale) const
		{
			//Assert (scale < 0 || scale > 255);
			scale += 1;
			const uint32& u = reinterpret_cast<const uint32&> (*this);
			uint32 lsb = (((u & 0x00ff00ff) * scale) >> 8) & 0x00ff00ff;
			uint32 msb = (((u & 0xff00ff00) >> 8) * scale) & 0xff00ff00;
			lsb |= msb;
			return Color32 (lsb);
		}

	inline void operator *= (const Color32& inC1)
	{
#if 0
		r = (r * inC1.r) / 255;
		g = (g * inC1.g) / 255;
		b = (b * inC1.b) / 255;
		a = (a * inC1.a) / 255;
#else // This is much faster, but doesn't guarantee 100% matching result (basically color values van vary 1/255 but not at ends, check out unit test in cpp file).
		uint32& u = reinterpret_cast<uint32&> (*this);
		const uint32& v = reinterpret_cast<const uint32&> (inC1);
		uint32 result = (((u & 0x000000ff) * ((v & 0x000000ff) + 1)) >> 8) & 0x000000ff;
		result |= (((u & 0x0000ff00) >> 8) * (((v & 0x0000ff00) >> 8) + 1)) & 0x0000ff00;
		result |= (((u & 0x00ff0000) * (((v & 0x00ff0000) >> 16) + 1)) >> 8) & 0x00ff0000;
		result |= (((u & 0xff000000) >> 8) * (((v & 0xff000000) >> 24) + 1)) & 0xff000000;
		u = result;
#endif
}

	inline Color32 SwizzleToBGRA() const { return Color32(b, g, r, a); }
	inline Color32 SwizzleToBGR() const { return Color32(b, g, r, 255); }
	inline Color32 SwizzleToARGB() const { return Color32(a, r, g, b); }
	inline Color32 UnswizzleBGRA() const { return Color32(b, g, r, a); }
	inline Color32 UnswizzleARGB() const { return Color32(g, b, a, r); }
};

#if GFX_OPENGLESxx_ONLY
	inline Color32 SwizzleColorForPlatform(const Color32& col) { return col; }
	inline Color32 UnswizzleColorForPlatform(const Color32& col) { return col; }
#elif PLATFORM_XENON || PLATFORM_PS3 || PLATFORM_WII
	inline Color32 SwizzleColorForPlatform(const Color32& col) { return col.SwizzleToARGB(); }
	inline Color32 UnswizzleColorForPlatform(const Color32& col) { return col.UnswizzleARGB(); }
#else
	inline Color32 SwizzleColorForPlatform(const Color32& col) { return col.SwizzleToBGRA(); }
	inline Color32 UnswizzleColorForPlatform(const Color32& col) { return col.UnswizzleBGRA(); }
#endif

inline Color32 operator + (const Color32& inC0, const Color32& inC1)
{
	return Color32 (std::min<int> (inC0.r + inC1.r, 255), 
		std::min<int> (inC0.g + inC1.g, 255),
		std::min<int> (inC0.b + inC1.b, 255),
		std::min<int> (inC0.a + inC1.a, 255));
}

inline Color32 operator * (const Color32& inC0, const Color32& inC1)
{
#if 0
	return ColorRGBA32 ((inC0.r * inC1.r) / 255, 
		(inC0.g * inC1.g) / 255,
		(inC0.b * inC1.b) / 255,
		(inC0.a * inC1.a) / 255);
#else // This is much faster, but doesn't guarantee 100% matching result (basically color values van vary 1/255 but not at ends, check out unit test in cpp file).
	const uint32& u = reinterpret_cast<const uint32&> (inC0);
	const uint32& v = reinterpret_cast<const uint32&> (inC1);
	uint32 result = (((u & 0x000000ff) * ((v & 0x000000ff) + 1)) >> 8) & 0x000000ff;
	result |= (((u & 0x0000ff00) >> 8) * (((v & 0x0000ff00) >> 8) + 1)) & 0x0000ff00;
	result |= (((u & 0x00ff0000) * (((v & 0x00ff0000) >> 16) + 1)) >> 8) & 0x00ff0000;
	result |= (((u & 0xff000000) >> 8) * (((v & 0xff000000) >> 24) + 1)) & 0xff000000;
	return Color32 (result);
#endif
}

inline Color32 Lerp(const Color32& c0, const Color32& c1, int scale)
{
	//Assert (scale < 0 || scale > 255);
	const uint32& u0 = reinterpret_cast<const uint32&> (c0);
	const uint32& u1 = reinterpret_cast<const uint32&> (c1);
	uint32 vx = u0 & 0x00ff00ff;
	uint32 rb = vx + ((((u1 & 0x00ff00ff) - vx) * scale) >> 8) & 0x00ff00ff;
	vx = u0 & 0xff00ff00;
	return Color32( rb | (vx + ((((u1 >> 8) & 0x00ff00ff) - (vx >> 8)) * scale) & 0xff00ff00) );
}

QIMP_END_NAMESPACE

#endif //__MATH_COLOR_H__