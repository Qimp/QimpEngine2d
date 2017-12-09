#pragma once

QIMP_BEGIN_NAMESPACE

class Sphere
{
public:
	Sphere();
	~Sphere();
	void				SetWorldTM(const Matrix4x4f& mat);
	const Matrix4x4f &	GetWorldTM() const;
	void				SetPosition(const Vector3f &pos) { m_Position = pos; };
	void				SetPosition(float x, float y, float z) { m_Position = Vector3f(x, y, z); };
	Vector3f			GetPosition() const { return m_Position; };
	Vector3f			GetFinalPosition() const;
	void				SetRadius(float radius) { m_fRadius = radius; };
	float				GetRadius() const { return m_fRadius; };
private:
	Matrix4x4f			m_WorldTM;
	Vector3f			m_Position;
	float				m_fRadius;
};

QIMP_END_NAMESPACE