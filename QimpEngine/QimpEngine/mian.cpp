#include "main.h"

#include "common/Qimp.h"
#include "common/Global.h"
#include "math/MathPublic.h"
#include "common/Win32.h"

QIMP_USING_NAMESPACE

#pragma comment(lib, "Dbghelp.lib")
#include <Dbghelp.h>

int GenerateMiniDump(PEXCEPTION_POINTERS pExceptionPointers)
{
	// 创建 dmp 文件件
	TCHAR szFileName[MAX_PATH] = { 0 };
	TCHAR* szVersion = _T("coredump_");
	SYSTEMTIME stLocalTime;
	GetLocalTime(&stLocalTime);
	wsprintf(szFileName, L"%s-%04d%02d%02d-%02d%02d%02d.dmp",
		szVersion, stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay,
		stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond);
	HANDLE hDumpFile = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);
	if (INVALID_HANDLE_VALUE == hDumpFile)
	{
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	// 写入 dmp 文件
	MINIDUMP_EXCEPTION_INFORMATION expParam;
	expParam.ThreadId = GetCurrentThreadId();
	expParam.ExceptionPointers = pExceptionPointers;
	expParam.ClientPointers = FALSE;
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
		hDumpFile, MiniDumpWithDataSegs, (pExceptionPointers ? &expParam : NULL), NULL, NULL);
	// 释放文件
	CloseHandle(hDumpFile);
	MessageBoxA(0, "fuck 程序异常 crash!!!", "error", MB_OK);
	return EXCEPTION_EXECUTE_HANDLER;
}

LONG WINAPI ExceptionFilter(LPEXCEPTION_POINTERS lpExceptionInfo)
{
	if (IsDebuggerPresent())// 这里做一些异常的过滤或提示
	{
		return EXCEPTION_CONTINUE_SEARCH;
	}
	return GenerateMiniDump(lpExceptionInfo);
}

int CALLBACK WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	SetUnhandledExceptionFilter(ExceptionFilter);

	Win32::InitGame();
	{
		g_Win32->InitWin("Test", 500, 30);
		AllocConsole();
		FILE *stream;
		freopen_s(&stream, "CONIN$", "r", stdin);
		freopen_s(&stream, "CONOUT$", "w", stdout);
		freopen_s(&stream, "CONOUT$", "w", stderr);
		DragAcceptFiles(g_Win32->GetHWnd(), TRUE);			// 开启支持窗口拖放

		g_Win32->GameStart();
		g_Win32->AutoSetWinPos();
		g_Win32->Show();

		int Fps = 0;
		g_Win32->Loop(Fps);
	}
	Win32::QuitGame();

	return 0;
}


