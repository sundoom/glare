/*
 *  Audio.h
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __Audio__
#define __Audio__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

#include <Glare.h>
#include <GMath.h>
NAMESPACE_USE(Math)

//////////////////////////////////////////////////////////////////////////

#ifdef _WIN32	
  #include <al.h>
  #include <alc.h>
  #pragma comment(lib,"OpenAL32.lib")
#elif __APPLE__
  #include <OpenAL/al.h>
  #include <OpenAL/alc.h>
#else
  #error Unknown platform!
#endif

//////////////////////////////////////////////////////////////////////////

#include <Wave.h>
#include <AudioBuffer.h>
#include <AudioSource.h>
#include <AudioDevice.h>

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Audio)

//////////////////////////////////////////////////////////////////////////
// Audio Interface
//////////////////////////////////////////////////////////////////////////
 
  // Length of sound in seconds to be considered as looping forever
  #define INDEFINITELY_LOOPING_DURATION	10000.0f
 
  // query the instance of the audio
  extern AudioDevice& instance();

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __Audio__

//////////////////////////////////////////////////////////////////////////