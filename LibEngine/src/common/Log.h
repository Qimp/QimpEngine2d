#ifndef __LOG__H__
#define __LOG__H__

QIMP_BEGIN_NAMESPACE

enum EnumLogLevelFlag
{
	kLogLevelNormal = 0,
	kLogLevelDebug,
	kLogLevelLowWarning,
	kLogLevelWarning,
	kLogLevelError,
};

void LogByLevel(EnumLogLevelFlag e, const char* szFormat, ...);

QIMP_END_NAMESPACE


#endif