#ifndef __MATH_TINY_VECTOR_H__
#define __MATH_TINY_VECTOR_H__

QIMP_BEGIN_NAMESPACE

Vector3f	Int2Vec(uint32 value, float f);
Quaternionf Int2Quat(uint32 value);

uint32		Vec2Int(Vector3f value);
uint32		Quat2Int(Quaternionf value);

template<typename T>
T TinyDecode(uint32 value, float f);



QIMP_END_NAMESPACE

#endif//__MATH_TINY_VECTOR_H__
