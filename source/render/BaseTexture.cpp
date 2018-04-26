/*
 *  BaseTexture.cpp
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#include "BaseTexture.h"
#include "State.h"

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Render)

//////////////////////////////////////////////////////////////////////////

	BaseTexture::BaseTexture()
	{
		mObject = 0;
		mWidth = mHeight = 0;
    mFormat = PF_RGBA;
    mWrapS = GL_REPEAT;
    mWrapT = GL_REPEAT;
    mMinFilter = GL_NEAREST_MIPMAP_LINEAR;
    mMagFilter = GL_LINEAR;
    mDirty = TRUE;
	}

	BaseTexture::~BaseTexture() 
	{
		if(mObject){glDeleteTextures(1, &mObject); mObject = 0;}
	}	

  //////////////////////////////////////////////////////////////////////////	

	/*
	====================
	Bind
	====================
	*/
	VOID BaseTexture::Bind(U32 unit)
	{
    // bind the texture
    BindTexture(mTarget, mObject, unit);

    // update the texture parameter
    if(mDirty)
    {
      glTexParameteri(mTarget, GL_TEXTURE_WRAP_S, mWrapS);
      glTexParameteri(mTarget, GL_TEXTURE_WRAP_T, mWrapT);
      glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER, mMinFilter);
      glTexParameteri(mTarget, GL_TEXTURE_MAG_FILTER, mMagFilter);
      mDirty = FALSE;
    }
	}

	//////////////////////////////////////////////////////////////////////////
	
	/*
	====================
	Width
	====================
	*/
	U32 BaseTexture::Width() const
	{
		return mWidth;
	}

	/*
	====================
	Height
	====================
	*/
	U32 BaseTexture::Height() const
	{
		return mHeight;
	}	

  /*
	====================
	Format
	====================
	*/
	U32 BaseTexture::Format() const
	{
		return mFormat;
	}

  //////////////////////////////////////////////////////////////////////////

  /*
	====================
	wrap
	====================
	*/
	VOID BaseTexture::WrapS(U32 s)
	{
		if(mWrapS != s) {mWrapS = s; mDirty = TRUE;}
	}
	
	U32 BaseTexture::WrapS() const
	{
		return mWrapS;
	}

	VOID BaseTexture::WrapT(U32 t)
	{
		if(mWrapT != t) {mWrapT = t; mDirty = TRUE;}
	}

	U32 BaseTexture::WrapT() const
	{
		return mWrapT;
	}

	/*
	====================
	filter
	====================
	*/
	VOID BaseTexture::MinFilter(U32 min)
	{
		if(mMinFilter != min) {mMinFilter = min; mDirty = TRUE;}
	}

	U32 BaseTexture::MinFilter() const
	{
		return mMinFilter;
	}

	VOID BaseTexture::MagFilter(U32 mag)
	{
		if(mMagFilter != mag) {mMagFilter = mag; mDirty = TRUE;}
	}

	U32 BaseTexture::MagFilter() const
	{
		return mMagFilter;
	}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
