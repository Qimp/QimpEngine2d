#include "common/Qimp.h"
#include "common/ReleasePool.h"

QIMP_BEGIN_NAMESPACE


static umap<Object*, bool>	s_mapAllObject;


void ReleasePool::AddObject(Object* p)
{
	umap<Object*, bool>::iterator itr = s_mapAllObject.find(p);
	if (itr != s_mapAllObject.end())
	{
		return;
	}
	p->AddRef("AutoRelase");
	s_mapAllObject[p] = true;
}

void ReleasePool::RemoveObject(Object* p)
{
	umap<Object*, bool>::iterator itr = s_mapAllObject.find(p);
	if (itr != s_mapAllObject.end())
	{
		s_mapAllObject.erase(itr);
	}
}

void ReleasePool::ClearAllObject()
{
	Object* p;
	umap<Object*, bool>::iterator itr = s_mapAllObject.begin();
	for (; itr != s_mapAllObject.end();)
	{
		p = itr->first;
		itr = s_mapAllObject.erase(itr);
		p->Release("AutoRelase");
	}
}

QIMP_END_NAMESPACE