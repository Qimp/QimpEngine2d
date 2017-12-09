#include "Private.h"

QIMP_BEGIN_NAMESPACE

GradientNEW::GradientNEW()
	:	m_NumColorKeys(2)
	,	m_NumAlphaKeys(2)
{
	m_Keys[0] = m_Keys[1] = Color32(0xffffffff);
	m_ColorTime[0] = m_AlphaTime[0] = NormalizedToWord(0.0f);
	m_ColorTime[1] = m_AlphaTime[1] = NormalizedToWord(1.0f);

	for(uint32 i = 2; i < kGradientMaxNumKeys; i++)
	{
		m_Keys[i] = Color32(0);
		m_ColorTime[i] = NormalizedToWord(0.0f);
		m_AlphaTime[i] = NormalizedToWord(0.0f);
	}
}

GradientNEW::~GradientNEW()
{
}

void GradientNEW::SetKeys (ColorKey* colorKeys, unsigned numColorKeys, AlphaKey* alphaKeys, unsigned numAlphaKeys)
{
	SetColorKeys (colorKeys, numColorKeys);
	SetAlphaKeys (alphaKeys, numAlphaKeys);
}

void GradientNEW::SwapColorKeys(int i, int j)
{
	Color32 tmpCol = m_Keys[i];
	uint16 tmpTime = m_ColorTime[i];
	m_Keys[i].r = m_Keys[j].r;
	m_Keys[i].g = m_Keys[j].g;
	m_Keys[i].b = m_Keys[j].b;
	m_ColorTime[i] = m_ColorTime[j];
	m_Keys[j].r = tmpCol.r;
	m_Keys[j].g = tmpCol.g;
	m_Keys[j].b = tmpCol.b;
	m_ColorTime[j] = tmpTime;
}

void GradientNEW::SwapAlphaKeys(int i, int j)
{
	Color32 tmpCol = m_Keys[i];
	uint16 tmpTime = m_AlphaTime[i];
	m_Keys[i].a = m_Keys[j].a;
	m_AlphaTime[i] = m_AlphaTime[j];
	m_Keys[j].a = tmpCol.a;
	m_AlphaTime[j] = tmpTime;
}

void GradientNEW::SetColorKeys (ColorKey* colorKeys, unsigned int numKeys)
{
	Assert (numKeys <= kGradientMaxNumKeys);
	if (numKeys > kGradientMaxNumKeys)
		numKeys = kGradientMaxNumKeys;

	for (size_t i=0; i<numKeys; ++i)
	{
		const Colorf& color = colorKeys[i].m_Color;
		m_Keys[i].r = NormalizedToByte(color.r);
		m_Keys[i].g = NormalizedToByte(color.g);
		m_Keys[i].b = NormalizedToByte(color.b);
		m_ColorTime[i] = NormalizedToWord(colorKeys[i].m_Time);
	}
	m_NumColorKeys = numKeys;

	// Ensure sorted!
	int i = 0;
	const int keyCount = m_NumColorKeys;
	while ((i + 1) < keyCount)
	{
		if (m_ColorTime[i] > m_ColorTime[i+1])
		{
			SwapColorKeys(i, i + 1);
			if (i > 0)
				i -= 2;
		}
		i++;
	}

	ValidateColorKeys();
}

void GradientNEW::SetAlphaKeys (AlphaKey* alphaKeys, unsigned int numKeys)
{
	Assert (numKeys <= kGradientMaxNumKeys);
	if (numKeys > kGradientMaxNumKeys)
		numKeys = kGradientMaxNumKeys;

	for (size_t i=0; i<numKeys; ++i)
	{
		float alpha = alphaKeys[i].m_Alpha;
		m_Keys[i].a = NormalizedToByte(alpha);
		m_AlphaTime[i] = NormalizedToWord(alphaKeys[i].m_Time);
	}
	m_NumAlphaKeys = numKeys;

	// Ensure sorted!
	int i = 0;
	const int keyCount = m_NumAlphaKeys;
	while ((i + 1) < keyCount)
	{
		if (m_AlphaTime[i] > m_AlphaTime[i+1])
		{
			SwapAlphaKeys(i, i + 1);
			if (i > 0)
				i -= 2;
		}
		i++;
	}
	
	ValidateAlphaKeys();
}

