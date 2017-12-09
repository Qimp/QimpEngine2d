#ifndef __RENDERMANAGER__H__
#define __RENDERMANAGER__H__

QIMP_BEGIN_NAMESPACE

class RenderManager
{
private:

	Rect							m_Viewport;

public:
	bool							InitGL();
	void							SetViewPort(uint32 x, uint32 y, uint32 w, uint32 h);
	void							Render();
};


QIMP_END_NAMESPACE
#endif