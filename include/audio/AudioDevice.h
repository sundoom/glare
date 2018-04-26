/*
 *  AudioDevice.h
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __AudioDevice__
#define __AudioDevice__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Audio)

//////////////////////////////////////////////////////////////////////////

  // this class provides an object-oriented AudioDevice interface.
  class AudioDevice
  {
  public:
    // constructor
    AudioDevice();

    // destructor
    ~AudioDevice();

  public:   
    // update the audio device.
    VOID Update(F32 delta_time);

    // set the listener
    VOID Listen(const Vector3& location, const Vector3& orientation);

    // clean the audio device.
    VOID Clean();

    // play the wave
    VOID Play(Wave* wave);

    // stops the wave
    VOID Stop(Wave* wave);

  private:
    ALCdevice* mDevice;
    ALCcontext*	mContext;
    AudioSource* mChannels;
    std::list<AudioSource*>mFrees;
    std::list<WavePtr>mWaves;
    Vector3 mLocation, mOrientation;
  };
 
//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __AudioDevice__

//////////////////////////////////////////////////////////////////////////