#include "Private.h"

QIMP_BEGIN_NAMESPACE

#define DEFAULT_SCALE 10.0f

OBB::OBB()
{
	Reset();
}

OBB::OBB(const AABB& aabb)
{
	Reset();

	m_Center = aabb.m_Center;
	m_Rotation = Quaternionf(0.0f, 0.0f, 0.0f, 1.0f);
	m_Scale = aabb.m_Extent;
	SetDirty(true);
}

OBB::OBB(const OBB &obb)
{
	m_Center = obb.m_Center;
	m_Rotation = obb.m_Rotation;
	m_Scale = obb.m_Scale;
	m_WorldTM.SetIdentity();
	m_Corners.resize(8);
	SetDirty(true);
}

//	判断一点是否在OBB包围盒内
bool OBB::ContainPoint(const Vector3f& point) const
{
	//相当于将点坐标从世界坐标系中转换到了OBB包围盒的物体坐标系中
	Vector3f center = m_Center + m_WorldTM.GetPosition();
	Quaternionf quat = m_WorldTM.GetRotation() * m_Rotation;
	Vector3f vd = point - center;

	Vector3f d = vd * quat;
	if (d.x > m_Scale.x || d.x < -m_Scale.x) return false;
	if (d.y > m_Scale.y || d.y < -m_Scale.y) return false;
	if (d.z > m_Scale.z || d.z < -m_Scale.z) return false;

	return true;
}

OBB::OBB(const Vector3f& center, const Quaternionf& rot, const Vector3f& scale)
{
	m_Center = center;
	m_Rotation = rot;
	m_Scale = scale;
	m_WorldTM.SetIdentity();
	m_Corners.resize(8);
	SetDirty(true);
}

OBB::OBB(const Vector3f &from, const Vector3f & to)
{
	Reset(from, to);
}

void OBB::Reset(const Vector3f &from, const Vector3f & to)
{
	Vector3f vec = to - from;
	Vector3f yaxis(0.0f, 1.0f, 0.0f);
	float len = vec.Length();
	vec = Normalize(vec);
	if (fabs(vec.y - 1.0f) > EPSILON)
	{
		Vector3f rotationAxis = Cross(yaxis, vec);
		rotationAxis = Normalize(rotationAxis);
		Vector3f dist = vec - yaxis;
		float angle = 2.0f * asin(0.5f * dist.Length());
		m_Rotation = AxisAngleToQuaternion(rotationAxis, angle);
	}
	else
	{
		m_Rotation = Quaternionf(0.0f, 0.0f, 0.0f, 1.0f);
	}

	m_Center = (from + to) * 0.5f;
	m_Scale.y = len/2;
	m_Scale.x = DEFAULT_SCALE;
	m_Scale.z = DEFAULT_SCALE;
	m_WorldTM.SetIdentity();
	m_Corners.resize(8);
	SetDirty(true);
}

void OBB::Reset()
{
	m_Center = Vector3f(0.0f, 0.0f, 0.0f);
	m_Rotation = Quaternionf(0.0f, 0.0f, 0.0f, 1.0f);
	m_Scale = Vector3f(1.0f, 1.0f, 1.0f);
	m_WorldTM.SetIdentity();
	m_Corners.resize(8);
	SetDirty(true);
}

//	获取顶点信息  
vector<Vector3f>* OBB::GetCorners() const
{
	if (IsDirty())
	{
		SetDirty(false);
		Vector3f v[8];
		Vector3f center = m_Center + m_WorldTM.GetPosition();
		Quaternionf quat = m_WorldTM.GetRotation() * m_Rotation;
		v[0].x = -m_Scale.x; v[0].y = -m_Scale.y; v[0].z = -m_Scale.z;
		v[1].x = +m_Scale.x; v[1].y = -m_Scale.y; v[1].z = -m_Scale.z;
		v[2].x = +m_Scale.x; v[2].y = +m_Scale.y; v[2].z = -m_Scale.z;
		v[3].x = -m_Scale.x; v[3].y = +m_Scale.y; v[3].z = -m_Scale.z;
		v[4].x = -m_Scale.x; v[4].y = -m_Scale.y; v[4].z = +m_Scale.z;
		v[5].x = +m_Scale.x; v[5].y = -m_Scale.y; v[5].z = +m_Scale.z;
		v[6].x = +m_Scale.x; v[6].y = +m_Scale.y; v[6].z = +m_Scale.z;
		v[7].x = -m_Scale.x; v[7].y = +m_Scale.y; v[7].z = +m_Scale.z;
		for (int i = 0; i < 8; i++)
		{
			m_Corners[i] = v[i] * quat + center;
		}
	}
	return &m_Corners;
}

