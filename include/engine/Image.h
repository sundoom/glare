/*
 *  Image.h
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __Image__
#define __Image__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Engine)

//////////////////////////////////////////////////////////////////////////

	// this class provides an object-oriented base image interface.
	class Image : public GReference
	{
  public:
    enum TYPE
    {
      IMAGE_NORMAL,
      IMAGE_CUBEMAP,
    };

	public:
		// constructor
		Image();

		// destructor
		virtual ~Image();

    // set/get the type of the image
    VOID Type(U32 t);
    U32 Type() const;

		// set/get the width of the image
		VOID Width(U32 w);
		U32 Width() const;

		// set/get the height of the image
		VOID Height(U32 h);
		U32 Height() const;

    // set/get the number of bits per pixel
    VOID BitCount(U32 b);
    U32 BitCount() const;

		// set/get the format of the pixel
		VOID PixelFormat(U32 f);
		U32 PixelFormat() const;    

		// set/get the data type of the pixel
		VOID DataType(U32 t);
		U32 DataType() const;

		// set/get the count of the mipmap
		VOID MipmapCount(U32 n);
		U32 MipmapCount() const;

		// set/get the data of the mipmap
		VOID Mipmap(const U8*data, U32 size);
		const U8* Mipmap(U32& size) const;
		const U8* Mipmap() const;

	public:
		// load the image
		static Image* Load(const GData* data);
	
	private:
		static Image* LoadTGA(const GData* data);
		static Image* LoadDDS(const GData* data);
    static Image* LoadPVR(const GData* data);

	public:
    U32 mType;
		U32	mWidth, mHeight;
    U32 mBitCount;
		U32	mPixelFormat, mDataType;
    U32 mMipmapCount;
    std::vector<U8>mMipmaps;
	};

	//////////////////////////////////////////////////////////////////////////

	typedef GPtr<Image>ImagePtr;

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __Image__

//////////////////////////////////////////////////////////////////////////