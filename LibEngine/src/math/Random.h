#ifndef __MATH_RANDOM_H__
#define __MATH_RANDOM_H__

QIMP_BEGIN_NAMESPACE

float RangedRandom(float min, float max);
float Random01();
int RangedRandom(int min, int max);
Vector3f RandomUnitVector();
Vector2f RandomUnitVector2();
Quaternionf RandomQuaternion();
Quaternionf RandomQuaternionUniformDistribution();
Vector3f RandomPointInsideCube(const Vector3f& extents);
Vector3f RandomPointBetweenCubes(const Vector3f& min, const Vector3f& max);
Vector3f RandomPointInsideUnitSphere();
Vector3f RandomPointInsideEllipsoid(const Vector3f& extents);
Vector3f RandomPointBetweenSphere(float minRadius, float maxRadius);
Vector2f RandomPointInsideUnitCircle();
Vector3f RandomPointBetweenEllipsoid(const Vector3f& maxExtents, float minRange);
Vector3f RandomBarycentricCoord();

QIMP_END_NAMESPACE

#endif//__MATH_RANDOM_H__
