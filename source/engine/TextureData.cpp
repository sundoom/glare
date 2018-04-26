/*
 *  TextureData.cpp
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

  TextureData::TextureData(const CHAR* name) : Resource(name)
	{
		mHandle = NULL;
    mWidth = mHeight = 0;
	}

	TextureData::~TextureData() 
	{
    if(mHandle)
    {
      RIDestroyTexture(mHandle);
      mHandle = NULL;
    }
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	Load
	====================
	*/
	VOID TextureData::Load(const GData* data)
	{
		CHECK(mHandle == NULL);
    ImagePtr image = Image::Load(data); CHECK(image);
		switch(image->Type())
		{
		case Image::IMAGE_NORMAL:
			mHandle = RICreateTexture(image->MipmapCount(), image->Width(), image->Height(), image->BitCount(), image->PixelFormat(), image->DataType(), image->Mipmap());
			break;
		case Image::IMAGE_CUBEMAP:
			mHandle = RICreateCubeTexture(image->MipmapCount(), image->Width(), image->Height(), image->BitCount(), image->PixelFormat(), image->DataType(), image->Mipmap());
			break;
		default:
			GAssert(VA("The image`s type(%s) is unknown!", image->Type()));
			break;
		}
    CHECK(mHandle);
    mWidth = image->Width();
    mHeight = image->Height();
	}

  VOID TextureData::Load(U32 level, U32 width, U32 height, U32 bits, U32 format, U32 type, const VOID* pixels)
  {
    CHECK(mHandle == NULL);
    mHandle = RICreateTexture(level, width, height, bits, format, type, pixels);
    CHECK(mHandle);
    mWidth = width;
    mHeight = height;
  }

  //////////////////////////////////////////////////////////////////////////
  /*
	====================
	Handle
	====================
	*/
  HANDLE TextureData::Handle() const
  {
    return mHandle;
  }

  //////////////////////////////////////////////////////////////////////////

  /*
	====================
	Width
	====================
	*/
	U32 TextureData::Width() const
	{
		return mWidth;
	}

	/*
	====================
	Height
	====================
	*/
	U32 TextureData::Height() const
	{
		return mHeight;
	}	

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
