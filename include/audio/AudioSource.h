/*
 *  AudioSource.h
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __AudioSource__
#define __AudioSource__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Audio)

//////////////////////////////////////////////////////////////////////////

	// this class provides an object-oriented AudioSource interface.
	class AudioSource
	{
  public:
		// constructor
		AudioSource();
		
		// destructor
		virtual ~AudioSource();

    // bind the wave
    VOID Bind(const Wave* wave);

    // update the data
    VOID Update();

    // play the current wave instance.
    VOID Play();

	  // stops the current wave instance and detaches it from the source.	
	  VOID Stop();
	
    // pauses playback of current wave instance.
	  VOID Pause();

		// queries the status of the currently associated wave
		BOOL IsFinished();

  private:
    ALuint mObject;
    BOOL mPlaying, mPaused;
    WavePtr mWavePtr;
  
  private:friend class AudioDevice;
	}; 

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __AudioSource__

//////////////////////////////////////////////////////////////////////////