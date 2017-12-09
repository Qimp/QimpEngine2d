#ifndef __RENDER_TEX_FORMAT_H__
#define __RENDER_TEX_FORMAT_H__


QIMP_BEGIN_NAMESPACE

struct TexFormat
{
	enum Type {
		NULLTARGET, R5G6B5,	// for directx shadow map null color target

		RGB10A2, // just for render target

		RG16, // for indirect texture

		// compressed image
		ETC1, ETC2, ETC2A, ETC4, PVRTC2, PVRTC2A, PVRTC4, PVRTC4A, PVR,

		// 8 bits image
		L8, LA8, A8, RGB8, RGBA8, RGBX8,
		RGBA4,

		// 16 bits fixed image
		L16, // 16 bits int texture, terrain heightmap use this

		// 16 bits float image
		R16F, RG16F, RGB16F, RGBA16F,

		// 32 bits float image
		R32F, RG32F, RGB32F, RGBA32F,

		// depth texture
		D16, D24, D32F,

		// depth-stencil
		D24S8,

		// vendor specified
		INTZ,

		MAX_NUMBER,
		NONE = MAX_NUMBER
	};

	DECLARE_ENUM(TexFormat);

	bool IsByte() const;
	bool IsUShort() const;
	//bool IsDXTC() const;
	bool IsFloat() const;
	bool IsHalf() const;
	bool IsColor() const { return !IsDepth(); }
	bool IsDepth() const { return m_Type >= D16 && m_Type <= INTZ; }
	bool IsStencil() const { return m_Type == D24S8; }
	bool IsCompressed() const;
	int GetNumComponents() const;
	int GetBitsPerPixel() const;
	int CalculateDataSize(int width, int height) const;
	int GetBlockSize() const;
	int GetBlockDataSize() const;
	int GetDepthBits() const;
	const char *ToString() const;
};

inline bool TexFormat::IsByte() const
{
	return m_Type >= L8 && m_Type <= RGBX8;
}

inline bool TexFormat::IsUShort() const
{
	return m_Type == L16 || m_Type == RG16;
}

inline bool TexFormat::IsFloat() const
{
	return m_Type >= R32F && m_Type <= RGBA32F;
}

inline bool TexFormat::IsHalf() const
{
	return m_Type >= R16F && m_Type <= RGBA16F;
}

inline int TexFormat::GetNumComponents() const
{
	switch (m_Type) {
	case RG16:
		return 2;
	case L8:				// 8 bits luminance: alpha is always 1.0, Uint8
		return 1;
	case LA8:				// 8 bits luminance & alpha
		return 2;
	case A8:				// 8 bits alpha: font texture use this
		return 1;
	case RGB8:
		return 3;
	case RGBA8:
		return 4;
	case RGBX8:
		return 4;
	case R5G6B5:
		return 3;

	case L16:
		return 1;					// 16 bits short int, for terrain or other use, UShort
	case PVRTC2:
		return 3;
	case PVRTC2A:
		return 4;
	case PVRTC4:
		return 3;
	case PVRTC4A:
		return 4;

	// 16 bits float image
	case R16F:			// 16 bits float luminance: alpha is always 1.0
		return 1;
	case RG16F:
		return 2;
	case RGB16F:
		return 3;
	case RGBA16F:
		return 4;
	case RGBA4:
		return 4;

	// 32 bits float image
	case R32F:			// 32 bits float luminance: alpha is always 1.0
		return 1;
	case RG32F:
		return 2;
	case RGB32F:
		return 3;
	case RGBA32F:
		return 4;

	case D16:
	case D24:
	case D32F:
	case D24S8:
	case INTZ:
		return 1;

	default: Assert(0); return 0;
	};
}

// get image's bits per pixel
inline int TexFormat::GetBitsPerPixel() const
{
	if (IsCompressed())
	{
		if (m_Type == ETC1) return 3;
		if (m_Type == ETC2) return 3;
		if (m_Type == ETC2A) return 4;
		if (m_Type == PVRTC2) return 2;
		if (m_Type == PVRTC2A) return 2;
		if (m_Type == PVRTC4) return 4;
		if (m_Type == PVRTC4A) return 4;
	}
	if (IsByte())
		return GetNumComponents() * 8;

	if (IsUShort() || IsHalf())
		return GetNumComponents() * 16;

	if (IsFloat())
		return GetNumComponents() * 32;

	if (m_Type == NULLTARGET) return 0;
	if (m_Type == R5G6B5 || m_Type == RGBA4) return 16;
	if (m_Type == RGB10A2) return 32;
	if (m_Type == D16) return 16;
	if (m_Type == D24) return 24;
	if (m_Type == D32F) return 32;
	if (m_Type == D24S8 || m_Type == INTZ) return 32;

	return 0;
}

inline bool TexFormat::IsCompressed() const
{
	return m_Type >= ETC1 && m_Type <= PVR;
}

// return number bytes
inline int TexFormat::CalculateDataSize(int width, int height) const
{
	if (IsCompressed() && m_Type >= ETC1 && m_Type <= ETC4)
		return ((width >> 2) * (height >> 2)) * GetBlockDataSize();
	int numPixels = width * height;
	return numPixels * GetBitsPerPixel() >> 3;
}

inline int TexFormat::GetBlockSize() const
{
	if (IsCompressed()) {
		return 4;
	} else {
		return 1;
	}
}

inline int TexFormat::GetBlockDataSize() const
{
	if (IsCompressed())
		return 1 << GetBitsPerPixel();

	return GetBitsPerPixel() >> 3;
}

inline int TexFormat::GetDepthBits() const
{
	if (!IsDepth()) {
		return 0;
	}

	switch (m_Type) {
	case D16:
		return 16;
	case D24:
	case D24S8:
	case INTZ:
		return 24;
	case D32F:
		return 32;
	default:
		return 0;
	}
}

inline const char *TexFormat::ToString() const
{
	switch (m_Type) {
	case NULLTARGET: return "NULL";
	case R5G6B5: return "R5G6B5";
	case RGB10A2: return "RGB10A2";
	case RG16: return "RG16";
	case L8: return "L8"; // 8 bits luminance: alpha is always 1.0, Uint8
	case LA8: return "LA8"; // 8 bits luminance & alpha
	case A8: return "A8"; // 8 bits alpha: font texture use this
	case RGB8: return "BGR8";
	case RGBA8: return "BGRA8";
	case RGBX8: return "BGRX8";

	case L16: return "L16";					// 16 bits short int, for terrain or other use, UShort

	// 16 bits float image
	case R16F: return "R16F"; // 16 bits float luminance: alpha is always 1.0
	case RG16F: return "RG16F";
	case RGB16F: return "RGB16F";
	case RGBA16F: return "RGBA16F";
	case RGBA4: return "RGBA4";

	// 32 bits float image
	case R32F: return "R32F"; // 32 bits float luminance: alpha is always 1.0
	case RG32F: return "RG32F";
	case RGB32F: return "RGB32F";
	case RGBA32F: return "RGBA32F";

	case D16: return "D16";
	case D24: return "D24";
	case D32F: return "D32F";
	case D24S8: return "D24S8";
	case PVR: return "PVR";
	case PVRTC2: return "PVRTC2";
	case PVRTC2A: return "PVRTC2A";
	case PVRTC4: return "PVRTC4";
	case PVRTC4A: return "PVRTC4A";

	case INTZ: return "INTZ";

	default: return "NONAME";
	};
}



QIMP_END_NAMESPACE

#endif//__RENDER_TEX_FORMAT_H__
