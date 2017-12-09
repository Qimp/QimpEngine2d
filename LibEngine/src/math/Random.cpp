#include "Private.h"

QIMP_BEGIN_NAMESPACE
Rand*			g_Rand = NULL;

float RangedRandom(float min, float max)
{
	float t = g_Rand->GetFloat();
	t = min * t + (1.0F - t) * max;
	return t;
}

float Random01()
{
	return g_Rand->GetFloat();
}

int RangedRandom(int min, int max)
{
	int dif;
	if (min < max)
	{
		dif = max - min;
		int t = g_Rand->Get() % dif;
		t += min;
		return t;
	}
	else if (min > max)
	{
		dif = min - max;
		int t = g_Rand->Get() % dif;
		t = min - t;
		return t;
	}
	else
	{
		return min;
	}
}

Vector3f RandomUnitVector()
{
	float z = RangedRandom(-1.0f, 1.0f);
	float a = RangedRandom(0.0f, (float)TWO_PI);

	float r = sqrt(1.0f - z*z);

	float x = r * cos(a);
	float y = r * sin(a);

	return Vector3f(x, y, z);
}

Vector2f RandomUnitVector2()
{
	float a = RangedRandom(0.0f, (float)TWO_PI);

	float x = cos(a);
	float y = sin(a);

	return Vector2f(x, y);
}


Quaternionf RandomQuaternion()
{
	Quaternionf q;
	q.x = RangedRandom(-1.0f, 1.0f);
	q.y = RangedRandom(-1.0f, 1.0f);
	q.z = RangedRandom(-1.0f, 1.0f);
	q.w = RangedRandom(-1.0f, 1.0f);
	q = NormalizeSafe(q);
	if (Dot(q, Quaternionf::identity()) < 0.0f)
		return -q;
	else
		return q;
}

Quaternionf RandomQuaternionUniformDistribution()
{
	// Employs Hopf fibration to uniformly distribute quaternions
	float u1 = RangedRandom(0.0f, 1.0f);
	float theta = RangedRandom(0.0f, (float)TWO_PI);
	float rho = RangedRandom(0.0f, (float)TWO_PI);

	float i = sqrt(1.0f - u1);
	float j = sqrt(u1);

	// We do not need to normalize the generated quaternion, because the probability density corresponds to the Haar measure.
	// This means that a random rotation is obtained by picking a point at random on S^3, and forming the unit quaternion.
	Quaternionf q(i * sin(theta), i * cos(theta), j * sin(rho), j * cos(rho));

	if (Dot(q, Quaternionf::identity()) < 0.0f)
		return -q;
	else
		return q;
}


Vector3f RandomPointInsideCube(const Vector3f& extents)
{
	return Vector3f(RangedRandom(-extents.x, extents.x),
		RangedRandom(-extents.y, extents.y),
		RangedRandom(-extents.z, extents.z));
}

Vector3f RandomPointBetweenCubes(const Vector3f& min, const Vector3f& max)
{
	Vector3f v;
	int i;
	for (i = 0; i<3; i++)
	{
		float x = g_Rand->GetFloat() * 2.0F - 1.0F;
		if (x > 0.0f)
			v[i] = min[i] + x * (max[i] - min[i]);
		else
			v[i] = -min[i] + x * (max[i] - min[i]);
	}
	return v;
}

Vector3f RandomPointInsideUnitSphere()
{
	Vector3f v = RandomUnitVector();
	v *= pow(Random01(), 1.0F / 3.0F);
	return v;
}

Vector3f RandomPointInsideEllipsoid(const Vector3f& extents)
{
	return Scale(RandomPointInsideUnitSphere(), extents);
}

Vector3f RandomPointBetweenSphere(float minRadius, float maxRadius)
{
	Vector3f v = RandomUnitVector();
	// As the volume of the sphere increases (x^3) over an interval we have to increase range as well with x^(1/3)
	float range = pow(RangedRandom(0.0F, 1.0F), 1.0F / 3.0F);
	return v * (minRadius + (maxRadius - minRadius) * range);
}

Vector2f RandomPointInsideUnitCircle()
{
	Vector2f v = RandomUnitVector2();
	// As the volume of the sphere increases (x^3) over an interval we have to increase range as well with x^(1/3)
	v *= pow(RangedRandom(0.0F, 1.0F), 1.0F / 2.0F);
	return v;
}

Vector3f RandomPointBetweenEllipsoid(const Vector3f& maxExtents, float minRange)
{
	Vector3f v = Scale(RandomUnitVector(), maxExtents);
	// As the volume of the sphere increases (x^3) over an interval we have to increase range as well with x^(1/3)
	float range = pow(RangedRandom(minRange, 1.0F), 1.0F / 3.0F);
	return v * range;
}

/// Builds a random Barycentric coordinate which can be used to generate random points on a triangle:
/// Vector3f point = v0 * barycentric.x + v1 * barycentric.y + v2 * barycentric.z;
Vector3f RandomBarycentricCoord()
{
	// Was told that this leads to bad distribution because of the 1.0F - s
	//	float s = gRand.GetFloat ();
	//	float t = RangedRandom (gRand, 0.0F, 1.0F - s);
	//	float r = (1.0F - s - t);
	//	Vector3f positionOnMesh = r * vertices[face.v1] + s * vertices[face.v2] + t * vertices[face.v3];
	//	return positionOnMesh;
	float u = g_Rand->GetFloat();
	float v = g_Rand->GetFloat();
	if (u + v > 1.0F)
	{
		u = 1.0F - u;
		v = 1.0F - v;
	}
	float w = 1.0F - u - v;
	return Vector3f(u, v, w);
}

QIMP_END_NAMESPACE