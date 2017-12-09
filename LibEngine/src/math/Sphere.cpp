#include "Private.h"

QIMP_BEGIN_NAMESPACE

Sphere::Sphere()
{
	m_WorldTM = Matrix4x4f::identity;
	m_Position = Vector3f::zero;
	m_fRadius = 1.0f;
}

Sphere::~Sphere()
{

}

Vector3f Sphere::GetFinalPosition() const
{
	return m_Position + m_WorldTM.GetPosition();
}

void Sphere::SetWorldTM(const Matrix4x4f& mat)
{
	m_WorldTM = mat;
}

const Matrix4x4f& Sphere::GetWorldTM() const
{
	return m_WorldTM;
}

QIMP_END_NAMESPACE