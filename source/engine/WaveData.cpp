/*
 *  WaveData.cpp
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#include <Engine.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////

  #pragma pack (push,1)

  //////////////////////////////////////////////////////////////////////////

  // Main Wave header.
  struct WaveHeader
  {
    U32 ID;			            // Contains 'RIFF'
    U32	ChunkLen;           // Remaining length of the entire riff chunk (= file).
    U32	Magic;			        // Form type. Contains 'WAVE' for .wav files.
  };

  // General chunk header format.
  struct WaveChunk
  {
    U32	ChunkID;		        // General data chunk ID like 'data', or 'fmt '
    U32	ChunkLen;		        // Length of the rest of this chunk in bytes.
  };

  // ChunkID: 'fmt ' ("WaveFormatEx" structure )
  struct WaveFormatChunk
  {
    U16 FormatTag;          // Format type: 1 = PCM
    U16 Channels;           // Number of channels (i.e. mono, stereo...).
    U32 SamplesPerSec;      // Sample rate. 44100 or 22050 or 11025  Hz.
    U32 AvgBytesPerSec;     // For buffer estimation  = sample rate * BlockAlign.
    U16 BlockAlign;         // Block size of data = Channels times BYTES per sample.
    U16 BitsPerSample;      // Number of bits per sample of mono data.
    U16 Size;               // The count in bytes of the size of extra information (after cbSize).
  };

  // ChunkID: 'smpl'
  struct WaveSampleChunk
  {
    U32 Manufacturer;
    U32 Product;
    U32 SamplePeriod;
    U32 MIDIUnityNote;
    U32 MIDIPitchFraction;
    U32	SMPTEFormat;
    U32 SMPTEOffset;
    U32 SampleLoops;        // Number of tSampleLoop structures following this chunk
    U32 SamplerData;
  };

  struct WaveSampleLoop			// Immediately following cbSamplerData in the SMPL chunk.
  {
    U32	Identifier;
    U32	Type;
    U32	Start;			        // Startpoint of the loop in samples
    U32	End;				        // Endpoint of the loop in samples
    U32	Fraction;			      // Fractional sample adjustment
    U32	PlayCount;		      // Play count
  };

  //////////////////////////////////////////////////////////////////////////

  #pragma pack (pop)

  //////////////////////////////////////////////////////////////////////////

  WaveData::WaveData(const CHAR* name) : Resource(name)
	{
    mDuration = 0.0f;
	}

	WaveData::~WaveData() 
	{
    
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	Load
	====================
	*/
	VOID WaveData::Load(const GData* data)
	{
		CHECK(data);

    // get the data
    U32 data_pos = 0;
    const U8* data_ptr = (U8*)data->Ptr();

    // get the wave header and check it
    WaveHeader* header = (WaveHeader*)data->Ptr();
    data_pos += sizeof(WaveHeader);
    CHECK(header->Magic == MAKEFOURCC('W','A','V','E'));

    // the sample info
    U32 samples_per_sec = 0;
    U32 avg_bytes_per_sec = 0;
    U16 block_align = 0;
    U16 bits_per_sample = 0;
    U16 num_channels = 0;    
    U8* sample_data_ptr = NULL;
    U32 sample_data_size = 0;

    // get all of the chunk info.
    while(data_pos < data->Size())
    {
      // get the chunk info
      WaveChunk* chunk = (WaveChunk*)(data_ptr+data_pos);
      data_pos += sizeof(WaveChunk);

      // get the format chunk
      if(chunk->ChunkID == MAKEFOURCC('f','m','t',' '))
      {
        WaveFormatChunk* format_chunk = (WaveFormatChunk*)(data_ptr+data_pos);
        bits_per_sample = format_chunk->BitsPerSample;
        samples_per_sec = format_chunk->SamplesPerSec;
        avg_bytes_per_sec = format_chunk->AvgBytesPerSec;
        block_align = format_chunk->BlockAlign;
        num_channels = format_chunk->Channels;

        // check the bits and the number of the channels
        if(bits_per_sample != 16) 
        {
          GAssert(VA("Currently, only 16 bit WAV files are supported (%s).", mName.c_str()));
        }
        if(num_channels != 1 && num_channels != 2) 
        {
          GAssert(VA("Currently, only mono or stereo WAV files are supported (%s).", mName.c_str()));
        }
      }
      // get the data chunk
      else if(chunk->ChunkID == MAKEFOURCC('d','a','t','a'))
      {
        sample_data_ptr = (U8*)(data_ptr+data_pos);
        sample_data_size = chunk->ChunkLen;
      }
      // get the sample chunk
      else if(chunk->ChunkID == MAKEFOURCC('s','m','p','l'))
      {
        WaveSampleChunk* sample_chunk = (WaveSampleChunk*)(data_ptr+data_pos);
        WaveSampleLoop* sample_loop = (WaveSampleLoop*)(sample_chunk+1);
      }

      // the next chunk
      data_pos += chunk->ChunkLen;
    }
    CHECK(sample_data_ptr != NULL && sample_data_size != 0);

    // get the audio format
    U32 format = 0;
    if(num_channels == 1) format = 0x1101;       // AL_FORMAT_MONO16
    else if(num_channels == 2) format = 0x1103;  // AL_FORMAT_STEREO16

    // create the audio buffer    
    mBufferPtr = GNEW(AudioBuffer); CHECK(mBufferPtr);
    mBufferPtr->Load(format, samples_per_sec, sample_data_ptr, sample_data_size);

    // calculate duration.
    I32 duration_div = num_channels * bits_per_sample * samples_per_sec;  
    if(duration_div) mDuration = sample_data_size * 8.0f / duration_div;    
/*
    U8* sound_data_ptr = NULL;
    U32 sound_data_size = 0;

    // get the format chunk
    {
      // reset the data
      U32 data_pos = 0;
      const U8* data_ptr = (U8*)data->Ptr();
      data_pos += sizeof(WaveHeader);

      // look for the 'fmt ' chunk.
      while(data_pos < data->Size())
      {
        // get the chunk info
        WaveChunk* chunk = (WaveChunk*)(data_ptr+data_pos);
        data_pos += sizeof(WaveChunk);
        
        // get the format chunk
        if(chunk->ChunkID == MAKEFOURCC('f','m','t',' '))
        {
          WaveFormatChunk* format_chunk = (WaveFormatChunk*)(data_ptr+data_pos);
          data_pos += sizeof(WaveFormatChunk);

          mBitsPerSample = format_chunk->BitsPerSample;
          mSamplesPerSec = format_chunk->SamplesPerSec;
          mAvgBytesPerSec = format_chunk->AvgBytesPerSec;
          mBlockAlign = format_chunk->BlockAlign;
          mChannels = format_chunk->Channels;

          break;
        }

        // the next chunk
        data_pos += chunk->ChunkLen;
      }
    }

    // check the bits and the number of the channels
    if(mBitsPerSample != 16) GAssert(VA("Currently, only 16 bit WAV files are supported (%s).", mName.c_str()));
    if(mChannels != 1 && mChannels != 2) GAssert(VA("Currently, only mono or stereo WAV files are supported (%s).", mName.c_str()));

    // get the data chunk
    {
      // reset the data
      U32 data_pos = 0;
      const U8* data_ptr = (U8*)data->Ptr();
      data_pos += sizeof(WaveHeader);

      // look for the 'data' chunk.
      while(data_pos < data->Size())
      {
        // get the chunk info
        WaveChunk* chunk = (WaveChunk*)(data_ptr+data_pos);
        data_pos += sizeof(WaveChunk);

        // get the data chunk
        if(chunk->ChunkID == MAKEFOURCC('d','a','t','a'))
        {
          sound_data_ptr = (U8*)(data_ptr+data_pos);
          sound_data_size = chunk->ChunkLen;
          break;
        }

        // the next chunk
        data_pos += chunk->ChunkLen;
      }
    }

    // get the sample chunk
    {
      // reset the data
      U32 data_pos = 0;
      const U8* data_ptr = (U8*)data->Ptr();
      data_pos += sizeof(WaveHeader);

      // look for the 'smpl' chunk.
      while(data_pos < data->Size())
      {
        // get the chunk info
        WaveChunk* chunk = (WaveChunk*)(data_ptr+data_pos);
        data_pos += sizeof(WaveChunk);

        // get the data chunk
        if(chunk->ChunkID == MAKEFOURCC('s','m','p','l'))
        {
          WaveSampleChunk* sample_chunk = (WaveSampleChunk*)(data_ptr+data_pos);
          data_pos += sizeof(WaveSampleChunk);
          WaveSampleLoop* sample_loop = (WaveSampleLoop*)(data_ptr+data_pos);
          data_pos += sizeof(WaveSampleLoop);

          break;
        }

        // the next chunk
        data_pos += chunk->ChunkLen;
      }
    }

    // create the audio buffer
    U32 format = 0;
    if(mChannels == 1) format = 0x1101;       // AL_FORMAT_MONO16
    else if(mChannels == 2) format = 0x1103;  // AL_FORMAT_STEREO16
    mBufferPtr = GNEW(AudioBuffer); CHECK(mBufferPtr);
    mBufferPtr->Load(format, mSamplesPerSec, sound_data_ptr, sound_data_size);
    */
	}

  /*
	====================
	Buffer
	====================
	*/
	const AudioBuffer* WaveData::Buffer() const
	{
		return mBufferPtr.Ptr();
	}

  /*
	====================
	Duration
	====================
	*/
	F32 WaveData::Duration() const
	{
		return mDuration;
	}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
