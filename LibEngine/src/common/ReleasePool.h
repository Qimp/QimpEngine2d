#ifndef __RELEASEPOOL__H__
#define __RELEASEPOOL__H__

QIMP_BEGIN_NAMESPACE

class ReleasePool
{
public:
	static void							AddObject(Object* p);			// 把对象加到池里管理，下一个逻辑帧会减一个引用计数
	static void							RemoveObject(Object* p);
	static void							ClearAllObject();
};


QIMP_END_NAMESPACE
#endif