/*
 *  Wave.cpp
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#include <Audio.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Audio)

//////////////////////////////////////////////////////////////////////////

  Wave::Wave()
	{
    mSource = NULL;    
    mPriority = 0.0f;
    mDuration = 0.0f;
    mVolume = 1.0f;
    mLooping = FALSE;
    mSpatialization = FALSE;
    mPitch = 1.0f;
    mPlaybackTime = 0.0f;
	}

	Wave::~Wave() 
	{
    
	}

  //////////////////////////////////////////////////////////////////////////

  /*
	====================
	Buffer
	====================
	*/
	VOID Wave::Buffer(const AudioBuffer* buffer)
	{
		mBuffer = (AudioBuffer*)buffer;
	}

	const AudioBuffer* Wave::Buffer() const
	{
		return mBuffer.Ptr();
	}

  /*
	====================
	Priority
	====================
	*/
  VOID Wave::Priority(F32 p)
  {
    mPriority = p;
  }

  F32 Wave::Priority() const
  {
    return mPriority;
  }

  /*
	====================
	Duration
	====================
	*/
  VOID Wave::Duration(F32 d)
  {
    mDuration = d;
  }

  F32 Wave::Duration() const
  {
    return mDuration;
  }

  /*
	====================
	Volume
	====================
	*/
  VOID Wave::Volume(F32 v)
  {
    mVolume = v;
  }

  F32 Wave::Volume() const
  {
    return mVolume;
  }

  /*
	====================
	Looping
	====================
	*/
  VOID Wave::Looping(BOOL b)
  {
    mLooping = b;
  }

  BOOL Wave::Looping() const
  {
    return mLooping;
  }

  /*
	====================
	Spatialization
	====================
	*/
  VOID Wave::Spatialization(BOOL b)
  {
    mSpatialization = b;
  }

  BOOL Wave::Spatialization() const
  {
    return mSpatialization;
  }

  /*
	====================
	Pitch
	====================
	*/
  VOID Wave::Pitch(F32 p)
  {
    mPitch = p;
  }

  F32 Wave::Pitch() const
  {
    return mPitch;
  }

  /*
	====================
	Velocity
	====================
	*/
  VOID Wave::Velocity(Vector3& v)
  {
    mVelocity = v;
  }

  const Vector3& Wave::Velocity() const
  {
    return mVelocity;
  }

  /*
	====================
	Location
	====================
	*/
  VOID Wave::Location(Vector3& v)
  {
    mLocation = v;
  }

  const Vector3& Wave::Location() const
  {
    return mLocation;
  }

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
