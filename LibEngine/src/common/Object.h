#ifndef __OBJECT__H__
#define __OBJECT__H__


QIMP_BEGIN_NAMESPACE


#define DEFINE_CLASS(cls)	\
public:						\
	virtual const char*				GetClassName(){ return #cls; };

#define SAFE_DELETE(p)				if(p != NULL) { delete p; p = NULL; }
#define SAFE_ADDREF(p, s, ...)		if(p != NULL) { p->AddRef(s, ##__VA_ARGS__ ); }
#define SAFE_RELEASE(p, s, ...)		if(p != NULL) { p->Release(s, ##__VA_ARGS__ ); p = NULL; }
#define SAFE_AUTORELEASE(p, s, ...)	if(p != NULL) { p->AutoRelease(s, ##__VA_ARGS__ ); p = NULL; }


class Object
{
	DEFINE_CLASS(Object);

protected:
	uint32							m_uRefCount;

public:
	Object();
	~Object();

	void							AddRef(const char * tips = nullptr);
	void							Release(const char * tips = nullptr);
	void							AutoRelease();
};




QIMP_END_NAMESPACE



#endif