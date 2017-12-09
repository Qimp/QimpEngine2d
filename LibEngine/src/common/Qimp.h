#ifndef __QIMP__H__
#define __QIMP__H__

#include <WinSock2.h>
#include <Iphlpapi.h>
#include <tchar.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <assert.h>
#include <windowsx.h>
#include <shellapi.h>

#include <glew/glew.h>
#include <glew/wglew.h>

#include "pthread/pthread.h"

#include "gl/GL.h"
#include <mmsystem.h>

#include <psapi.h>

#pragma comment(lib,"psapi.lib")
#pragma comment( lib, "ws2_32.lib" )
#pragma comment( lib, "shell32.lib" )


#define Assert(f) assert(f)
#define AssertIf(f) assert(!(f))

typedef unsigned char uint8;
typedef signed char int8;
typedef unsigned short uint16;
typedef signed short int16;
typedef unsigned int uint32;
typedef signed int int32;
typedef unsigned long long uint64;
typedef signed long long int64;


#include <algorithm>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_map>
#include <bitset>
#include <string>
#include <cmath>
#include <float.h>
#include <math.h>
#include <sstream>
#include <atomic>

#define QIMP_BEGIN_NAMESPACE	namespace Qimp {
#define QIMP_END_NAMESPACE	}
#define QIMP_USING_NAMESPACE	using namespace Qimp;
#define QIMP_LOG(fmt, ...) Qimp::LogByLevel(Qimp::kLogLevelNormal, fmt, ##__VA_ARGS__)
#define QIMP_LOGL(e, fmt, ...) Qimp::LogByLevel(e, fmt, ##__VA_ARGS__)
#define QIMP_ASSERT(b, fmt, ...) {																\
	if (!(b)){																					\
		Qimp::LogByLevel(Qimp::kLogLevelError, "Qimp_ASSERT in %s at %d", __FILE__, __LINE__);	\
		Qimp::LogByLevel(Qimp::kLogLevelError, fmt, ##__VA_ARGS__);								\
		assert(0);																				\
			}}

using namespace std;


#define DECLARE_ENUM(type)	\
	type::Type m_Type; \
	inline type() {}	\
	inline type(Type e) { m_Type = e; } \
	inline type(const type &i) { m_Type = i.m_Type; } \
	inline operator Type() const { return m_Type; }

template <typename T>
struct remove_const_reference
{
	typedef typename remove_const<typename remove_reference<T>::type>::type type;
};

#define umap std::unordered_map

#define FORVEC_START(v, c, n) {												\
	size_t _##n##Size = v.size();											\
	if (_##n##Size > 0)														\
	for (size_t _##n##Count = 0; _##n##Count < _##n##Size; _##n##Count++){	\
		c n = v[_##n##Count];

#define FORVEC_START_REVERSE(v, c, n) {										\
	size_t _##n##Size = v.size();											\
	if (_##n##Size > 0)														\
	for (size_t _##n##Count = _##n##Size; _##n##Count > 0; _##n##Count--){	\
		c n = v[_##n##Count - 1];

#define FORVEC_END() }}

#include "Log.h"
#include "common/Global.h"
#include "math/MathPublic.h"
#include "common/Object.h"
#include "render/TexFormat.h"

#endif