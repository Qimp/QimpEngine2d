#include "common/Qimp.h"
#include "common/TimeManager.h"

QIMP_BEGIN_NAMESPACE

static double timerInvFreq;
static void timerInit()
{
	LARGE_INTEGER ticksPerSec;
	QueryPerformanceFrequency(&ticksPerSec);
	timerInvFreq = 1000.0 / ticksPerSec.QuadPart;
}

static int64 timerReadTicks()
{
	LARGE_INTEGER qpft;
	QueryPerformanceCounter(&qpft);
	return qpft.QuadPart;
}

TimeManager::TimeManager()
{
	m_CurTime = GetTimeSinceStartup();
	m_LastTime = m_CurTime;
	m_DeltaTime = 0;
	m_Scale = 1.0;
}

int64 TimeManager::GetTimeSinceStartup()
{
	static int64 startTicks = -1;
	if (startTicks == -1)
	{
		timerInit();
		startTicks = timerReadTicks();
	}
	int64 nowTicks = timerReadTicks();
	return (int64)((nowTicks - startTicks) * timerInvFreq);
}

void TimeManager::Update()
{
	m_LastTime = m_CurTime;
	m_CurTime = GetTimeSinceStartup();
	m_DeltaTime = (int64)std::max(1.0f, (m_CurTime - m_LastTime) * m_Scale);
}

QIMP_END_NAMESPACE

