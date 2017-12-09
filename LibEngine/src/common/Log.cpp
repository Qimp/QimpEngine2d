#include "Qimp.h"

QIMP_BEGIN_NAMESPACE

#define BUFF_SIZE 16 * 1024

void LogByLevel(EnumLogLevelFlag e, const char* szFormat, ...)
{
	//AX_MUTEX_LOCK(kMutexTypeLog);
	bool b = true;
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	switch (e){
	case kLogLevelDebug:
		SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case kLogLevelLowWarning:
		SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN);
		OutputDebugStringW(L"[warning] ");
		printf("[warning] ");
		break;
	case kLogLevelWarning:
		SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		OutputDebugStringW(L"[warning] ");
		printf("[warning] ");
		break;
	case kLogLevelError:
		SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
		OutputDebugStringW(L"[error] ");
		printf("[error] ");
		break;
	default:
		b = false;
	}

	static char szBuf[BUFF_SIZE];
	va_list ap;
	va_start(ap, szFormat);
	vsnprintf(szBuf, BUFF_SIZE, szFormat, ap);
	va_end(ap);
	static WCHAR wszBuf[BUFF_SIZE] = { 0 };
	MultiByteToWideChar(CP_UTF8, 0, szBuf, -1, wszBuf, sizeof(wszBuf));
	OutputDebugStringW(wszBuf);
	OutputDebugStringA("\n");
	WideCharToMultiByte(CP_ACP, 0, wszBuf, sizeof(wszBuf), szBuf, sizeof(szBuf), NULL, FALSE);
	printf("%s\n", szBuf);

	if (b)
		SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	//AX_MUTEX_UNLOCK(kMutexTypeLog);
}

QIMP_END_NAMESPACE