//将点投影到坐标轴  
float OBB::ProjectPoint(const Vector3f& point, const Vector3f& axis)const
{
	float dot = Dot(axis, point);
	float ret = dot * point.Length();
	return ret;
}

//计算最大最小投影值  
void OBB::GetInterval(const OBB& box, const Vector3f& axis, float &minValue, float &maxValue)const
{
	vector<Vector3f> corners = *box.GetCorners();
	float value;
	//分别投影八个点，取最大和最小值  
	minValue = maxValue = ProjectPoint(axis, corners[0]);
	for (int i = 1; i < 8; i++)
	{
		value = ProjectPoint(axis, corners[i]);
		minValue = std::min(minValue, value);
		maxValue = std::max(maxValue, value);
	}
}

//取边的矢量  
Vector3f OBB::GetEdgeDirection(int index) const
{
	vector<Vector3f> corners = *GetCorners();

	Vector3f tmpLine;
	switch (index)
	{
	case 0:// x轴方向  
		tmpLine = corners[1] - corners[0];
		tmpLine = Normalize(tmpLine);
		break;
	case 1:// y轴方向  
		tmpLine = corners[3] - corners[0];
		tmpLine = Normalize(tmpLine);
		break;
	case 2:// z轴方向  
		tmpLine = corners[4] - corners[6];
		tmpLine = Normalize(tmpLine);
		break;
	default:
		break;
	}
	return tmpLine;
}

//取面的方向矢量  
Vector3f OBB::GetFaceDirection(int index) const
{
	vector<Vector3f> corners = *GetCorners();

	Vector3f faceDirection, v0, v1;
	switch (index)
	{
	case 0:	//	前/后 计算一个与Z轴平行的矢量
		v0 = corners[1] - corners[0];
		v1 = corners[3] - corners[0];
		faceDirection = Cross(v0, v1);
		faceDirection = Normalize(faceDirection);
		break;
	case 1://	左/右 计算一个与Y轴平行的矢量
		v0 = corners[4] - corners[0];
		v1 = corners[1] - corners[0];
		faceDirection = Cross(v0, v1);
		faceDirection = Normalize(faceDirection);
		break;
	case 2:// 上/下 计算为一个与X轴平行的矢量  
		v0 = corners[3] - corners[0];
		v1 = corners[4] - corners[0];
		faceDirection = Cross(v0, v1);
		faceDirection = Normalize(faceDirection);
		break;
	default:
		break;
	}
	return faceDirection; //返回方向矢量  
}

//	检测两个OBB包围盒是否重合  
bool OBB::Intersects(const OBB& box) const
{
	float min1, max1, min2, max2;
	//当前包围盒的三个面方向 相当于取包围盒的三个坐标轴为分离轴并计算投影作比较  
	for (int i = 0; i < 3; i++)
	{
		GetInterval(*this, GetFaceDirection(i), min1, max1);//计算当前包围盒在某轴上的最大最小投影值  
		GetInterval(box, GetFaceDirection(i), min2, max2);//计算另一个包围盒在某轴上的最大最小投影值  
		if (max1 < min2 || max2 < min1) return false; //判断分离轴上投影是否重合  
	}
	//box包围盒的三个面方向  
	for (int i = 0; i < 3; i++)
	{
		GetInterval(*this, box.GetFaceDirection(i), min1, max1);
		GetInterval(box, box.GetFaceDirection(i), min2, max2);
		if (max1 < min2 || max2 < min1) return false;
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Vector3f axis = Cross(GetEdgeDirection(i), box.GetEdgeDirection(j));
			GetInterval(*this, axis, min1, max1);
			GetInterval(box, axis, min2, max2);
			if (max1 < min2 || max2 < min1) return false;
		}
	}

	return true;
}

//由一个给定矩阵对OBB包围盒进行变换  
void OBB::Transform(const Matrix4x4f& mat)
{
	// 新的中心点  
	m_Center = m_Center + mat.GetPosition();
	m_Rotation = mat.GetRotation() * m_Rotation;
	//m_Scale.Scale(mat.GetScale());
	SetDirty(true);
}

void OBB::SetWorldTM(const Matrix4x4f& mat)
{
	m_WorldTM = mat;
	SetDirty(true);
}

const Matrix4x4f& OBB::GetWorldTM() const
{
	return m_WorldTM;
}

QIMP_END_NAMESPACE