Color32 GradientNEW::GetConstantColor () const
{
	return m_Keys[0];
}

void GradientNEW::SetConstantColor (Color32 color)
{
	m_Keys[0] = color;
	m_NumAlphaKeys = 1;
	m_NumColorKeys = 1;
}

void GradientNEW::ValidateColorKeys()
{
	// Make sure there is a minimum of 2 keys
	if(m_NumColorKeys < 2)
	{
		m_NumColorKeys = 2;
		m_Keys[1] = m_Keys[0];
		m_ColorTime[0] = NormalizedToWord(0.0f);
		m_ColorTime[1] = NormalizedToWord(1.0f);
	}
}

void GradientNEW::ValidateAlphaKeys()
{
	// Make sure there is a minimum of 2 keys
	if(m_NumAlphaKeys < 2)
	{
		m_NumAlphaKeys = 2;
		m_Keys[1].a = m_Keys[0].a;
		m_AlphaTime[0] = NormalizedToWord(0.0f);
		m_AlphaTime[1] = NormalizedToWord(1.0f);
	}
}

void GradientNEW::InitializeOptimized(OptimizedGradient& gradient)
{
	// Copy all time values
	for(int i = 0; i < m_NumColorKeys; ++i)
		gradient.times[i] = m_ColorTime[i];

	for(int i = 0, i2 = m_NumColorKeys; i < m_NumAlphaKeys; ++i, ++i2)
		gradient.times[i2] = m_AlphaTime[i];

	// Remove duplicates
	int keyCount = m_NumColorKeys + m_NumAlphaKeys;
	for(int i = 0; i < keyCount-1; ++i)
	{
		for(int j = i+1; j < keyCount; )
		{
			if(gradient.times[i] == gradient.times[j])
			{
				std::swap(gradient.times[j], gradient.times[keyCount-1]);
				keyCount--;
				continue;	
			}
			++j;
		}
	}

	// Sort
	int i = 0;
	while ((i + 1) < keyCount)
	{
		if (gradient.times[i] > gradient.times[i+1])
		{
			std::swap(gradient.times[i], gradient.times[i+1]);
			if (i > 0)
				i -= 2;
		}
		i++;
	}

	for(int i = 0; i < keyCount; ++i)
		gradient.colors[i] = Evaluate(WordToNormalized(gradient.times[i]));
	gradient.keyCount = keyCount;

	for(int i = 1; i < keyCount; ++i)
		gradient.rcp[i] = ((((1<<24)) / std::max<uint32>(gradient.times[i] - gradient.times[i-1], 1)))+1;
}

//template<class TransferFunction>
//void GradientNEW::Transfer (TransferFunction& transfer)
//{
//	Assert (kGradientMaxNumKeys > 9);
//	
//	const char* kKeyNames [kGradientMaxNumKeys] = { "key0", "key1", "key2", "key3", "key4", "key5", "key6", "key7", }; 
//	for(uint32 i = 0; i < kGradientMaxNumKeys; i++)
//		transfer.Transfer(m_Keys[i], kKeyNames[i], kHideInEditorMask);
//	
//	const char* kColorTimeNames [kGradientMaxNumKeys] = { "ctime0", "ctime1", "ctime2", "ctime3", "ctime4", "ctime5", "ctime6", "ctime7", }; 
//	for(uint32 i = 0; i < kGradientMaxNumKeys; i++)
//		transfer.Transfer(m_ColorTime[i], kColorTimeNames[i], kHideInEditorMask);
//	
//	const char* kAlphaTimeNames [kGradientMaxNumKeys] = { "atime0", "atime1", "atime2", "atime3", "atime4", "atime5", "atime6", "atime7", }; 
//	for(uint32 i = 0; i < kGradientMaxNumKeys; i++)
//		transfer.Transfer(m_AlphaTime[i], kAlphaTimeNames[i], kHideInEditorMask);
//
//	transfer.Transfer (m_NumColorKeys, "m_NumColorKeys", kHideInEditorMask);
//	transfer.Transfer (m_NumAlphaKeys, "m_NumAlphaKeys", kHideInEditorMask);
//	transfer.Align();
//	
//	if(transfer.IsReading())
//	{
//		ValidateColorKeys();
//		ValidateAlphaKeys();
//	}
//}
//INSTANTIATE_TEMPLATE_TRANSFER(GradientNEW)


