#ifndef __MATH_COLOR_MATRIX_H__
#define __MATH_COLOR_MATRIX_H__

QIMP_BEGIN_NAMESPACE

class ColorMatrix {

private:
	float m_Data[20];
	float m_Matrix[16]; // mat4
	float m_Offset[4]; // mat1

	#define toFloat 1.f / 255.f;

	void Concat(float *m) {
		float tmp[20];

		for (int i = 0; i<4; i++) {

			tmp[i * 5 + 0] = m_Data[i * 5] * m[0] + m_Data[i * 5 + 1] * m[5 + 0] + m_Data[i * 5 + 2] * m[10 + 0] + m_Data[i * 5 + 3] * m[15 + 0];
			tmp[i * 5 + 1] = m_Data[i * 5] * m[1] + m_Data[i * 5 + 1] * m[5 + 1] + m_Data[i * 5 + 2] * m[10 + 1] + m_Data[i * 5 + 3] * m[15 + 1];
			tmp[i * 5 + 2] = m_Data[i * 5] * m[2] + m_Data[i * 5 + 1] * m[5 + 2] + m_Data[i * 5 + 2] * m[10 + 2] + m_Data[i * 5 + 3] * m[15 + 2];
			tmp[i * 5 + 3] = m_Data[i * 5] * m[3] + m_Data[i * 5 + 1] * m[5 + 3] + m_Data[i * 5 + 2] * m[10 + 3] + m_Data[i * 5 + 3] * m[15 + 3];
			tmp[i * 5 + 4] = m_Data[i * 5] * m[4] + m_Data[i * 5 + 1] * m[5 + 4] + m_Data[i * 5 + 2] * m[10 + 4] + m_Data[i * 5 + 3] * m[15 + 4] + m_Data[i * 5 + 4];
		}

		for (int i = 0; i<20; i++) m_Data[i] = tmp[i];
	}

	float *Identity(float *mat) {
		mat[0] = 1; mat[1] = 0; mat[2] = 0; mat[3] = 0; mat[4] = 0;
		mat[5] = 0; mat[6] = 1; mat[7] = 0; mat[8] = 0; mat[9] = 0;
		mat[10] = 0; mat[11] = 0; mat[12] = 1; mat[13] = 0; mat[14] = 0;
		mat[15] = 0; mat[16] = 0; mat[17] = 0; mat[18] = 1; mat[19] = 0;
		return mat;
	}

public:
	ColorMatrix() {

		//m_Data = (float *)malloc(sizeof(float) * 20);
		//m_Matrix = (float *)malloc(sizeof(float) * 16);
		//m_Offset = (float *)malloc(sizeof(float) * 4);

		Identity();
	}

	~ColorMatrix() {
		//free(m_Data);
		//free(m_Matrix);
		//free(m_Offset);
	}

	ColorMatrix *Identity() {
		m_Data[0] = 1; m_Data[1] = 0; m_Data[2] = 0; m_Data[3] = 0; m_Data[4] = 0;
		m_Data[5] = 0; m_Data[6] = 1; m_Data[7] = 0; m_Data[8] = 0; m_Data[9] = 0;
		m_Data[10] = 0; m_Data[11] = 0; m_Data[12] = 1; m_Data[13] = 0; m_Data[14] = 0;
		m_Data[15] = 0; m_Data[16] = 0; m_Data[17] = 0; m_Data[18] = 1; m_Data[19] = 0;
		return this;
	}

	ColorMatrix *Reset() { return Identity(); }


	ColorMatrix *Invert() {
		float tmp[20] = {
			-1., 0., 0., 0., 255.,
			0., -1., 0., 0., 255.,
			0., 0., -1., 0., 255.,
			0., 0., 0., 1., 0.
		};
		Concat(tmp);
		return this;
	}

