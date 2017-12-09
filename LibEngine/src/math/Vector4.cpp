#include "Private.h"


#define FPFIXES 1

using namespace std;

QIMP_BEGIN_NAMESPACE
	
const float		Vector4f::epsilon = 0.00001F;
const float		Vector4f::infinity = numeric_limits<float>::infinity ();
const Vector4f	Vector4f::infinityVec = Vector4f(numeric_limits<float>::infinity(), numeric_limits<float>::infinity(), numeric_limits<float>::infinity(), numeric_limits<float>::infinity());

const Vector4f	Vector4f::zero = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
const Vector4f	Vector4f::one  = Vector4f (1.0f, 1.0f, 1.0f, 1.0f);



QIMP_END_NAMESPACE