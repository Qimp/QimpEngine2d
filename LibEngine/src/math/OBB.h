#pragma once

QIMP_BEGIN_NAMESPACE

class OBB
{
public:
	OBB();
	OBB(const AABB& aabb);
	OBB(const OBB& obb);
	OBB(const Vector3f& center, const Quaternionf& rot, const Vector3f& scale);
	OBB(const Vector3f &from, const Vector3f & to);

	//	判断点是否在一个OBB包围盒内
	bool			ContainPoint(const Vector3f& point) const;

	void			SetPosition(const Vector3f &center) { m_Center = center; SetDirty(true); };
	void			SetPosition(float x, float y, float z) { m_Center = Vector3f(x, y, z); SetDirty(true); };
	Vector3f		GetPosition() const { return m_Center; };
	Vector3f		GetFinalPosition() const { return m_Center + m_WorldTM.GetPosition(); };
	void			SetScale(const Vector3f & scale) { m_Scale = scale; SetDirty(true); };
	void			SetScale(float x, float y, float z) { m_Scale = Vector3f(x, y, z); SetDirty(true); };
	Vector3f		GetScale() const { return m_Scale; };
	void			SetRotation(const Quaternionf& rot) { m_Rotation = rot; SetDirty(true); };
	void			SetRotation(float x, float y, float z, float w) { m_Rotation = Quaternionf(x, y, z, w); SetDirty(true); };
	Quaternionf		GetRotation() const { return m_Rotation; };
	Quaternionf		GetFinalRotation() const { return m_WorldTM.GetRotation() * m_Rotation; };
	void			SetEulerRotation(const Vector3f & rot) { m_Rotation = EulerToQuaternion(rot); SetDirty(true); };
	void			SetEulerRotation(float x, float y, float z) { SetEulerRotation(Vector3f(x, y, z)); SetDirty(true); };
	Vector3f		GetEulerRotation() const { return QuaternionToEuler(m_Rotation); };
	void				SetWorldTM(const Matrix4x4f& mat);
	const Matrix4x4f &	GetWorldTM() const;

	void			SetDirty(bool bDirty) const { m_bDirty = bDirty; };
	bool			IsDirty() const { return m_bDirty; };
	void			Reset();
	void			Reset(const Vector3f &from, const Vector3f & to);

	/* 
	    5-----6
	   /|    /|
	  1-----2 |
	  | 4---|-7
	  |/    |/
	  0-----3
	*/
	vector<Vector3f> *	GetCorners() const;

	//	检测是否和其他OBB盒碰撞
	bool			Intersects(const OBB& box) const;

	//	由给定的变换矩阵变换OBB包围盒
	void			Transform(const Matrix4x4f& mat);

protected:
	//	将点投影到目标轴
	float			ProjectPoint(const Vector3f& point, const Vector3f& axis) const;

	//	计算最大和最小投影值
	void			GetInterval(const OBB& box, const Vector3f& axis, float &minValue, float &maxValue) const;

	//	取边的矢量 获取分离轴使用
	Vector3f		GetEdgeDirection(int index) const;

	//	取面的方向矢量 获取分离轴时使用
	Vector3f		GetFaceDirection(int index) const;

private:
	mutable bool	m_bDirty;
	Vector3f		m_Center;
	Quaternionf		m_Rotation;
	Vector3f		m_Scale;
	Matrix4x4f		m_WorldTM;
	mutable vector<Vector3f>	m_Corners;
};

inline OBB Lerp(const OBB& lhs, const OBB& rhs, float t)
{
	Vector3f center = Lerp(lhs.GetPosition(), rhs.GetPosition(), t);
	Vector3f scale = Lerp(lhs.GetScale(), rhs.GetScale(), t);
	Quaternionf rot = Lerp(lhs.GetRotation(), rhs.GetRotation(), t);

	return OBB(center, rot, scale);
}
QIMP_END_NAMESPACE