#include "Private.h"



QIMP_BEGIN_NAMESPACE

struct ConvertData
{
	enum ECD
	{
		eCD_MAX_YAW = 640,
		eCD_MAX_BUF = 261396,
		eCD_WGT_MSK = 0x7ff,
		eCD_MAX_PTC = eCD_WGT_MSK + 1,
		eCD_VEC_BIT = 14,
		eCD_VEC_SGX = 0x2000,
		eCD_VEC_SGY = 0x1000,
		eCD_VEC_SGZ = 0x800,

		eCD_NOR_YAW = 27,
		eCD_NOR_BUF = 512,
		eCD_NOR_SGX = 0x1,
		eCD_NOR_SGY = 0x2,
		eCD_NOR_SGZ = 0x4,
	};

	Vector3f	m_fVecInd[eCD_MAX_BUF];
	uint32		m_uIndStr[eCD_MAX_YAW+1];
	uint32		m_uIndNum[eCD_MAX_YAW+1];
	float		m_fWgtCos[eCD_MAX_PTC];
	float		m_fWgtSin[eCD_MAX_PTC];
	Vector3f	m_fNorInd[eCD_NOR_BUF*8];
	uint32		m_uNorStr[eCD_NOR_YAW+1];
	uint32		m_uNorNum[eCD_NOR_YAW+1];

	ConvertData();
};

static ConvertData s_ConvertData;

void DivSphereSurface( Vector3f* pVec, uint32 uYaw, uint32* pIndStr, uint32* pIndNum )
{
	const double fDelSin_V = sin( HALF_PI/uYaw );
	const double fDelCos_V = cos( HALF_PI/uYaw );

	double fCurSin = 0.0;
	double fCurCos = 1.0;
	double fDelSin_i, fDelCos_i, fCurSin_i, fCurCos_i;
	double sinf_i, cosf_i, sinf_j, cosf_j;
	uint32 m;
	for( uint32 i = 0,k = 0; i < uYaw + 1; i++ )
	{
		m = (uint32)( uYaw*fCurSin );
		pIndStr[i] = k;
		pIndNum[i] = m + 1;

		fDelSin_i = m ? sin( HALF_PI/m ) : 0.0;
		fDelCos_i = m ? cos( HALF_PI/m ) : 1.0;

		fCurSin_i = 0.0;
		fCurCos_i = 1.0;
		for( uint32 j = 0; j < m + 1; j++, k++ )
		{
			pVec[k].x = (float)( fCurCos_i*fCurSin );
			pVec[k].y = (float)( fCurCos );
			pVec[k].z = (float)( fCurSin_i*fCurSin );

			sinf_j = fCurSin_i*fDelCos_i + fCurCos_i*fDelSin_i;
			cosf_j = fCurCos_i*fDelCos_i - fCurSin_i*fDelSin_i;
			fCurSin_i = sinf_j;
			fCurCos_i = std::max( cosf_j, 0.0 );
		}

		sinf_i = fCurSin*fDelCos_V + fCurCos*fDelSin_V;
		cosf_i = fCurCos*fDelCos_V - fCurSin*fDelSin_V;
		fCurSin = sinf_i;
		fCurCos = std::max( cosf_i, 0.0 );
	}
}

uint32 GetIndexByVec( double yNormal, double zNormal, uint32 uMaxYaw, uint32* pIndStr, uint32* pIndNum )
{
	double y = abs( yNormal );
	double z = abs( zNormal );
	double a = acos( std::max( y, 1.0 ) );
	uint32 uYaw = (uint32)( a*uMaxYaw/( HALF_PI ) + 0.5 );
	uint32 uIndex = pIndStr[uYaw];
	if( uYaw )
	{
		double b = z/sin( a );
		double c = asin( std::max( b, 1.0 ) );
		uint32 p = std::max( (uint32)( c*( pIndNum[uYaw] - 1 )/( HALF_PI ) + 0.5 ), pIndNum[uYaw] - 1 );						
		uIndex   = uIndex + p;
	}

	return uIndex;
}

