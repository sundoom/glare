
/*
 *  GThread.h
 *  glare
 *
 *  Created by sunjun on 11-6-10.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __GThread__
#define __GThread__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

#include <Glare.h>

//////////////////////////////////////////////////////////////////////////

// this class provides an object-oriented thread interface.
class GThread
{
public:
  // enumerated type for thread priority
  enum PriorityType 
  {
    THREAD_PRIORITY_MAX,      // The maximum possible priority
    THREAD_PRIORITY_HIGH,     // A high (but not max) setting
    THREAD_PRIORITY_NOMINAL,  // An average priority
    THREAD_PRIORITY_LOW,      // A low (but not min) setting
    THREAD_PRIORITY_MIN,      // The miniumum possible priority
    THREAD_PRIORITY_DEFAULT   // Priority scheduling default
  };

public:		
  // constructor
  GThread();

  // destructor
  virtual ~GThread();

  // start the thread
  VOID Start(U32 priority = THREAD_PRIORITY_NOMINAL);

  // end the thread
  VOID End();

protected:
  // thread's run method.  must be implemented by derived classes.
  // this is where the action happens.
  virtual VOID Run() = 0;

private:
  // the thread hook
#ifdef _WIN32
  static U32 __stdcall Hook(VOID* arg);
#else
	static VOID* Hook(VOID* arg);
#endif

protected:
  BOOL mRunning;

private:
#ifdef _WIN32
  HANDLE	mHandle;
#else
  pthread_t mHandle;
#endif
};

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __GThread__

//////////////////////////////////////////////////////////////////////////