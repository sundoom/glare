/*
 *  BaseTexture.h
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __BaseTexture__
#define __BaseTexture__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

#include <Glare.h>

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Render)

//////////////////////////////////////////////////////////////////////////

	// this class provides an object-oriented BaseTexture interface.
	class BaseTexture
	{
	public:		
		// constructor
		BaseTexture();
		
		// destructor
		virtual ~BaseTexture();

		// load the data
		virtual VOID Load(U32 level, U32 width, U32 height, U32 bits, U32 format, U32 type, const U8* pixels) = 0;

    // update the texture
    virtual VOID Update(U32 level, I32 x, I32 y, U32 width, U32 height, U32 bits, U32 format, U32 type, const U8* pixels) = 0;

    // bind the texture
    VOID Bind(U32 unit = 0);

    // get the width of the texture		
		U32 Width() const;

		// get the height of the texture
		U32 Height() const;

    // get the format of the texture
    U32 Format() const;

    // set/get the wrap of the texture
    VOID WrapS(U32 s);
    U32 WrapS() const;
    VOID WrapT(U32 t);
    U32 WrapT() const;

    // set/get the filter of the texture
    VOID MinFilter(U32 min);
    U32 MinFilter() const;
    VOID MagFilter(U32 mag);
    U32 MagFilter() const;    

	protected:
		U32	mObject, mTarget;
		U32	mWidth, mHeight;
    U32 mFormat;
    U32 mWrapS, mWrapT;
    U32 mMinFilter, mMagFilter;
    BOOL mDirty;

  private:friend class RenderTarget;
	};

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __BaseTexture__

//////////////////////////////////////////////////////////////////////////