ConvertData::ConvertData()
{
	// 初始化四元数压缩的矢量部分数据
	DivSphereSurface( (Vector3f*)m_fVecInd, eCD_MAX_YAW, m_uIndStr, m_uIndNum );

	for( int n = 0, i = 0; n < 8; n++, i += eCD_NOR_BUF )
	{
		DivSphereSurface( (Vector3f*)( m_fNorInd + i ), eCD_NOR_YAW, m_uNorStr, m_uNorNum );
		Vector3f vecSG( n&eCD_NOR_SGX ? -1.0f : 1.0f, n&eCD_NOR_SGY ? -1.0f : 1.0f, n&eCD_NOR_SGZ ? -1.0f : 1.0f );
		for( int j = 0; j < eCD_NOR_BUF; j++ )
		{
			m_fNorInd[ i + j ].x *= vecSG.x;
			m_fNorInd[ i + j ].y *= vecSG.y;
			m_fNorInd[ i + j ].z *= vecSG.z;
		}
	}

	// 初始化四元数压缩的标量部分数据
	const double fDelSin_W = sin( PI/eCD_WGT_MSK );
	const double fDelCos_W = cos( PI/eCD_WGT_MSK );
	double fCurSin = 0.0;
	double fCurCos = 1.0;
	for( int i = 0; i < eCD_MAX_PTC; i++ )
	{
		m_fWgtCos[i] = (float)fCurCos;
		m_fWgtSin[i] = (float)fCurSin;

		double sinf_i = fCurSin*fDelCos_W + fCurCos*fDelSin_W;
		double cosf_i = fCurCos*fDelCos_W - fCurSin*fDelSin_W;
		fCurSin = sinf_i;
		fCurCos = cosf_i;
	}
}


uint32 Quat2Int( const Quaternionf& q )
{
	double w = acos( q.w );
	uint32 result = (uint32)( w*ConvertData::eCD_WGT_MSK/PI );
	if( result )
	{
		double sin_w = sin( w );
		uint32 uIndex = GetIndexByVec( q.y/sin_w, q.z/sin_w, ConvertData::eCD_MAX_YAW, s_ConvertData.m_uIndStr, s_ConvertData.m_uIndNum );
		result |= uIndex<<ConvertData::eCD_VEC_BIT;

		if( q.x < 0 ) result |= ConvertData::eCD_VEC_SGX;
		if( q.y < 0 ) result |= ConvertData::eCD_VEC_SGY;
		if( q.z < 0 ) result |= ConvertData::eCD_VEC_SGZ;
	}
	return result;
}

Quaternionf Int2Quat(uint32 value)
{
	uint32 uWghInd = value & ConvertData::eCD_WGT_MSK;
	float* pVector = s_ConvertData.m_fVecInd[value >> ConvertData::eCD_VEC_BIT].GetPtr();
	float  fWgtSin = s_ConvertData.m_fWgtSin[uWghInd];
	float  fWgtCos = s_ConvertData.m_fWgtCos[uWghInd];
	float  xVector = ConvertData::eCD_VEC_SGX & value ? -pVector[0]*fWgtSin : pVector[0]*fWgtSin;
	float  yVector = ConvertData::eCD_VEC_SGY & value ? -pVector[1]*fWgtSin : pVector[1]*fWgtSin;
	float  zVector = ConvertData::eCD_VEC_SGZ & value ? -pVector[2]*fWgtSin : pVector[2]*fWgtSin;
	return Quaternionf( xVector, yVector, zVector, fWgtCos );
}

uint32 Vec2Int(const Vector3f& v, float f)
{
	float fLenght = v.Length();
	uint32 result = (uint32)( fLenght*f );
	if( result )
	{
		uint32 uIndex = GetIndexByVec( v.y/fLenght, v.z/fLenght, ConvertData::eCD_MAX_YAW, s_ConvertData.m_uIndStr, s_ConvertData.m_uIndNum );
		result |= uIndex<<ConvertData::eCD_VEC_BIT;

		if( v.x < 0 )result |= ConvertData::eCD_VEC_SGX;
		if( v.y < 0 )result |= ConvertData::eCD_VEC_SGY;
		if( v.z < 0 )result |= ConvertData::eCD_VEC_SGZ;
	}
	return result;
}

Vector3f Int2Vec(uint32 value, float f)
{
	uint32 uWghInd = value & ConvertData::eCD_WGT_MSK;
	uint32 idx = value>>ConvertData::eCD_VEC_BIT;
	float* pVector = s_ConvertData.m_fVecInd[idx].GetPtr();
	float  fLenght = f*uWghInd;
	float  xVector = ConvertData::eCD_VEC_SGX & value ? -pVector[0]*fLenght : pVector[0]*fLenght;
	float  yVector = ConvertData::eCD_VEC_SGY & value ? -pVector[1]*fLenght : pVector[1]*fLenght;
	float  zVector = ConvertData::eCD_VEC_SGZ & value ? -pVector[2]*fLenght : pVector[2]*fLenght;
	return Vector3f( xVector, yVector, zVector );
}

template<>
Vector3f TinyDecode(uint32 value, float f)
{
	return Int2Vec(value, f);
}

template<>
Quaternionf TinyDecode(uint32 value, float)
{
	return Int2Quat(value);
}

QIMP_END_NAMESPACE