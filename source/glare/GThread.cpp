/*
 *  GThread.cpp
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

#include <process.h>

//////////////////////////////////////////////////////////////////////////

GThread::GThread() 
{
	mHandle = NULL;
  mRunning = FALSE;
}

GThread::~GThread() 
{
	
}

/*
====================
Start
====================
*/
VOID GThread::Start(U32 priority)
{
  mRunning = TRUE;

	// create the thread
	U32 id;
	mHandle = reinterpret_cast<HANDLE>(::_beginthreadex(0, 0, Hook, this, CREATE_SUSPENDED, &id));
	if(mHandle == 0)
	{
		GAssert(VA("Fail to create a new thread and the error code is %d!", ::GetLastError()));
	}

	// set the thread priority
	I32 prio = THREAD_PRIORITY_NORMAL;
	switch(priority) 
	{
	case THREAD_PRIORITY_MAX:
		prio = THREAD_PRIORITY_HIGHEST;
		break;
	case THREAD_PRIORITY_HIGH:
		prio = THREAD_PRIORITY_ABOVE_NORMAL;
		break;
	case THREAD_PRIORITY_NOMINAL:
		prio = THREAD_PRIORITY_NORMAL;
		break;
	case THREAD_PRIORITY_LOW:
		prio = THREAD_PRIORITY_BELOW_NORMAL;
		break;
	case THREAD_PRIORITY_MIN:
		prio = THREAD_PRIORITY_IDLE;
		break;
	}
	if(::SetThreadPriority(mHandle, prio) == 0)
	{
		GAssert(VA("Fail to set the thread priority and the error code is %d!", ::GetLastError()));
	}

	// resume the thread
	if(::ResumeThread(mHandle) == -1)
	{
		GAssert(VA("Fail to resume the thread and the error code is %d!", ::GetLastError()));
	}
}

/*
====================
End
====================
*/
VOID GThread::End()
{
  // terminate the thread and close the thread handle
	if(mHandle)
	{
    mRunning = FALSE;

		// wait until the controlled thread terminates.
		if(::WaitForSingleObject(mHandle, INFINITE) != WAIT_OBJECT_0)
		{
			GAssert( VA("Fail to terminate the thread and the error code is %d!", ::GetLastError()) );
		}

		// close the thread handle
		if(::CloseHandle(mHandle) == 0)
		{
			GAssert( VA("Fail to close the thread handle and the error code is %d!", ::GetLastError()) );
		}

		mHandle = NULL;
	}
}

/*
====================
Hook
====================
*/
U32 __stdcall GThread::Hook(VOID * arg) 
{
	try
	{
		// the thread running
		static_cast<GThread*>(arg)->Run();
	}
	catch (...)
	{
		GAssert("The thread run is error!");
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////

#else

//////////////////////////////////////////////////////////////////////////

#include <pthread.h>

//////////////////////////////////////////////////////////////////////////

GThread::GThread() 
{
  mHandle = NULL;
  mRunning = FALSE;
}

GThread::~GThread() 
{

}

/*
====================
Start
====================
*/
VOID GThread::Start(U32 priority)
{
  mRunning = TRUE;
  CHECK(pthread_create(&mHandle, 0, Hook, this) == 0);
  CHECK(pthread_detach(mHandle)==0);

  // set the thread priority
  I32 policy; struct sched_param param;
  CHECK(pthread_getschedparam(mHandle, &policy, &param) != 0);
  policy = SCHED_RR;
  I32 min_priority = sched_get_priority_min(policy);
  I32 max_priority = sched_get_priority_max(policy);
  I32 nominal_priority = (max_priority + min_priority)/2;

  // set the thread priority
  switch(priority) 
  {
  case THREAD_PRIORITY_MAX:
    param.sched_priority = max_priority;
    break;
  case THREAD_PRIORITY_HIGH:
    param.sched_priority = (max_priority + nominal_priority)/2;
    break;
  case THREAD_PRIORITY_NOMINAL:
    param.sched_priority = nominal_priority;
    break;
  case THREAD_PRIORITY_LOW:
    param.sched_priority = (min_priority + nominal_priority)/2;
    break;
  case THREAD_PRIORITY_MIN:
    param.sched_priority = min_priority;
    break;
  }
  CHECK(pthread_setschedparam(mHandle, policy, &param) == 0);
}

/*
====================
End
====================
*/
VOID GThread::End()
{
  // terminate the thread
  if(mHandle)
  {
    mRunning = FALSE;

    // cancel the thread handle
    CHECK(pthread_cancel(mHandle)==0);
    mHandle = NULL;
  }
}

/*
====================
Hook
====================
*/
VOID* GThread::Hook(VOID * arg) 
{
  try
  {
    // the thread running
    static_cast<GThread*>(arg)->Run();
  }
  catch (...)
  {
    GAssert("The thread run is error!");
  }

  return 0;
}

//////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////