#if ENABLE_UNIT_TESTS

#include "External/UnitTest++/src/UnitTest++.h"

bool CompareColors(Color32 c0, Color32 c1, int tolerance)
{
	if(Math::Abs(c0.r-c1.r) > tolerance)
		return false;
	if(Math::Abs(c0.g-c1.g) > tolerance)
		return false;
	if(Math::Abs(c0.b-c1.b) > tolerance)
		return false;
	if(Math::Abs(c0.a-c1.a) > tolerance)
		return false;
	return true;
}

TEST (GradientEvaluateTest)
{
	// Set up rainbow gradient
	GradientNEW gradient;
	gradient.SetNumColorKeys(5);
	gradient.SetNumAlphaKeys(3);
	gradient.GetKey(0) = Color32(0xff, 0x00, 0x00, 0xff);
	gradient.GetKey(1) = Color32(0xf8, 0xff, 0x00, 0x00);
	gradient.GetKey(2) = Color32(0x00, 0xff, 0x49, 0xff);
	gradient.GetKey(3) = Color32(0x22, 0x00, 0xff, 0x00);
	gradient.GetKey(4) = Color32(0xff, 0x00, 0xe6, 0x00);
	gradient.GetColorTime(0) = 0x0000;
	gradient.GetColorTime(1) = 0x40c1;
	gradient.GetColorTime(2) = 0x9212;
	gradient.GetColorTime(3) = 0xce4e;
	gradient.GetColorTime(4) = 0xffff;
	gradient.GetAlphaTime(0) = 0x0000;
	gradient.GetAlphaTime(1) = 0x8000;
	gradient.GetAlphaTime(2) = 0xffff;

	CHECK_EQUAL(Color32(0xff, 0x00, 0x00, 0xff) == gradient.Evaluate(0.0f), true);
	CHECK_EQUAL(Color32(0xfd, 0x31, 0x00, 0xe6) == gradient.Evaluate(0.05f), true);
	CHECK_EQUAL(Color32(0xfa, 0x96, 0x00, 0xb3) == gradient.Evaluate(0.15f), true);
	CHECK_EQUAL(Color32(0xf8, 0xfc, 0x00, 0x7f) == gradient.Evaluate(0.25f), true);
	CHECK_EQUAL(Color32(0xac, 0xff, 0x16, 0x4c) == gradient.Evaluate(0.35f), true);
	CHECK_EQUAL(Color32(0x5e, 0xff, 0x2d, 0x19) == gradient.Evaluate(0.45f), true);
	CHECK_EQUAL(Color32(0x10, 0xff, 0x44, 0x18) == gradient.Evaluate(0.55f), true);
	CHECK_EQUAL(Color32(0x0b, 0xa9, 0x86, 0x4b) == gradient.Evaluate(0.65f), true);
	CHECK_EQUAL(Color32(0x19, 0x3c, 0xd3, 0x7e) == gradient.Evaluate(0.75f), true);
	CHECK_EQUAL(Color32(0x54, 0x00, 0xf9, 0xb2) == gradient.Evaluate(0.85f), true);
	CHECK_EQUAL(Color32(0xc6, 0x00, 0xec, 0xe5) == gradient.Evaluate(0.95f), true);
	CHECK_EQUAL(Color32(0xff, 0x00, 0xe6, 0xff) == gradient.Evaluate(1.0f), true);

	OptimizedGradient optGradient;
	gradient.InitializeOptimized(optGradient);

	// Being off by 1LSB is okay... (due to rounding)
	for(float time = 0.0f; time <= 1.0f; time += 0.02f)
		CHECK_EQUAL(CompareColors(optGradient.Evaluate(time), gradient.Evaluate(time), 1), true);

	// ... but require exactness precicely at key times
	for(int i = 0; i < 5; i++)
	{
		float time = WordToNormalized(gradient.GetColorTime(i));
		CHECK_EQUAL(CompareColors(optGradient.Evaluate(time), gradient.Evaluate(time), 0), true); 
	}
	for(int i = 0; i < 3; i++)
	{
		float time = WordToNormalized(gradient.GetAlphaTime(i));
		CHECK_EQUAL(CompareColors(optGradient.Evaluate(time), gradient.Evaluate(time), 0), true);
	}
}

#endif

QIMP_END_NAMESPACE