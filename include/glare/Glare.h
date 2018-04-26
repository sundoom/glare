/*
 *  Glare.h
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#ifndef __Glare__
#define __Glare__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <setjmp.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

#ifdef __APPLE__
#include <unistd.h>
#include <dlfcn.h>
#include <dirent.h>
#include <sys/time.h>
#endif

#ifndef __GCN__
#include <sys/types.h>
#include <sys/stat.h>
#endif

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include <io.h>
#endif

//////////////////////////////////////////////////////////////////////////

#define NAMESPACE_BEGIN(name)	namespace name {
#define NAMESPACE_END			}
#define NAMESPACE_USE(name)		using namespace name;

//////////////////////////////////////////////////////////////////////////

#undef CHAR
#undef VOID
#undef HANDLE

#ifdef _WIN32
	typedef __int64				I64;
	typedef signed int			I32;
	typedef signed short		I16;
	typedef signed char			I8;
	
	typedef unsigned __int64	U64;
	typedef unsigned int		U32;
	typedef unsigned short		U16;
	typedef unsigned char		U8;
	
	typedef float				F32;
	typedef double				F64;

	typedef char				CHAR;
	typedef wchar_t				WCHAR;

	typedef void				VOID;
	typedef void*				HANDLE;
	typedef signed int			BOOL;		
#elif LINUX
	typedef long long			I64;
	typedef signed int			I32;
	typedef signed short		I16;
	typedef signed char			I8;

	typedef unsigned long long	U64;
	typedef unsigned int		U32;
	typedef unsigned short		U16;
	typedef unsigned char		U8;

	typedef float				F32;
	typedef double				F64;

	typedef char				CHAR;
	typedef wchar_t				WCHAR;

	typedef void				VOID;
	typedef void*				HANDLE;
	typedef signed int			BOOL;
#elif __APPLE__
	typedef long long			I64;
	typedef signed int			I32;
	typedef signed short		I16;
	typedef signed char			I8;

	typedef unsigned long long	U64;
	typedef unsigned int		U32;
	typedef unsigned short		U16;
	typedef unsigned char		U8;

	typedef float				F32;
	typedef double				F64;

	typedef char				CHAR;
	typedef wchar_t				WCHAR;

	typedef void				VOID;
	typedef void*				HANDLE;
	typedef signed char			BOOL;
#elif _XBOX
	typedef __int64				I64;
	typedef signed int			I32;
	typedef signed short		I16;
	typedef signed char			I8;

	typedef unsigned __int64	U64;
	typedef unsigned int		U32;
	typedef unsigned short		U16;
	typedef unsigned char		U8;

	typedef float				F32;
	typedef double				F64;

	typedef char				CHAR;
	typedef wchar_t				WCHAR;

	typedef void				VOID;
	typedef void*				HANDLE;
	typedef signed int			BOOL;
#else
	#error Unknown platform!
#endif

#ifndef NULL
	#ifdef __cplusplus
		#define NULL    0
	#else
		#define NULL    ((void *)0)
	#endif
#endif

#ifndef FALSE
	#define FALSE       0
#endif

#ifndef TRUE
	#define TRUE		1
#endif

//////////////////////////////////////////////////////////////////////////

#define	MAX_I8			0x7f
#define	MIN_I8			0x80
#define	MAX_U8			0xff
#define	MIN_U8			0x00
#define	MAX_I16			0x7fff
#define	MIN_I16			0x8000
#define	MAX_U16			0xffff
#define	MIN_U16			0x0000
#define	MAX_I32			0x7fffffff
#define	MIN_I32			0x80000000
#define	MAX_U32			0xffffffff
#define	MIN_U32			0x00000000
#define	MAX_I64			0x7fffffffffffffff
#define	MIN_I64			0x8000000000000000
#define	MAX_U64			0xffffffffffffffff
#define	MIN_U64			0x0000000000000000
#define	MAX_F32			3.402823466e+38F
#define	MIN_F32			(-3.402823466e+38F)
#define	MAX_F64			1.7976931348623158e+308
#define	MIN_F64			(-1.7976931348623158e+308)
#define EPS_F32			1.192092896e-07F			// smallest number not zero
#define EPS_F64			2.2204460492503131e-016		// smallest number not zero
#define IEEE_1_0		0x3f800000					// integer representation of 1.0
#define IEEE_255_0		0x437f0000					// integer representation of 255.0
#define IEEE_MAX_F32	0x7f7fffff					// integer representation of max float
#define IEEE_MIN_F32	0xff7fffff					// integer representation of min float
#define	N_PI			3.14159265358979323846
#ifndef MIN
#define	MIN(a, b)		((a) < (b) ? (a) : (b))		// returns the min value between a and b
#endif
#ifndef MAX
#define	MAX(a, b)		((a) > (b) ? (a) : (b))		// returns the max value between a and b
#endif

#ifndef MAX_PATH
  #define MAX_PATH 260
#endif

#ifndef MAX_NAME
  #define MAX_NAME 32
#endif

//////////////////////////////////////////////////////////////////////////

#ifndef MAKEFOURCC
  #define MAKEFOURCC(ch0, ch1, ch2, ch3) ((U32)(U8)(ch0)|((U32)(U8)(ch1)<<8)|((U32)(U8)(ch2)<<16)|((U32)(U8)(ch3)<<24))
#endif

//////////////////////////////////////////////////////////////////////////

extern F64 GSeconds();
extern VOID GSleep(F32 seconds);

//////////////////////////////////////////////////////////////////////////

extern const CHAR* VA(const CHAR *format, ...);
extern VOID GAssert(const CHAR * msg);
extern VOID GAssert(const CHAR * msg, const CHAR * filename, I32 line);
extern VOID GPrint(const CHAR * msg);

//////////////////////////////////////////////////////////////////////////

#ifndef DO_CHECK
	#define DO_CHECK 1
#endif

#if DO_CHECK
	#define CHECK(expr)	{if(!(expr))GAssert( #expr, __FILE__, __LINE__ );}
#else
	#define CHECK(expr)	if(expr){}
#endif	

//////////////////////////////////////////////////////////////////////////

extern VOID* GMalloc(U32 size, const CHAR * filename, I32 line);
extern VOID GFree(VOID* p);

//////////////////////////////////////////////////////////////////////////

#ifdef GLARE_MEMORY
	VOID* operator new(U32 cb);
	VOID* operator new(U32 cb, const CHAR* file, I32 line);
	VOID* operator new[](U32 cb);
	VOID* operator new[](U32 cb, const CHAR* file, I32 line);
	VOID operator delete(VOID *p);
	VOID operator delete[](VOID *p);
	#define GNEW(x)		new(__FILE__, __LINE__) x
	#define GDELETE(x)	delete x
#else
	#define GNEW(x)		new x
	#define GDELETE(x)	delete x
#endif

//////////////////////////////////////////////////////////////////////////

#include <vector>
#include <map>
#include <set>
#include <list>
#include <string>
#include <algorithm>
#include <functional>
typedef std::string	Str;
typedef std::wstring WStr;

//////////////////////////////////////////////////////////////////////////

extern std::vector<Str> GParse(const Str& in, CHAR split = ' ');

//////////////////////////////////////////////////////////////////////////

#include <GPtr.h>
#include <GReference.h>
#include <GData.h>
#include <GConfig.h>
#include <GFile.h>
#include <GThread.h>
#include <GMutex.h>

//////////////////////////////////////////////////////////////////////////

extern const GData* GLoad(const CHAR * path);

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __Glare__

//////////////////////////////////////////////////////////////////////////
