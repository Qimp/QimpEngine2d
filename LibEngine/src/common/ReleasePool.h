#ifndef __RELEASEPOOL__H__
#define __RELEASEPOOL__H__

QIMP_BEGIN_NAMESPACE

class ReleasePool
{
public:
	static void							AddObject(Object* p);			// �Ѷ���ӵ����������һ���߼�֡���һ�����ü���
	static void							RemoveObject(Object* p);
	static void							ClearAllObject();
};


QIMP_END_NAMESPACE
#endif