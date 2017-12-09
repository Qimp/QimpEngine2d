#ifndef __MATH_COMMON_H__
#define __MATH_COMMON_H__


using namespace std;

QIMP_BEGIN_NAMESPACE

#ifndef EPSILON
#define EPSILON 1e-7f
#endif

#ifndef PI
#define PI       3.14159265358979323846
#endif

#ifndef TWO_PI
#define TWO_PI   6.28318530717958647693
#endif

#ifndef FOUR_PI
#define FOUR_PI 12.56637061435917295385
#endif

#ifndef HALF_PI
#define HALF_PI		1.57079632679489661923
#endif

#ifndef DEG_TO_RAD
#define DEG_TO_RAD (PI/180.0f)
#endif

#ifndef RAD_TO_DEG
#define RAD_TO_DEG (180.0f/PI)
#endif

#ifndef ABS
#define ABS(x) (((x) < 0) ? -(x) : (x))
#endif

}


#endif //__MATH_COMMON_H__
