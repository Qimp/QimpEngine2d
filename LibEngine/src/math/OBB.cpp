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

//	�ж�һ���Ƿ���OBB��Χ����
bool OBB::ContainPoint(const Vector3f& point) const
{
	//�൱�ڽ����������������ϵ��ת������OBB��Χ�е���������ϵ��
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

//	��ȡ������Ϣ  
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

//����ͶӰ��������  
float OBB::ProjectPoint(const Vector3f& point, const Vector3f& axis)const
{
	float dot = Dot(axis, point);
	float ret = dot * point.Length();
	return ret;
}

//���������СͶӰֵ  
void OBB::GetInterval(const OBB& box, const Vector3f& axis, float &minValue, float &maxValue)const
{
	vector<Vector3f> corners = *box.GetCorners();
	float value;
	//�ֱ�ͶӰ�˸��㣬ȡ������Сֵ  
	minValue = maxValue = ProjectPoint(axis, corners[0]);
	for (int i = 1; i < 8; i++)
	{
		value = ProjectPoint(axis, corners[i]);
		minValue = std::min(minValue, value);
		maxValue = std::max(maxValue, value);
	}
}

//ȡ�ߵ�ʸ��  
Vector3f OBB::GetEdgeDirection(int index) const
{
	vector<Vector3f> corners = *GetCorners();

	Vector3f tmpLine;
	switch (index)
	{
	case 0:// x�᷽��  
		tmpLine = corners[1] - corners[0];
		tmpLine = Normalize(tmpLine);
		break;
	case 1:// y�᷽��  
		tmpLine = corners[3] - corners[0];
		tmpLine = Normalize(tmpLine);
		break;
	case 2:// z�᷽��  
		tmpLine = corners[4] - corners[6];
		tmpLine = Normalize(tmpLine);
		break;
	default:
		break;
	}
	return tmpLine;
}

//ȡ��ķ���ʸ��  
Vector3f OBB::GetFaceDirection(int index) const
{
	vector<Vector3f> corners = *GetCorners();

	Vector3f faceDirection, v0, v1;
	switch (index)
	{
	case 0:	//	ǰ/�� ����һ����Z��ƽ�е�ʸ��
		v0 = corners[1] - corners[0];
		v1 = corners[3] - corners[0];
		faceDirection = Cross(v0, v1);
		faceDirection = Normalize(faceDirection);
		break;
	case 1://	��/�� ����һ����Y��ƽ�е�ʸ��
		v0 = corners[4] - corners[0];
		v1 = corners[1] - corners[0];
		faceDirection = Cross(v0, v1);
		faceDirection = Normalize(faceDirection);
		break;
	case 2:// ��/�� ����Ϊһ����X��ƽ�е�ʸ��  
		v0 = corners[3] - corners[0];
		v1 = corners[4] - corners[0];
		faceDirection = Cross(v0, v1);
		faceDirection = Normalize(faceDirection);
		break;
	default:
		break;
	}
	return faceDirection; //���ط���ʸ��  
}

//	�������OBB��Χ���Ƿ��غ�  
bool OBB::Intersects(const OBB& box) const
{
	float min1, max1, min2, max2;
	//��ǰ��Χ�е������淽�� �൱��ȡ��Χ�е�����������Ϊ�����Ტ����ͶӰ���Ƚ�  
	for (int i = 0; i < 3; i++)
	{
		GetInterval(*this, GetFaceDirection(i), min1, max1);//���㵱ǰ��Χ����ĳ���ϵ������СͶӰֵ  
		GetInterval(box, GetFaceDirection(i), min2, max2);//������һ����Χ����ĳ���ϵ������СͶӰֵ  
		if (max1 < min2 || max2 < min1) return false; //�жϷ�������ͶӰ�Ƿ��غ�  
	}
	//box��Χ�е������淽��  
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

//��һ�����������OBB��Χ�н��б任  
void OBB::Transform(const Matrix4x4f& mat)
{
	// �µ����ĵ�  
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