#ifndef __TIMEMAMAGER__H__
#define __TIMEMAMAGER__H__

QIMP_BEGIN_NAMESPACE

class TimeManager
{
public:
	TimeManager();

	int64					GetTimeSinceStartup();

	int64					GetCurTime()						{ return m_CurTime; };
	int64					GetLastTime()						{ return m_LastTime; };
	int64					GetDeltaTime()						{ return m_DeltaTime; };
	void					SetTimeScale(float scale);	

	void					Update();

private:
	int64					m_CurTime;
	int64					m_LastTime;
	int64					m_DeltaTime;
	float					m_Scale;

};

QIMP_END_NAMESPACE

#endif