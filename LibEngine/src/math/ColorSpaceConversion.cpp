#include "Private.h"

QIMP_BEGIN_NAMESPACE
	
	ColorSpace gActiveColorSpace = kUninitializedColorSpace;

void SetActiveColorSpace (ColorSpace space)
{
	gActiveColorSpace = space;
}

ColorSpace GetActiveColorSpace ()
{
	return gActiveColorSpace;
}

QIMP_END_NAMESPACE