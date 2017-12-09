#ifndef __MATH_SPHERICAL_HARMONICS_H__
#define __MATH_SPHERICAL_HARMONICS_H__

#pragma once

QIMP_BEGIN_NAMESPACE

static inline void SHEvalDirection9 (float x, float y, float z, float outsh[9])
{
	// Core i7 920, VS2008 Release, FPU code:

	// 114 clocks
	//D3DXSHEvalDirection (outsh, 3, &D3DXVECTOR3(x,y,z));

	/*
	// 314 clocks
	// Reference implementation from Stupid Spherical Harmonics Tricks
	// http://www.ppsloan.org/publications/StupidSH36.pdf
	const float kSqrtPI = sqrtf(kPI);
	const float kSqrt3 = sqrtf(3.0f);
	const float kSqrt15 = sqrtf(15.0f);
	outsh[0] = 1.0f / (2.0f * kSqrtPI);
	outsh[1] = -(kSqrt3 * y) / (2.0f * kSqrtPI);
	outsh[2] = (kSqrt3 * z) / (2.0f * kSqrtPI);
	outsh[3] = -(kSqrt3 * x) / (2.0f * kSqrtPI);
	outsh[4] = (kSqrt15 * x * y) / (2.0f * kSqrtPI);
	outsh[5] = -(kSqrt15 * y * z) / (2.0f * kSqrtPI);
	outsh[6] = (sqrtf(5.0f) * (3.0f*z*z-1.0f)) / (4.0f * kSqrtPI);
	outsh[7] = -(kSqrt15 * x * z) / (2.0f * kSqrtPI);
	outsh[8] = (kSqrt15 * (x*x - y*y)) / (4.0f * kSqrtPI);
	*/

	// 86 clocks
	// Make sure all constants are never computed at runtime
	const float kInv2SqrtPI = 0.28209479177387814347403972578039f; // 1 / (2*sqrt(kPI))
	const float kSqrt3Div2SqrtPI = 0.48860251190291992158638462283835f; // sqrt(3) / (2*sqrt(kPI))
	const float kSqrt15Div2SqrtPI = 1.0925484305920790705433857058027f; // sqrt(15) / (2*sqrt(kPI))
	const float k3Sqrt5Div4SqrtPI = 0.94617469575756001809268107088713f; // 3 * sqrtf(5) / (4*sqrt(kPI))
	const float kSqrt15Div4SqrtPI = 0.54627421529603953527169285290135f; // sqrt(15) / (4*sqrt(kPI))
	const float kOneThird = 0.3333333333333333333333f; // 1.0/3.0
	outsh[0] = kInv2SqrtPI;
	outsh[1] = - y * kSqrt3Div2SqrtPI;
	outsh[2] =   z * kSqrt3Div2SqrtPI;
	outsh[3] = - x * kSqrt3Div2SqrtPI;
	outsh[4] =   x * y * kSqrt15Div2SqrtPI;
	outsh[5] = - y * z * kSqrt15Div2SqrtPI;
	outsh[6] =  (z*z-kOneThird) * k3Sqrt5Div4SqrtPI;
	outsh[7] = - x * z * kSqrt15Div2SqrtPI;
	outsh[8] =  (x*x-y*y) * kSqrt15Div4SqrtPI;
}


static inline void SHEvalDirectionalLight9 (
	float x, float y, float z,
	float colorR, float colorG, float colorB,
	float outR[9], float outG[9], float outB[9])
{
	// Core i7 920, VS2008 Release, FPU code:

	// 397 clocks
	//D3DXSHEvalDirectionalLight (3, &D3DXVECTOR3(x,y,z), colorR, colorG, colorB, outR, outG, outB);

	// 300 clocks
	float sh[9];
	SHEvalDirection9 (x, y, z, sh);
	// Normalization factor from http://www.ppsloan.org/publications/StupidSH36.pdf
	const float kNormalization = 2.9567930857315701067858823529412f; // 16*kPI/17
	float rscale = colorR * kNormalization;
	float gscale = colorG * kNormalization;
	float bscale = colorB * kNormalization;
	for (int i = 0; i < 9; ++i)
	{
		float c = sh[i];
		outR[i] = c * rscale;
		outG[i] = c * gscale;
		outB[i] = c * bscale;
	}
}

// Add 'out' to the 0th SH coefficients
static inline void SHEvalAmbientLight(const Colorf& ambient, float out[3])
{
	const float k2SqrtPI = 3.54490770181103205459633496668229f; // 2*sqrt(kPI)
	out[0] = ambient.r * k2SqrtPI;
	out[1] = ambient.g * k2SqrtPI;
	out[2] = ambient.b * k2SqrtPI;
}

QIMP_END_NAMESPACE

#endif// __MATH_SPHERICAL_HARMONICS_H__