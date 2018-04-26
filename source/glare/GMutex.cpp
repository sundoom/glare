/*
 *  GMutex.cpp
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#include <Glare.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

#ifdef _WIN32

//////////////////////////////////////////////////////////////////////////

GMutex::GMutex() 
{
  CRITICAL_SECTION * cs = GNEW(CRITICAL_SECTION);
  mData = static_cast<VOID*>(cs);
  ::InitializeCriticalSection(cs);
}

GMutex::~GMutex() 
{
  CRITICAL_SECTION* cs = static_cast<CRITICAL_SECTION*>(mData);
  ::DeleteCriticalSection(cs);
  GDELETE(cs);
}

/*
====================
Lock
====================
*/
VOID GMutex::Lock() const
{
  // block until we can take this lock.
  :: EnterCriticalSection(static_cast<CRITICAL_SECTION*>(mData));
}

/*
====================
Unlock
====================
*/
VOID GMutex::Unlock() const
{
  // release this lock. CRITICAL_SECTION is nested, thus
  // unlock() calls must be paired with lock() calls.
  ::LeaveCriticalSection(static_cast<CRITICAL_SECTION*>(mData));
}

//////////////////////////////////////////////////////////////////////////

#else

//////////////////////////////////////////////////////////////////////////

GMutex::GMutex() 
{
  pthread_mutexattr_t atts;
  pthread_mutexattr_init(&atts);
  pthread_mutexattr_settype(&atts, PTHREAD_MUTEX_RECURSIVE);
  pthread_mutex_init(&mData, &atts);
}

GMutex::~GMutex() 
{
  pthread_mutex_destroy(&mData);
}

/*
====================
Lock
====================
*/
VOID GMutex::Lock() const
{
  // block until we can take this lock.
  pthread_mutex_lock(&mData);
}

/*
====================
Unlock
====================
*/
VOID GMutex::Unlock() const
{
  // unlock() calls must be paired with lock() calls.
  pthread_mutex_unlock(&mData);
}

//////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////

GLock::GLock(const GMutex* m) : mMutex(m) 
{
  if(mMutex) mMutex->Lock();
}

GLock::~GLock()
{
  if(mMutex) mMutex->Unlock();
}

//////////////////////////////////////////////////////////////////////////