/*
 *  TextureData.h
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __TextureData__
#define __TextureData__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////

	// this class provides an object-oriented TextureData interface.
	class TextureData : public Resource
	{
  public:		
		// constructor
		TextureData(const CHAR* name);
		
		// destructor
		virtual ~TextureData();

    // load the data
    VOID Load(const GData* data);
    VOID Load(U32 level, U32 width, U32 height, U32 bits, U32 format, U32 type, const VOID* pixels);

    // get the handle
    HANDLE Handle() const;

    // get the width of the texture		
    U32 Width() const;

    // get the height of the texture
    U32 Height() const;
  
  private:
    HANDLE mHandle;
    U32	mWidth, mHeight;

  private:friend class Resource;
          friend class Primitive;
	};

  //////////////////////////////////////////////////////////////////////////

  typedef GPtr<TextureData>TextureDataPtr;

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __TextureData__

//////////////////////////////////////////////////////////////////////////