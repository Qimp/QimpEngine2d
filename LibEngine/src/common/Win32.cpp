#include "common/Qimp.h"
#include "common/Win32.h"
#include "common/ReleasePool.h"
#include "common/TimeManager.h"
#include "render/RenderManager.h"
#include "render/ShaderProgram.h"

QIMP_BEGIN_NAMESPACE

Win32*					g_Win32					= nullptr;
RenderManager*			g_RenderManager			= nullptr;
TimeManager*			g_TimeManager			= nullptr;


static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:				break;
	case WM_LBUTTONUP:					break;
	case WM_LBUTTONDBLCLK:				break;
	case WM_RBUTTONDOWN:				break;
	case WM_RBUTTONUP:					break;
	case WM_RBUTTONDBLCLK:				break;
	case WM_MBUTTONDOWN:				break;
	case WM_MBUTTONUP:					break;
	case WM_MOUSEMOVE:					break;
	case WM_TOUCH:						break;
	case WM_SIZE:
		// 		switch (wParam)
		// 		{
		// 		case SIZE_RESTORED:
		// 			Win32::Resume();			break;
		// 		case SIZE_MINIMIZED:
		// 			Win32::Pause();				break;
		// 		}
		break;
	case WM_KEYDOWN:					break;
	case WM_KEYUP:						break;
	case WM_CHAR:						break;
	case WM_MOUSEWHEEL:					break;
	case WM_CLOSE:
		PostQuitMessage(0);				break;
	case WM_DROPFILES:					break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

bool Win32::InitWin(const char* title, int32 w, int32 h)
{
	m_Title = title;
	m_Width = w;
	m_Height = h;

	CreateWin();
	if (!m_hWnd)
		return false;


	return true;
}

void Win32::CreateWin()
{
	char szTitle[256];
	sprintf(szTitle, "%s    (Qimp2d - %d * %d)", m_Title.c_str(), m_Width, m_Height);

	// 创建 windows 窗体
	WNDCLASS wc;
	// Redraw On Size, And Own DC For Window.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
	wc.lpfnWndProc = WindowProc;						// WndProc Handles Messages
	wc.cbClsExtra = 0;									// No Extra Window Data
	wc.cbWndExtra = 0;									// No Extra Window Data
	wc.hInstance = NULL;								// Set The Instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);				// Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground = NULL;							// No Background Required For GL
	wc.lpszMenuName = NULL;								//
	wc.lpszClassName = L"QIMP";							// Set The Class Name
	if (RegisterClass(&wc) && 1410 == GetLastError())
	{
		QIMP_LOGL(kLogLevelError, "CreateWindow Failed!");
		return ;
	}
	WCHAR wszBuf[255] = { 0 };
	MultiByteToWideChar(CP_UTF8, 0, szTitle, -1, wszBuf, sizeof(wszBuf));
	m_hWnd = CreateWindowEx(
		WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,					// Extended Style For The Window
		L"QIMP",											// Class Name
		wszBuf,												// Window Title
		WS_CAPTION | WS_POPUPWINDOW | WS_MINIMIZEBOX,		// Defined Window Style
		0, 0,												// Window Position
		//TODO: Initializing width with a large value to avoid getting a wrong client area by 'GetClientRect' function.
		m_Width, m_Height,										// Window Width, Height
		NULL,												// No Parent Window
		NULL,												// No Menu
		NULL,												// Instance
		NULL);
}

Win32::Win32()
: m_hWnd(nullptr)
, m_hDC(nullptr)
, m_Width(300)
, m_Height(300)
, m_FpsCount(0)
, m_Fps(0.f)
{

}

void Win32::AutoSetWinPos()
{
	RECT rcWindow;
	GetWindowRect(m_hWnd, &rcWindow);

	RECT rcClient;
	GetClientRect(m_hWnd, &rcClient);

	// calculate new window width and height
	POINT ptDiff;
	ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
	ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;
	rcClient.right = rcClient.left + m_Width;
	rcClient.bottom = rcClient.top + m_Height;

	AdjustWindowRectEx(&rcClient, GetWindowLong(m_hWnd, GWL_STYLE), FALSE, GetWindowLong(m_hWnd, GWL_EXSTYLE));

	// change width and height
	SetWindowPos(m_hWnd, 0, 0, 0, m_Width + ptDiff.x, m_Height + ptDiff.y,
		SWP_NOCOPYBITS | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
}

void Win32::Show()
{
	RECT rcWindow;
	GetWindowRect(m_hWnd, &rcWindow);
	RECT rcClient;
	GetClientRect(m_hWnd, &rcClient);
	int diff_w = (rcWindow.right - rcWindow.left) - (rcClient.right - rcClient.left);
	int diff_h = (rcWindow.bottom - rcWindow.top) - (rcClient.bottom - rcClient.top);

	RECT rcDesktop;
	GetWindowRect(GetDesktopWindow(), &rcDesktop);
	SetWindowPos(m_hWnd, 0, (rcDesktop.right - rcDesktop.left - m_Width - diff_w) / 2, (rcDesktop.bottom - rcDesktop.top - m_Height - diff_h) / 2,
		m_Width + diff_w, m_Height + diff_h, SWP_NOCOPYBITS | SWP_NOOWNERZORDER | SWP_NOZORDER);

	ShowWindow(m_hWnd, SW_SHOW);
}

void Win32::Loop(uint32 fps)
{
	MSG msg;

	LARGE_INTEGER nFreq;
	LARGE_INTEGER nLast;
	LARGE_INTEGER nNow;
	LARGE_INTEGER nDelta;
	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nLast);
	if (fps != 0)
		nDelta.QuadPart = (LONGLONG)(1.0 / fps * nFreq.QuadPart);

	m_LastCountFpsTime = g_TimeManager->GetCurTime();
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			Sleep(0);
		}
		else
		{
			if (fps)
			{
				QueryPerformanceCounter(&nNow);
				if (nNow.QuadPart - nLast.QuadPart >= nDelta.QuadPart)
				{
					nLast.QuadPart = nNow.QuadPart;
					this->Update();
				}
			}
 			else
				this->Update();
		}
	}
}

void Win32::Update()
{
	CaleFps();

	g_TimeManager->Update();
	g_RenderManager->Render();

	ReleasePool::ClearAllObject();
}

void Win32::InitGame()
{
	g_Win32 = new Win32();
	g_TimeManager = new TimeManager();
	g_RenderManager = new RenderManager();
}

void Win32::QuitGame()
{
	delete g_Win32;
	g_Win32 = nullptr;
}

bool Win32::GameStart()
{
	//启动GL
	if (!g_RenderManager->InitGL())
		return false;
	g_RenderManager->SetViewPort(0, 0, m_Width, m_Height);

	return true;
}

void Win32::CaleFps()
{
	m_FpsCount++;
	int32 delta = int32(g_TimeManager->GetCurTime() - m_LastCountFpsTime);
	if (delta >= 1000)
	{
		m_Fps = m_FpsCount * 1000.f / delta;
		m_FpsCount = 0;
		m_LastCountFpsTime = g_TimeManager->GetCurTime();

		static char szTitle[256];
		memset(szTitle, 0, 256);
		sprintf(szTitle, "%s    (Qimp2d - fps:%.1f)", m_Title.c_str(), m_Fps);
		this->SetTitle(szTitle);
	}
}

void Win32::SetTitle(const char* title)
{
	WCHAR wszBuf[255] = { 0 };
	MultiByteToWideChar(CP_UTF8, 0, title, -1, wszBuf, sizeof(wszBuf));
	SetWindowText(m_hWnd, &wszBuf[0]);
}

QIMP_END_NAMESPACE