	void GetAdjustParameters(Vector4f& param1, Vector4f& param2, Vector4f& param3) {
		//param1 = CVector4f(m_Data[0], m_Data[5], m_Data[10], m_Data[15]);
		//param2 = CVector4f(m_Data[1], m_Data[6], m_Data[11], m_Data[16]);
		//param3 = CVector4f(m_Data[2], m_Data[7], m_Data[12], m_Data[17]);
		param1 = Vector4f(m_Data[0], m_Data[1], m_Data[2], m_Data[4]);
		param2 = Vector4f(m_Data[5], m_Data[6], m_Data[7], m_Data[9]);
		param3 = Vector4f(m_Data[10], m_Data[11], m_Data[12], m_Data[14]);
	}

	void Adjust(int32 brightness, int32 contrast, int32 saturation, int32 hue) {
		AdjustHue(hue);
		AdjustContrast(contrast);
		AdjustBrightness(brightness);
		AdjustSaturation(saturation);
	}

	// -100.0 to 100.0 (default = 0.0)
	void AdjustSaturation(int32 sat){
		sat = Clamp(sat, -100, 100);
		if (sat == 0)
			return;

		float x = 1.0f + ((sat > 0) ? 0.03f * sat : sat * 0.01f);
		float lumR = 0.3086f;
		float lumG = 0.6094f;
		float lumB = 0.0820f;
		float is = 1.0f - x;
		float ir = is*lumR;
		float ig = is*lumG;
		float ib = is*lumB;

		float tmp[20] = {
			(ir + x), ig, ib, 0, 0,
			ir, (ig + x), ib, 0, 0,
			ir, ig, (ib + x), 0, 0,
			0, 0, 0, 1, 0
		};
		Concat(tmp);
	}

	// -100 (=grey) to 100 (default = 0)
	void AdjustContrast(int32 con) {

		static float DELTA_INDEX[] = {
			0.0f, 0.01f, 0.02f, 0.04f, 0.05f, 0.06f, 0.07f, 0.08f, 0.1f, 0.11f,
			0.12f, 0.14f, 0.15f, 0.16f, 0.17f, 0.18f, 0.20f, 0.21f, 0.22f, 0.24f,
			0.25f, 0.27f, 0.28f, 0.30f, 0.32f, 0.34f, 0.36f, 0.38f, 0.40f, 0.42f,
			0.44f, 0.46f, 0.48f, 0.50f, 0.53f, 0.56f, 0.59f, 0.62f, 0.65f, 0.68f,
			0.71f, 0.74f, 0.77f, 0.80f, 0.83f, 0.86f, 0.89f, 0.92f, 0.95f, 0.98f,
			1.00f, 1.06f, 1.12f, 1.18f, 1.24f, 1.30f, 1.36f, 1.42f, 1.48f, 1.54f,
			1.60f, 1.66f, 1.72f, 1.78f, 1.84f, 1.90f, 1.96f, 2.00f, 2.12f, 2.25f,
			2.37f, 2.50f, 2.62f, 2.75f, 2.87f, 3.0f, 3.2f, 3.40f, 3.60f, 3.80f,
			4.00f, 4.30f, 4.70f, 4.90f, 5.00f, 5.50f, 6.00f, 6.50f, 6.80f, 7.00f,
			7.30f, 7.50f, 7.80f, 8.00f, 8.40f, 8.70f, 9.00f, 9.40f, 9.60f, 9.80f,
			10.0f
		};

		con = Clamp(con, -100, 100);
		if (con == 0)
			return;

		float x;
		if (con < 0) {
			x = 127.0f + con * 1.27f;
		}
		else {
			x = DELTA_INDEX[con];
			x = x * 127.0f + 127.0f;
		}

		float scale = 1.0f/ (2.0f * 255.0f);
		float tmp[20] = {
			x / 127.0f, 0, 0, 0, scale * (127.0f - x),
			0, x / 127.0f, 0, 0, scale * (127.0f - x),
			0, 0, x / 127.0f, 0, scale * (127.0f - x),
			0, 0, 0, 1.0f, 0,
		};
		Concat(tmp);
	}

	// (default = 0.)
	void AdjustBrightness(int32 value) {
		value = Clamp(value, -100, 100);
		if (value == 0)
			return;
		float scale = 1.0f / 255.0f;
		float tmp[20] = {
			1.0f, 0.0f, 0.0f, 0.0f, (float)value * scale,
			0.0f, 1.0f, 0.0f, 0.0f, (float)value * scale,
			0.0f, 0.0f, 1.0f, 0.0f, (float)value * scale,
			0.0f, 0.0f, 0.0f, 1.0f, 0.0f
		};
		Concat(tmp);
	}

