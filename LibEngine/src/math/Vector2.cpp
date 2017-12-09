#include "Private.h"


using namespace std;

QIMP_BEGIN_NAMESPACE
const float		Vector2f::epsilon = 0.00001F;
const float		Vector2f::infinity = numeric_limits<float>::infinity ();
const Vector2f	Vector2f::infinityVec = Vector2f (numeric_limits<float>::infinity (), numeric_limits<float>::infinity ());
const Vector2f	Vector2f::zero  = Vector2f (0, 0);
const Vector2f	Vector2f::xAxis = Vector2f (1, 0);
const Vector2f	Vector2f::yAxis = Vector2f (0, 1);

QIMP_END_NAMESPACE