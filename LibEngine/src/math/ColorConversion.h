#ifndef __COLOR_CONVERSION_H__
#define __COLOR_CONVERSION_H__

#pragma once

#include "Color.h"

QIMP_BEGIN_NAMESPACE

enum ColorSpace { kUninitializedColorSpace = -1, kGammaColorSpace = 0, kLinearColorSpace, kMaxColorSpace };
extern ColorSpace gActiveColorSpace;


// http://www.opengl.org/registry/specs/EXT/framebuffer_sRGB.txt
// http://www.opengl.org/registry/specs/EXT/texture_sRGB_decode.txt
// {  cs / 12.92,                 cs <= 0.04045 }
// {  ((cs + 0.055)/1.055)^2.4,   cs >  0.04045 }

inline float GammaToLinearSpace (float value)
{
	if (value <= 0.04045F)
		return value / 12.92F;
	else if (value < 1.0F)
		return pow((value + 0.055F)/1.055F, 2.4F);
	else
		return pow(value, 2.4F);
}

// http://www.opengl.org/registry/specs/EXT/framebuffer_sRGB.txt
// http://www.opengl.org/registry/specs/EXT/texture_sRGB_decode.txt
// {  0.0,                          0         <= cl
// {  12.92 * c,                    0         <  cl < 0.0031308
// {  1.055 * cl^0.41666 - 0.055,   0.0031308 <= cl < 1
// {  1.0,                                       cl >= 1  <- This has been adjusted since we want to maintain HDR colors

inline float LinearToGammaSpace (float value)
{
	if (value <= 0.0F)
		return 0.0F;
	else if (value <= 0.0031308F)
		return 12.92F * value;
	else if (value <= 1.0F)
		return 1.055F * powf(value, 0.41666F) - 0.055F;
	else
		return powf(value, 0.41666F);
}

inline float GammaToLinearSpaceXenon(float val)
{
	float ret;
	if (val < 0)
		ret = 0;
	else if (val < 0.25f)
		ret = 0.25f * val;
	else if (val < 0.375f)
		ret = (1.0f/16.0f) + 0.5f*(val-0.25f);
	else if (val < 0.75f)
		ret = 0.125f + 1.0f*(val-0.375f);
	else if (val < 1.0f)
		ret = 0.5f + 2.0f*(val-0.75f);
	else
		ret = 1.0f;
	return ret;
}

inline float LinearToGammaSpaceXenon(float val)
{
	float ret;
	if (val < 0)
		ret = 0;
	else if (val < (1.0f/16.0f))
		ret = 4.0f * val;
	else if (val < (1.0f/8.0f))
		ret = (1.0f/4.0f) + 2.0f*(val-(1.0f/16.0f));
	else if (val < 0.5f)
		ret = 0.375f + 1.0f*(val-0.125f);
	else if (val < 1.0f)
		ret = 0.75f + 0.5f*(val-0.50f);
	else
		ret = 1.0f;
	
	return ret;
}

inline Colorf GammaToLinearSpace (const Colorf& value)
{
	return Colorf(GammaToLinearSpace(value.r), GammaToLinearSpace(value.g), GammaToLinearSpace(value.b), value.a);
}

inline Colorf LinearToGammaSpace (const Colorf& value)
{
	return Colorf(LinearToGammaSpace(value.r), LinearToGammaSpace(value.g), LinearToGammaSpace(value.b), value.a);
}

inline Colorf GammaToLinearSpaceXenon (const Colorf& value)
{
	return Colorf(GammaToLinearSpaceXenon(value.r), GammaToLinearSpaceXenon(value.g), GammaToLinearSpaceXenon(value.b), value.a);
}

inline Colorf LinearToGammaSpaceXenon (const Colorf& value)
{
	return Colorf(LinearToGammaSpaceXenon(value.r), LinearToGammaSpaceXenon(value.g), LinearToGammaSpaceXenon(value.b), value.a);
}

inline float GammaToActiveColorSpace (float value)
{	
	if (gActiveColorSpace == kLinearColorSpace)
		return GammaToLinearSpace(value); 
	else
		return value; 
}

inline Colorf GammaToActiveColorSpace (const Colorf& value)
{	
	if (gActiveColorSpace == kLinearColorSpace)
		return GammaToLinearSpace(value); 
	else
		return value; 
}

inline Colorf ActiveToGammaColorSpace (const Colorf& value)
{
	if (gActiveColorSpace == kLinearColorSpace)
		return LinearToGammaSpace(value); 
	else
		return value;
}

void		SetActiveColorSpace (ColorSpace space);
ColorSpace  GetActiveColorSpace ();

QIMP_END_NAMESPACE

#endif//__COLOR_CONVERSION_H__