	void AdjustHue(int32 value) {
		value = Clamp(value, -180, 180);
		if (value == 0)
			return;

		static float GrisR = 0.213f;
		static float GrisG = 0.715f;
		static float GrisB = 0.072f;

		float angle = (float)value * (float)kDeg2Rad;
		float c = cos(angle);
		float s = sin(angle);

		float tmp[20] = {
			(GrisR + (c*(1 - GrisR))) + (s*(-GrisR)),
			(GrisG + (c* (-GrisG))) + (s*(-GrisG)),
			(GrisB + (c* (-GrisB))) + (s*(1 - GrisB)),
			0,
			0,

			(GrisR + (c* (-GrisR))) + (s* 0.143f),
			(GrisG + (c*(1.0f - GrisG))) + (s* 0.140f),
			(GrisB + (c* (-GrisB))) + (s*-0.283f),
			0,
			0,

			((GrisR + (c* (-GrisR))) + (s*(-(1.0f - GrisR)))),
			((GrisG + (c* (-GrisG))) + (s*GrisG)),
			((GrisB + (c*(1 - GrisB))) + (s*GrisB)),
			0,
			0,

			0, 0, 0, 1, 0
		};
		Concat(tmp);
	}

	ColorMatrix *Multiply(float r = 1., float g = 1., float b = 1., float a = 1.) {
		float tmp[20] = {
			r, 0, 0, 0, 0,
			0, g, 0, 0, 0,
			0, 0, b, 0, 0,
			0, 0, 0, a, 0
		};
		Concat(tmp);
		return this;
	}

	// getter
	Matrix4x4f GetMatrix() {
		Matrix4x4f result;
		result.Set(m_Data[0], m_Data[1], m_Data[2], m_Data[4],
			m_Data[5], m_Data[6], m_Data[7], m_Data[9],
			m_Data[10], m_Data[11], m_Data[12], m_Data[14],
			m_Data[15], m_Data[16], m_Data[17], m_Data[19]);

		return result;
	}

	float *GetOffset() {
		m_Offset[0] = m_Data[4] * toFloat;
		m_Offset[1] = m_Data[9] * toFloat;
		m_Offset[2] = m_Data[14] * toFloat;
		m_Offset[3] = m_Data[19] * toFloat;
		return m_Offset;
	}

	// for debug 
	uint32 Apply(uint32 argb) {
		float a = float((argb >> 24) & 0xFF);
		float r = float((argb >> 16) & 0xFF);
		float g = float((argb >> 8) & 0xFF);
		float b = float(argb & 0xFF);

		int r2 = int32(r*m_Data[0] + g*m_Data[1] + b*m_Data[2] + a*m_Data[3] + m_Data[4]);
		int g2 = int32(r*m_Data[5] + g*m_Data[6] + b*m_Data[7] + a*m_Data[8] + m_Data[9]);
		int b2 = int32(r*m_Data[10] + g*m_Data[11] + b*m_Data[12] + a*m_Data[13] + m_Data[14]);
		int a2 = int32(r*m_Data[15] + g*m_Data[16] + b*m_Data[17] + a*m_Data[18] + m_Data[19]);

		if (a2<0) a2 = 0;
		if (a2>0xFF) a2 = 0xFF;
		if (r2<0) r2 = 0;
		if (r2>0xFF) r2 = 0xFF;
		if (g2<0) g2 = 0;
		if (g2>0xFF) g2 = 0xFF;
		if (b2<0) b2 = 0;
		if (b2>0xFF) b2 = 0xFF;

		return a2 << 24 | r2 << 16 | g2 << 8 | b2;
	}

	void Trace() {
		printf("<ColorMatrix>");
		for (int i = 0; i<20; i++) printf((i % 5 == 0) ? "\n\t%f" : " %f", m_Data[i]);
		printf("\n</ColorMatrix>\n");
	}

};

QIMP_END_NAMESPACE

#endif//__MATH_COLOR_MATRIX_H__