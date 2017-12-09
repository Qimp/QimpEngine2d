#ifndef __WIN32__H__ 
#define __WIN32__H__

QIMP_BEGIN_NAMESPACE


class Win32
{
public:
	Win32();

	static void					InitGame();
	static void					QuitGame();
	bool						InitWin(const char* title, int32 w, int32 h);
	void						SetTitle(const char* title);
	bool						GameStart();
	void						AutoSetWinPos();
	void						Show();
	void						Loop(uint32 fps);
	void						Update();

	HWND						GetHWnd()											{ return m_hWnd; };
private:
	void						CreateWin();
	void						CaleFps();

private:
	HWND						m_hWnd;
	HDC							m_hDC;
	string						m_Title;
	int32						m_Width;
	int32						m_Height;
	int32						m_FpsCount;
	int64						m_LastCountFpsTime;
	float						m_Fps;

};



QIMP_END_NAMESPACE

#endif