#include "Private.h"

QIMP_BEGIN_NAMESPACE
#if 0 && DEBUGMODE && defined(__SSE__)

void InitFloatExceptions ()
{
	  _MM_SET_EXCEPTION_MASK (_MM_GET_EXCEPTION_MASK () & ~_MM_MASK_INVALID);
}

#else

void InitFloatExceptions ()
{
}

#endif
QIMP_END_NAMESPACE