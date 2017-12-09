#include "common/Qimp.h"
#include "common/ReleasePool.h"

QIMP_BEGIN_NAMESPACE

Object::Object() :m_uRefCount(0)
{

}

Object::~Object()
{

}

void Object::AddRef(const char* tips)
{
	m_uRefCount++;
}

void Object::Release(const char* tips)
{
	m_uRefCount--;

	QIMP_ASSERT(m_uRefCount >= 0, "Ref :0, Object have bend deleted");
	if (m_uRefCount == 0)
		delete this;
}

void Object::AutoRelease()
{
	ReleasePool::AddObject(this);
}


QIMP_END_NAMESPACE
