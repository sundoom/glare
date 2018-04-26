/*
 *  Glare.cpp
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#include <Glare.h>

//////////////////////////////////////////////////////////////////////////
/*
====================
GSeconds

  Returns time in seconds. Origin is arbitrary.
====================
*/
#ifdef _WIN32
  static F64 InitTiming()
  {
    LARGE_INTEGER Frequency;
    CHECK(QueryPerformanceFrequency(&Frequency));  
    return 1.0 / Frequency.QuadPart;
  }
  F64 GSeconds()
  {
    static F64 s_seconds_per_cycle = InitTiming();
	  LARGE_INTEGER cycles;
	  QueryPerformanceCounter(&cycles);
	  return cycles.QuadPart * s_seconds_per_cycle + 16777216.0;
  }
#else
  F64 GSeconds()
  {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((F64)tv.tv_sec) + (((F64)tv.tv_usec)/1000000.0);
  }
#endif

/*
====================
GSleep
====================
*/
VOID GSleep(F32 seconds)
{
#ifdef _WIN32
  ::Sleep((U32)(seconds*1000.0f));
#else
  usleep((I32)(seconds*1000000.0f));
#endif
}

//////////////////////////////////////////////////////////////////////////

/*
====================
VA
====================
*/
const CHAR* VA(const CHAR *format, ...)
{
	va_list	argptr;
	static CHAR	string[32000];	// in case va is called by nested functions

	va_start (argptr, format);
	vsnprintf(string,sizeof(string), format,argptr);
	va_end (argptr);

	return(string);
}

/*
====================
GAssert
====================
*/
VOID GAssert(const CHAR * msg)
{
#ifdef WIN32
	::MessageBox(NULL, msg, "Assert", MB_OK|MB_ICONERROR|MB_TASKMODAL);
#else
	GPrint(msg);
#endif
}

/*
====================
GAssert
====================
*/
VOID GAssert(const CHAR * msg, const CHAR * filename, I32 line)
{
	GAssert(VA("Assertion failed: %s [File:%s] [Line: %i]", msg, filename, line));
}

/*
====================
GPrint
====================
*/
VOID GPrint(const CHAR * msg)
{
	::printf("%s", msg);
}

//////////////////////////////////////////////////////////////////////////

/*
====================
GMalloc
====================
*/
VOID* GMalloc(U32 size, const CHAR * filename, I32 line)
{
	return ::malloc(size);	
}

/*
====================
GFree
====================
*/
VOID GFree(VOID* p)
{
	::free(p);
}

//////////////////////////////////////////////////////////////////////////
/*
====================
new and delete
====================
*/
#ifdef GLARE_MEMORY
VOID* operator new(U32 cb)
{ return GMalloc( cb, NULL, 0 ); }
VOID* operator new(U32 cb, const CHAR* file, I32 line)
{ return GMalloc( cb, file, line ); }
VOID* operator new[](U32 cb) 
{ return GMalloc( cb, NULL, 0 ); }
VOID* operator new[](U32 cb, const CHAR* file, I32 line)
{ return GMalloc( cb, file, line ); }
VOID operator delete(VOID *p)
{ GFree(p); }
VOID operator delete[](VOID *p)
{ GFree(p);}
#endif

//////////////////////////////////////////////////////////////////////////

/*
====================
GParse
====================
*/
std::vector<Str> GParse(const Str& in, CHAR split)
{
	std::vector<Str>strcol;

	if( in.empty() ) return strcol;

	Str	str;

	const CHAR*	token = in.c_str();

	while( 1 ) {
		/* skip whitespace */
		while( *token && *token <= ' ' || *token == split )
		{
			token++;
		}

		if( !*token ) break;

		str.resize(0);

		// handle quoted strings
		if( *token == '\"' ) 
		{
			token++;
			while( *token && *token != '\"' ) 
			{
				str += *token++;
			}
			strcol.push_back( str );
			str.resize(0);
			if( !*token ) 
			{
				break;
			} 
			else 
			{
				token++;
				continue;
			}
		}

		do 
		{
			str+=*token; token++;
		} while( *token > ' ' && *token != split );
		strcol.push_back( str );
	}

	return strcol;
}

//////////////////////////////////////////////////////////////////////////