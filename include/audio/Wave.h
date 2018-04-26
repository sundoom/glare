/*
 *  Wave.h
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __Wave__
#define __Wave__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Audio)

//////////////////////////////////////////////////////////////////////////

  class AudioBuffer;
  class AudioSource;

  //////////////////////////////////////////////////////////////////////////

	// this class provides an object-oriented Wave interface.
	class Wave : public GReference
	{
  public:
		// constructor
		Wave();
		
		// destructor
		virtual ~Wave();

    // set/get the audio buffer
    VOID Buffer(const AudioBuffer* buffer);
    const AudioBuffer* Buffer() const;

    // set/get the priority
    VOID Priority(F32 p);
    F32 Priority() const;

    // set/get the duration
    VOID Duration(F32 d);
    F32 Duration() const;  

    // set/get the volume
    VOID Volume(F32 v);
    F32 Volume() const;

    // set/get the looping
    VOID Looping(BOOL b);
    BOOL Looping() const;

    // set/get the spatialization
    VOID Spatialization(BOOL b);
    BOOL Spatialization() const;

    // set/get the pitch
    VOID Pitch(F32 p);
    F32 Pitch() const;

    // set/get the velocity
    VOID Velocity(Vector3& v);
    const Vector3& Velocity() const;

    // set/get the location
    VOID Location(Vector3& v);
    const Vector3& Location() const;   

  private:
    GPtr<AudioBuffer>mBuffer;
    AudioSource* mSource;
    F32   mPriority;
    F32   mDuration;
    F32   mVolume;
    BOOL  mLooping;    
    BOOL  mSpatialization;
    F32	  mPitch;    
    Vector3 mVelocity; // unit : meters.
    Vector3	mLocation; // unit : meters.
    F32   mPlaybackTime;

  private:friend class AudioDevice;
	};

  //////////////////////////////////////////////////////////////////////////

  typedef GPtr<Wave>WavePtr;

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __Wave__

//////////////////////////////////////////////////////////////////////////