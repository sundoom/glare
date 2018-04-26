/*
 *  GMutex.h
 *  glare
 *
 *  Created by sunjun on 11-6-10.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#ifndef __GMutex__
#define __GMutex__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

#define AUTO_LOCK_MUTEX(o)		class GLock __lock_##__LINE__(o);	

//////////////////////////////////////////////////////////////////////////

// this class provides an object-oriented thread mutex interface.
class GMutex
{
public:		
  // constructor
  GMutex();

  // destructor
  ~GMutex();

  // lock the mutex
  // @return 0 if normal, -1 if errno set, errno code otherwise.		
  VOID Lock() const;

  // unlock the mutex
  // @return 0 if normal, -1 if errno set, errno code otherwise.
  VOID Unlock() const;

private:
  // implementation-specific private data.
#ifdef _WIN32
  VOID* mData;
#else
  mutable pthread_mutex_t mData;
#endif
};

//////////////////////////////////////////////////////////////////////////

// the auto lock class
class GLock
{
public:
  explicit GLock(const GMutex* m);

  ~GLock();

private:
  const GMutex* mMutex;
};

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __GMutex__

//////////////////////////////////////////////////////////////////////////