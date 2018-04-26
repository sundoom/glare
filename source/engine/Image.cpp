/*
*  Image.cpp
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

	Image::Image()
	{
		mType = IMAGE_NORMAL;
		mWidth = mHeight = 0;
		mPixelFormat = PF_ALPHA;
		mDataType = DT_UNSIGNED_BYTE;
		mBitCount = 8;
	}

	Image::~Image()
	{

	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	Type
	====================
	*/
	VOID Image::Type(U32 t)
	{
		mType = t;
	}

	U32 Image::Type() const
	{
		return mType;
	}

	/*
	====================
	Width
	====================
	*/
	VOID Image::Width(U32 w)
	{
		mWidth = w;
	}

	U32 Image::Width() const
	{
		return mWidth;
	}

	/*
	====================
	Height
	====================
	*/
	VOID Image::Height(U32 h)
	{
		mHeight = h;
	}

	U32 Image::Height() const
	{
		return mHeight;
	}

	/*
	====================
	BitCount
	====================
	*/
	VOID Image::BitCount(U32 b)
	{
		mBitCount = b;
	}

	U32 Image::BitCount() const
	{
		return mBitCount;
	}

	/*
	====================
	PixelFormat
	====================
	*/
	VOID Image::PixelFormat(U32 f)
	{
		mPixelFormat = f;
	}

	U32 Image::PixelFormat() const
	{
		return mPixelFormat;
	}

	/*
	====================
	DataType
	====================
	*/
	VOID Image::DataType(U32 t)
	{
		mDataType = t;
	}

	U32 Image::DataType() const
	{
		return mDataType;
	}

	/*
	====================
	MipmapCount
	====================
	*/
	VOID Image::MipmapCount(U32 n)
	{
		mMipmapCount = n;
	}

	U32 Image::MipmapCount() const
	{
		return mMipmapCount;
	}

	/*
	====================
	Mipmap
	====================
	*/
	VOID Image::Mipmap(const U8*data, U32 size)
	{
		mMipmaps.resize(size);
		if(data)::memcpy(&mMipmaps[0], data, size);	
	}

	const U8* Image::Mipmap(U32& size) const
	{
		size = mMipmaps.size();
		if(size > 0) return &mMipmaps[0];
		return NULL;
	}

	const U8* Image::Mipmap() const
	{
		if(mMipmaps.size() > 0) return &mMipmaps[0];
		return NULL;
	}	

	//////////////////////////////////////////////////////////////////////////
	// TGA
	//////////////////////////////////////////////////////////////////////////

	#pragma pack(push,1)
	struct TGA_Header
	{
		U8	IdFieldLength;
		U8	ColorMapType;
		U8	ImageTypeCode;		// 2 for uncompressed RGB format
		U16	ColorMapOrigin;
		U16	ColorMapLength;
		U8	ColorMapEntrySize;
		U16	XOrigin;
		U16	YOrigin;
		U16	Width;
		U16	Height;
		U8	BitsPerPixel;
		U8	ImageDescriptor;
	};
	#pragma pack(pop)

	/*
	====================
	loadTGA

	load tga image file
	====================
	*/
	Image* Image::LoadTGA(const GData* data)
	{
		CHECK(data);

		// get the header of the tga
		const TGA_Header* tga_header = (TGA_Header*)data->Ptr();
		if(tga_header->ImageTypeCode == 10) // 10 = RLE compressed 
		{
			if(tga_header->BitsPerPixel == 32)
			{
				// check the stride of the image
				U16 usize = tga_header->Width, vsize = tga_header->Height;
				U16 ubits = 0, vbits = 0;
				for(I32 i = 1; i < usize; i <<= 1) ubits++;	
				for(I32 j = 1; j < vsize; j <<= 1) vbits++;
				if(usize != (1<<ubits) || vsize != (1<<vbits)){ GAssert( "The image width or height is not 2 power!\n" ); return NULL; }

				// build the image
				Image* image = GNEW(Image); CHECK(image);
				image->Type(IMAGE_NORMAL);
				image->Width(usize);
				image->Height(vsize);
				image->BitCount(tga_header->BitsPerPixel);
				image->PixelFormat(PF_RGBA);
				image->DataType(DT_UNSIGNED_BYTE);
				image->MipmapCount(0);
				image->Mipmap(NULL,usize*vsize*tga_header->BitsPerPixel/8);

				// process image data
				U8*	idData = (U8*)tga_header + sizeof(TGA_Header); 
				U8*	colorMap = idData + tga_header->IdFieldLength;
				U8*	imageData = (U8*)(colorMap + (tga_header->ColorMapEntrySize + 4) / 8 * tga_header->ColorMapLength);
				U32* pixelData = (U32*)image->Mipmap();

				U8	pixel[4];
				I32 rleRun = 0;
				I32 rawRun = 0;

				for(I32 Y = tga_header->Height - 1; Y >= 0; Y--) // Y-flipped.
				{					
					for(I32 X = 0; X < tga_header->Width; X++)
					{						
						if(rleRun > 0)
						{
							rleRun--;  // reuse current Pixel data.
						}
						else if(rawRun == 0) // new raw pixel or RLE-run.
						{
							U8 rleChunk = *(imageData++);							
							if(rleChunk & 0x80)
							{
								rleRun = ( rleChunk & 0x7F ) + 1;
								rawRun = 1;
							}
							else
							{
								rawRun = ( rleChunk & 0x7F ) + 1;
							}
						}							
						// Retrieve new pixel data - raw run or single pixel for RLE stretch.
						if(rawRun > 0)
						{
							pixel[2] = imageData[0];
							pixel[1] = imageData[1];
							pixel[0] = imageData[2];
							pixel[3] = imageData[3];
							imageData += 4;
							rawRun--;
							rleRun--;
						}
						// Store.
						pixelData[Y*tga_header->Width + X] = *(U32*)&pixel;
					}
				}

				return image;
			}
			else if(tga_header->BitsPerPixel == 24)
			{
				// check the stride of the image
				U16 usize = tga_header->Width, vsize = tga_header->Height;
				U16 ubits = 0, vbits = 0;
				for(I32 i = 1; i < usize; i <<= 1) ubits++;	
				for(I32 j = 1; j < vsize; j <<= 1) vbits++;
				if(usize != (1<<ubits) || vsize != (1<<vbits) ){ GAssert( "The image width or height is not 2 power!\n" ); return NULL;}

				// build the image
				Image* image = GNEW(Image); CHECK(image);
				image->Type(IMAGE_NORMAL);
				image->Width(usize);
				image->Height(vsize);
				image->BitCount(tga_header->BitsPerPixel);
				image->PixelFormat(PF_RGB);
				image->DataType(DT_UNSIGNED_BYTE);
				image->MipmapCount(0);
				image->Mipmap(NULL,usize*vsize*tga_header->BitsPerPixel/8);

				// process image data
				U8*	idData = (U8*)tga_header + sizeof(TGA_Header); 
				U8*	colorMap = idData + tga_header->IdFieldLength;
				U8*	imageData = (U8*)(colorMap + (tga_header->ColorMapEntrySize + 4) / 8 * tga_header->ColorMapLength);
				U8* pixelData = (U8*)image->Mipmap();
				U8 pixel[3];
				I32 rleRun = 0;
				I32 rawRun = 0;

				for(I32 Y = tga_header->Height - 1; Y >= 0; Y--) // Y-flipped.
				{					
					for(I32 X = 0;X < tga_header->Width; X++)
					{						
						if(rleRun > 0)
						{
							rleRun--;  // reuse current Pixel data.
						}
						else if(rawRun == 0) // new raw pixel or RLE-run.
						{
							U8 rleChunk = *(imageData++);
							if(rleChunk & 0x80)
							{
								rleRun = (rleChunk & 0x7F) + 1;
								rawRun = 1;
							}
							else
							{
								rawRun = (rleChunk & 0x7F) + 1;
							}
						}							
						// Retrieve new pixel data - raw run or single pixel for RLE stretch.
						if(rawRun > 0)
						{
							pixel[2] = *(imageData++);
							pixel[1] = *(imageData++);
							pixel[0] = *(imageData++);
							rawRun--;
							rleRun--;
						}
						// Store.
						pixelData[(Y*tga_header->Width*3) + (X*3+0)] = pixel[0];
						pixelData[(Y*tga_header->Width*3) + (X*3+1)] = pixel[1];
						pixelData[(Y*tga_header->Width*3) + (X*3+2)] = pixel[2];
					}
				}

				return image;
			}
			else if(tga_header->BitsPerPixel == 16)
			{
				// check the stride of the image
				U16 usize = tga_header->Width, vsize = tga_header->Height;
				U16 ubits = 0, vbits = 0;
				for(I32 i = 1; i < usize; i <<= 1) ubits++;
				for(I32 j = 1; j < vsize; j <<= 1) vbits++;
				if(usize != (1<<ubits) || vsize != (1<<vbits) ){ GAssert( "The image width or height is not 2 power!\n" ); return NULL; }

				// build the image
				Image* image = GNEW(Image); CHECK(image);
				image->Type(IMAGE_NORMAL);
				image->Width(usize);
				image->Height(vsize);
				image->BitCount(tga_header->BitsPerPixel);
				image->PixelFormat(PF_RGB);
				image->DataType(DT_UNSIGNED_SHORT_5_6_5);
				image->MipmapCount(0);
				image->Mipmap(NULL,usize*vsize*tga_header->BitsPerPixel/8);

				// process image data
				U8*	idData = (U8*)tga_header + sizeof(TGA_Header);
				U8*	colorMap = idData + tga_header->IdFieldLength;				
				U16*imageData = (U16*)(colorMap + (tga_header->ColorMapEntrySize + 4) / 8 * tga_header->ColorMapLength);
				U16*pixelData = (U16*)image->Mipmap();
				U16 pixel;
				I32 rleRun = 0;
				I32 rawRun = 0;

				for(I32 Y = tga_header->Height - 1; Y >= 0; Y--) // Y-flipped.
				{					
					for(I32 X = 0; X < tga_header->Width; X++)
					{						
						if(rleRun > 0)
						{
							rleRun--;  // reuse current Pixel data.
						}
						else if(rawRun == 0) // new raw pixel or RLE-run.
						{
							U8 rleChunk =  *((U8*)imageData);
							imageData = (U16*)(((U8*)imageData) + 1);
							if(rleChunk & 0x80)
							{
								rleRun = (rleChunk & 0x7F) + 1;
								rawRun = 1;
							}
							else
							{
								rawRun = (rleChunk & 0x7F) + 1;
							}
						}							
						// Retrieve new pixel data - raw run or single pixel for RLE stretch.
						if(rawRun > 0)
						{ 
							U16 color = *(imageData++);
							pixel = (((color>>10)&0x1f)<<11) | (((color>>5)&0x1f)<<6) | (color&0x1f);
							rawRun--;
							rleRun--;
						}
						// Store.
						pixelData[Y*tga_header->Width + X] = pixel;
					}
				}

				return image;
			}
			else
			{
				GAssert( VA( "TGA header uses an unsupported rle-compressed bit-depth: %u", tga_header->BitsPerPixel ) );
				return NULL;
			}
		}
		else if(tga_header->ImageTypeCode == 2) // 2 = Uncompressed 
		{
			if(tga_header->BitsPerPixel == 32)
			{
				// check the stride of the image
				U16 usize = tga_header->Width, vsize = tga_header->Height;
				U16 ubits = 0, vbits = 0;
				for(I32 i = 1; i < usize; i <<= 1) ubits++;	
				for(I32 j = 1; j < vsize; j <<= 1) vbits++;
				if(usize != (1<<ubits) || vsize != (1<<vbits)){ GAssert( "The image width or height is not 2 power!\n" ); return NULL; }

				// build the image
				Image* image = GNEW(Image); CHECK(image);
				image->Type(IMAGE_NORMAL);
				image->Width(usize);
				image->Height(vsize);
				image->BitCount(tga_header->BitsPerPixel);
				image->PixelFormat(PF_RGBA);
				image->DataType(DT_UNSIGNED_BYTE);
				image->MipmapCount(0);
				image->Mipmap(NULL,usize*vsize*tga_header->BitsPerPixel/8);

				// process image data
				U8*	idData = (U8*)tga_header + sizeof(TGA_Header);
				U8*	colorMap = idData + tga_header->IdFieldLength;
				U8* imageData = (U8*)(colorMap + (tga_header->ColorMapEntrySize + 4)/8 * tga_header->ColorMapLength);
				U32* pixelData = (U32*)image->Mipmap();
				U8 pixel[4];

				for(I32 Y = 0; Y < tga_header->Height; Y++)
				{
					for(I32 X = 0; X < tga_header->Width; X++)
					{
						pixel[2] = imageData[((tga_header->Height-Y-1) * tga_header->Width*4) + (X*4+0)];
						pixel[1] = imageData[((tga_header->Height-Y-1) * tga_header->Width*4) + (X*4+1)];
						pixel[0] = imageData[((tga_header->Height-Y-1) * tga_header->Width*4) + (X*4+2)];
						pixel[3] = imageData[((tga_header->Height-Y-1) * tga_header->Width*4) + (X*4+3)];
						pixelData[Y*tga_header->Width + X] = *(U32*)&pixel;
					}
				}

				return image;
			}
			else if(tga_header->BitsPerPixel == 24)
			{
				// check the stride of the image
				U16 usize = tga_header->Width, vsize = tga_header->Height;
				U16 ubits = 0, vbits = 0;
				for(I32 i = 1; i < usize; i <<= 1) ubits++;	
				for(I32 j = 1; j < vsize; j <<= 1) vbits++;
				if(usize != (1<<ubits) || vsize != (1<<vbits)){ GAssert( "The image width or height is not 2 power!\n" ); return NULL; }				

				// build the image
				Image* image = GNEW(Image); CHECK(image);
				image->Type(IMAGE_NORMAL);
				image->Width(usize);
				image->Height(vsize);
				image->BitCount(tga_header->BitsPerPixel);
				image->PixelFormat(PF_RGB);
				image->DataType(DT_UNSIGNED_BYTE);
				image->MipmapCount(0);
				image->Mipmap(NULL,usize*vsize*tga_header->BitsPerPixel/8);
				U8 pixel[3];

				// process image data
				U8*	idData = (U8*)tga_header + sizeof(TGA_Header);
				U8*	colorMap = idData + tga_header->IdFieldLength;
				U8*	imageData = (U8*)(colorMap + (tga_header->ColorMapEntrySize + 4) / 8 * tga_header->ColorMapLength);
				U8* pixelData = (U8*)image->Mipmap();

				for(I32 Y = 0; Y < tga_header->Height; Y++)
				{
					for(I32 X = 0; X < tga_header->Width; X++)
					{
						pixel[2] = imageData[((tga_header->Height-Y-1)*tga_header->Width*3) + (X*3+0)];
						pixel[1] = imageData[((tga_header->Height-Y-1)*tga_header->Width*3) + (X*3+1)];
						pixel[0] = imageData[((tga_header->Height-Y-1)*tga_header->Width*3) + (X*3+2)];
						pixelData[(Y*tga_header->Width*3) + (X*3+0)] = pixel[0];
						pixelData[(Y*tga_header->Width*3) + (X*3+1)] = pixel[1];
						pixelData[(Y*tga_header->Width*3) + (X*3+2)] = pixel[2];
					}
				}

				return image;
			}
			else if(tga_header->BitsPerPixel == 16)
			{
				// check the stride of the image
				U16 usize = tga_header->Width, vsize = tga_header->Height;
				U16 ubits = 0, vbits = 0;
				for(I32 i = 1; i < usize; i <<= 1) ubits++;	
				for(I32 j = 1; j < vsize; j <<= 1) vbits++;
				if(usize != (1<<ubits) || vsize != (1<<vbits)){ GAssert( "The image width or height is not 2 power!\n" ); return NULL; }

				// build the image
				Image* image = GNEW(Image); CHECK(image);
				image->Type(IMAGE_NORMAL);
				image->Width(usize);
				image->Height(vsize);
				image->BitCount(tga_header->BitsPerPixel);
				image->PixelFormat(PF_RGB);
				image->DataType(DT_UNSIGNED_SHORT_5_6_5);
				image->MipmapCount(0);
				image->Mipmap(NULL,usize*vsize*tga_header->BitsPerPixel/8);

				// process image data
				U8*	idData = (U8*)tga_header + sizeof(TGA_Header);
				U8*	colorMap = idData + tga_header->IdFieldLength;
				U16* imageData = (U16*)(colorMap + (tga_header->ColorMapEntrySize + 4) / 8 * tga_header->ColorMapLength);
				U16* pixelData = (U16*)image->Mipmap();

				for(I32 Y = 0; Y < tga_header->Height; Y++)
				{
					for(I32 X = 0; X < tga_header->Width; X++)
					{
						U16 color = imageData[(tga_header->Height-Y-1) * tga_header->Width + X];
						U16 pixel = (((color>>10)&0x1f)<<11) | (((color>>5)&0x1f)<<6) | (color&0x1f);
						pixelData[Y*tga_header->Width + X] = pixel;
					}
				}

				return image;
			}
			else
			{
				GAssert( VA("TGA uses an unsupported bit-depth: %u" ,tga_header->BitsPerPixel) );
				return NULL;
			}
		}
		else
		{
			GAssert( VA("TGA is an unsupported type: %u", tga_header->ImageTypeCode) );
			return NULL;
		}

		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	// DDS
	//////////////////////////////////////////////////////////////////////////

	#pragma pack(push,1)

	// DDPixelFormat flags
	enum 
	{
		DDPF_ALPHAPIXELS        = 0x00000001l,
		DDPF_ALPHA              = 0x00000002l,
		DDPF_FOURCC             = 0x00000004l,
		DDPF_RGB                = 0x00000040l,		
		DDPF_LUMINANCE          = 0x00020000l
	};

	struct DDPixelFormat
	{
		U32	Size;
		U32	Flags;
		U32	FourCC;
		U32	RGBBitCount;
		U32	RBitMask;
		U32	GBitMask;
		U32	BBitMask;
		U32	RGBAlphaBitMask;
	};	

	// DDCaps2 flags
	enum 
	{
		DDSCAPS_TEXTURE	= 0x00001000l,
		DDSCAPS_COMPLEX	= 0x00000008l,
		DDSCAPS_MIPMAP	= 0x00400000l,
		DDSCAPS2_VOLUME	= 0x00200000l
	};

	struct DDCaps2
	{
		U32	Caps1;
		U32	Caps2;
		U32	Reserved[2];
	};	

	// DDSurfaceDesc2 flags
	enum 
	{
		DDSD_CAPS			= 0x00000001l,
		DDSD_HEIGHT			= 0x00000002l,
		DDSD_WIDTH			= 0x00000004l,
		DDSD_PITCH			= 0x00000008l,
		DDSD_PIXELFORMAT	= 0x00001000l,		
		DDSD_MIPMAPCOUNT	= 0x00020000l,
		DDSD_LINEARSIZE		= 0x00080000l,
		DDSD_DEPTH			= 0x00800000l
	};

	struct DDSurfaceDesc2
	{
		U32	Size;
		U32	Flags;
		U32	Height;
		U32	Width;
		U32	PitchOrLinearSize;
		U32	Depth;
		U32	MipMapCount;
		U32	Reserved1[11];
		DDPixelFormat PixelFormat;
		DDCaps2	Caps;
		U32	Reserved2;
	};

	#define FOURCC_DXT1  (MAKEFOURCC('D','X','T','1'))	
	#define FOURCC_DXT3  (MAKEFOURCC('D','X','T','3'))
	#define FOURCC_DXT5  (MAKEFOURCC('D','X','T','5'))

	#pragma pack(pop)

	/*
	====================
	loadDDS

	load dds image file
	====================
	*/
	Image* Image::LoadDDS(const GData* data)
	{
		CHECK(data);

		// get the data
		U32 data_pos = 0;
		const U8* data_ptr = (U8*)data->Ptr();

		// check the dds magic
		U32 magic = *(U32*)(data_ptr+data_pos);
		data_pos += sizeof(U32);
		CHECK(magic == MAKEFOURCC('D','D','S',' '));

		// get the DDSurfaceDesc2
		DDSurfaceDesc2& ddsd = *(DDSurfaceDesc2*)(data_ptr+data_pos);
		data_pos += sizeof(DDSurfaceDesc2);

		// check valid structure sizes
		CHECK(ddsd.Size == 124 && ddsd.PixelFormat.Size == 32);

		// get the image properties.
		U32 width = 0, height = 0, depth = 0;
		width = ddsd.Width;
		height = ddsd.Height;
		if( ddsd.Flags & DDSD_DEPTH ) depth = ddsd.Depth;

		// the pixel info
		U32 data_type = 0, pixel_format	= 0, pixel_size = 0;

		// handle some esoteric formats
		if(ddsd.PixelFormat.Flags & DDPF_ALPHA)
		{
			pixel_format  = PF_ALPHA;
			data_type		  = DT_UNSIGNED_BYTE;
			pixel_size		= 1;
		}
		else if(ddsd.PixelFormat.Flags & DDPF_FOURCC)
		{
			switch(ddsd.PixelFormat.FourCC)
			{
			case FOURCC_DXT1:
				if(ddsd.PixelFormat.Flags&DDPF_ALPHAPIXELS)
					pixel_format  = PF_RGBA_DXT1;
				else 
					pixel_format  = PF_RGB_DXT1;
				data_type		    = DT_UNSIGNED_BYTE;
				pixel_size		  = 1;
				break;
			case FOURCC_DXT3:
				pixel_format  = PF_RGBA_DXT3;
				data_type		  = DT_UNSIGNED_BYTE;
				pixel_size		= 2;
				break;
			case FOURCC_DXT5:
				pixel_format  = PF_RGBA_DXT5;
				data_type		  = DT_UNSIGNED_BYTE;
				pixel_size		= 2;
				break;
			default:
				GAssert( VA("The FourCC pixel format(%c%c%c%c) is not supported!",  
					(CHAR)((ddsd.PixelFormat.FourCC & 0x000000ff)),
					(CHAR)((ddsd.PixelFormat.FourCC & 0x0000ff00)>>8), 
					(CHAR)((ddsd.PixelFormat.FourCC & 0x00ff0000)>>16), 
					(CHAR)((ddsd.PixelFormat.FourCC & 0xff000000)>>24)) );
				return NULL; 
			}
		}
		else if(ddsd.PixelFormat.Flags & DDPF_RGB)
		{
			struct RGBFormat
			{
				const CHAR* name;
				U32 bit_count;
				U32 r_bit_mask;
				U32 g_bit_mask;
				U32 b_bit_mask;
				U32	a_bit_mask;        
				U32 pixel_format;
				U32 data_type;
			};
			static const RGBFormat rgb_formats[] =            
			{        
				{ "RGB",			24,		0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000,	PF_RGB,		DT_UNSIGNED_BYTE  },        
				{ "RGBA",		  32,		0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000,	PF_RGBA,	DT_UNSIGNED_BYTE  },         
			};
			BOOL found = FALSE;
			for(U32 i = 0; i < sizeof(rgb_formats)/sizeof(RGBFormat); i++)
			{
				const RGBFormat& f = rgb_formats[i];
				if(ddsd.PixelFormat.RGBBitCount     == f.bit_count &&
					ddsd.PixelFormat.RBitMask         == f.r_bit_mask &&
					ddsd.PixelFormat.GBitMask         == f.g_bit_mask &&
					ddsd.PixelFormat.BBitMask         == f.b_bit_mask &&
					ddsd.PixelFormat.RGBAlphaBitMask  == f.a_bit_mask )
				{
					if(f.pixel_format != 0 && f.data_type != 0)
					{
						pixel_format  = f.pixel_format;
						data_type     = f.data_type;
						pixel_size		= f.bit_count/8;
						found = TRUE;
						break;
					}
					else
					{
						GAssert( VA("The RGB pixel format(%s) is not supported!",f.name) );
						return NULL;                   
					}
				}
			}
			if(!found)
			{
				GAssert( VA("The RGB pixel format is not supported! ddsd.PixelFormat.RGBBitCount = %d ddsd.PixelFormat.RBitMask = 0x%x ddsd.PixelFormat.GBitMask = 0x%x ddsd.PixelFormat.BBitMask = 0x%x ddsd.PixelFormat.RGBAlphaBitMask = 0x%x",
					ddsd.PixelFormat.RGBBitCount, ddsd.PixelFormat.RBitMask, ddsd.PixelFormat.GBitMask, ddsd.PixelFormat.BBitMask, ddsd.PixelFormat.RGBAlphaBitMask) );
				return NULL;
			}
		}
		else if(ddsd.PixelFormat.Flags & DDPF_LUMINANCE)
		{
			pixel_format  = (ddsd.PixelFormat.Flags&DDPF_ALPHAPIXELS) ? PF_LA : PF_L;
			data_type		  = DT_UNSIGNED_BYTE;
			pixel_size		= (ddsd.PixelFormat.Flags&DDPF_ALPHAPIXELS) ? 2 : 1;
		}
		else
		{
			GAssert( VA("DDS pixel format is error: 0x%x format is not supported!",ddsd.PixelFormat.Flags) );
			return NULL;
		}

		// the total of the pixel size
		U32 total_size = 0;
		U32 mipmap_count = 0;

		// take care of mipmaps if any.    
		if(ddsd.MipMapCount > 1)
		{
			// handle s3tc compressed mipmaps.
			if(ddsd.PixelFormat.Flags&DDPF_FOURCC)
			{
				CHECK(ddsd.PixelFormat.FourCC==FOURCC_DXT1 || ddsd.PixelFormat.FourCC==FOURCC_DXT3 || ddsd.PixelFormat.FourCC==FOURCC_DXT5);
				U32 w = ddsd.Width;
				U32 h = ddsd.Height;
				U32 block_size = (ddsd.PixelFormat.FourCC == FOURCC_DXT1) ? 8 : 16;
				for(U32 k = 1; k < ddsd.MipMapCount && (w || h); ++k)
				{
					if(w == 0) w = 1;
					if(h == 0) h = 1;
					total_size += (((w+3)/4) * ((h+3)/4) * block_size);
					w >>= 1;
					h >>= 1;
					mipmap_count++;
				}
			}
			// handle uncompressed mipmaps
			else
			{
				U32 w = ddsd.Width;
				U32 h = ddsd.Height;
				for(U32 k = 1; k < ddsd.MipMapCount && (w || h); ++k)
				{
					if(w == 0) w = 1;
					if(h == 0) h = 1;
					total_size += w * h * pixel_size;
					w >>= 1;
					h >>= 1;
					mipmap_count++;
				}
			}
		}
		else
		{
			mipmap_count = 1;
			if(ddsd.PixelFormat.Flags&DDPF_FOURCC)
			{
				CHECK(ddsd.PixelFormat.FourCC==FOURCC_DXT1 || ddsd.PixelFormat.FourCC==FOURCC_DXT3 || ddsd.PixelFormat.FourCC==FOURCC_DXT5);
				U32 block_size = (ddsd.PixelFormat.FourCC == FOURCC_DXT1) ? 8 : 16;
				total_size += (((width+3)/4) * ((height+3)/4) * block_size);
			}
			else
			{
				total_size = width * height * pixel_size;
			}
		}

		// build the image
		Image* image = GNEW(Image); CHECK(image);
		image->Type(IMAGE_NORMAL);
		image->Width(width);
		image->Height(height);
		image->BitCount(pixel_size);
		image->PixelFormat(pixel_format);
		image->DataType(data_type);
		image->MipmapCount(mipmap_count);
		image->Mipmap(data_ptr+data_pos,total_size);
		return image;
	}

	//////////////////////////////////////////////////////////////////////////
	// PVR
	//////////////////////////////////////////////////////////////////////////

	#pragma pack(push,1)

	struct PVR_Texture_Header
	{
		U32 HeaderSize;			  /*!< size of the structure */
		U32 Height;				    /*!< height of surface to be created */
		U32 Width;				    /*!< width of input surface */
		U32 MipMapCount;			/*!< number of mip-map levels requested */
		U32 pfFlags;				  /*!< pixel format flags */
		U32 TextureDataSize;	/*!< Total size in bytes */
		U32 BitCount;			    /*!< number of bits per pixel  */
		U32 RBitMask;			    /*!< mask for red bit */
		U32 GBitMask;			    /*!< mask for green bits */
		U32 BBitMask;			    /*!< mask for blue bits */
		U32 AlphaBitMask;		  /*!< mask for alpha channel */
		U32 PVR;					    /*!< magic number identifying pvr file */
		U32 NumSurfs;			    /*!< the number of surfaces present in the pvr */
	};

	enum PVR_Pixel_Type
	{
		MGLPT_ARGB_4444 = 0x00,
		MGLPT_ARGB_1555,
		MGLPT_RGB_565,
		MGLPT_RGB_555,
		MGLPT_RGB_888,
		MGLPT_ARGB_8888,
		MGLPT_ARGB_8332,
		MGLPT_I_8,
		MGLPT_AI_88,
		MGLPT_1_BPP,
		MGLPT_VY1UY0,
		MGLPT_Y1VY0U,
		MGLPT_PVRTC2,
		MGLPT_PVRTC4,

		// OpenGL version of pixel types
		OGL_RGBA_4444= 0x10,
		OGL_RGBA_5551,
		OGL_RGBA_8888,
		OGL_RGB_565,
		OGL_RGB_555,
		OGL_RGB_888,
		OGL_I_8,
		OGL_AI_88,
		OGL_PVRTC2,
		OGL_PVRTC4,
		OGL_BGRA_8888,
		OGL_A_8,
		OGL_PVRTCII4,	//Not in use
		OGL_PVRTCII2,	//Not in use

	#ifdef _WIN32
		// S3TC Encoding
		D3D_DXT1 = 0x20,
		D3D_DXT2,
		D3D_DXT3,
		D3D_DXT4,
		D3D_DXT5,
	#endif

		//RGB Formats
		D3D_RGB_332 
	#ifndef _WIN32
		= 0x25
	#endif
		,
		D3D_AL_44,
		D3D_LVU_655,
		D3D_XLVU_8888,
		D3D_QWVU_8888,

		//10 bit integer - 2 bit alpha
		D3D_ABGR_2101010,
		D3D_ARGB_2101010,
		D3D_AWVU_2101010,

		//16 bit integers
		D3D_GR_1616,
		D3D_VU_1616,
		D3D_ABGR_16161616,

		//Float Formats
		D3D_R16F,
		D3D_GR_1616F,
		D3D_ABGR_16161616F,

		//32 bits per channel
		D3D_R32F,
		D3D_GR_3232F,
		D3D_ABGR_32323232F,

		// Ericsson
		ETC_RGB_4BPP,
		ETC_RGBA_EXPLICIT,				// unimplemented
		ETC_RGBA_INTERPOLATED,			// unimplemented

		D3D_A8 = 0x40,
		D3D_V8U8,
		D3D_L16,

		D3D_L8,
		D3D_AL_88,

		//Y'UV Colourspace
		D3D_UYVY,
		D3D_YUY2,

		// DX10
		DX10_R32G32B32A32_FLOAT= 0x50,
		DX10_R32G32B32A32_UINT , 
		DX10_R32G32B32A32_SINT,

		DX10_R32G32B32_FLOAT,
		DX10_R32G32B32_UINT,
		DX10_R32G32B32_SINT,

		DX10_R16G16B16A16_FLOAT ,
		DX10_R16G16B16A16_UNORM,
		DX10_R16G16B16A16_UINT ,
		DX10_R16G16B16A16_SNORM ,
		DX10_R16G16B16A16_SINT ,

		DX10_R32G32_FLOAT ,
		DX10_R32G32_UINT ,
		DX10_R32G32_SINT ,

		DX10_R10G10B10A2_UNORM ,
		DX10_R10G10B10A2_UINT ,

		DX10_R11G11B10_FLOAT ,				// unimplemented

		DX10_R8G8B8A8_UNORM , 
		DX10_R8G8B8A8_UNORM_SRGB ,
		DX10_R8G8B8A8_UINT ,
		DX10_R8G8B8A8_SNORM ,
		DX10_R8G8B8A8_SINT ,

		DX10_R16G16_FLOAT , 
		DX10_R16G16_UNORM , 
		DX10_R16G16_UINT , 
		DX10_R16G16_SNORM ,
		DX10_R16G16_SINT ,

		DX10_R32_FLOAT ,
		DX10_R32_UINT ,
		DX10_R32_SINT ,

		DX10_R8G8_UNORM ,
		DX10_R8G8_UINT ,
		DX10_R8G8_SNORM , 
		DX10_R8G8_SINT ,

		DX10_R16_FLOAT ,
		DX10_R16_UNORM ,
		DX10_R16_UINT ,
		DX10_R16_SNORM ,
		DX10_R16_SINT ,

		DX10_R8_UNORM, 
		DX10_R8_UINT,
		DX10_R8_SNORM,
		DX10_R8_SINT,

		DX10_A8_UNORM, 
		DX10_R1_UNORM, 
		DX10_R9G9B9E5_SHAREDEXP,	// unimplemented
		DX10_R8G8_B8G8_UNORM,		// unimplemented
		DX10_G8R8_G8B8_UNORM,		// unimplemented

	#ifdef _WIN32
		DX10_BC1_UNORM,	
		DX10_BC1_UNORM_SRGB,

		DX10_BC2_UNORM,	
		DX10_BC2_UNORM_SRGB,

		DX10_BC3_UNORM,	
		DX10_BC3_UNORM_SRGB,

		DX10_BC4_UNORM,				// unimplemented
		DX10_BC4_SNORM,				// unimplemented

		DX10_BC5_UNORM,				// unimplemented
		DX10_BC5_SNORM,				// unimplemented
	#endif
		// OpenVG

		/* RGB{A,X} channel ordering */
		ePT_VG_sRGBX_8888  = 0x90,
		ePT_VG_sRGBA_8888,
		ePT_VG_sRGBA_8888_PRE,
		ePT_VG_sRGB_565,
		ePT_VG_sRGBA_5551,
		ePT_VG_sRGBA_4444,
		ePT_VG_sL_8,
		ePT_VG_lRGBX_8888,
		ePT_VG_lRGBA_8888,
		ePT_VG_lRGBA_8888_PRE,
		ePT_VG_lL_8,
		ePT_VG_A_8,
		ePT_VG_BW_1,

		/* {A,X}RGB channel ordering */
		ePT_VG_sXRGB_8888,
		ePT_VG_sARGB_8888,
		ePT_VG_sARGB_8888_PRE,
		ePT_VG_sARGB_1555,
		ePT_VG_sARGB_4444,
		ePT_VG_lXRGB_8888,
		ePT_VG_lARGB_8888,
		ePT_VG_lARGB_8888_PRE,

		/* BGR{A,X} channel ordering */
		ePT_VG_sBGRX_8888,
		ePT_VG_sBGRA_8888,
		ePT_VG_sBGRA_8888_PRE,
		ePT_VG_sBGR_565,
		ePT_VG_sBGRA_5551,
		ePT_VG_sBGRA_4444,
		ePT_VG_lBGRX_8888,
		ePT_VG_lBGRA_8888,
		ePT_VG_lBGRA_8888_PRE,

		/* {A,X}BGR channel ordering */
		ePT_VG_sXBGR_8888,
		ePT_VG_sABGR_8888 ,
		ePT_VG_sABGR_8888_PRE,
		ePT_VG_sABGR_1555,
		ePT_VG_sABGR_4444,
		ePT_VG_lXBGR_8888,
		ePT_VG_lABGR_8888,
		ePT_VG_lABGR_8888_PRE,

		// max cap for iterating
		END_OF_PIXEL_TYPES,

		MGLPT_NOTYPE = 0xffffffff

	};

	const U32 PVRTEX_MIPMAP		      = (1<<8);		  // has mip map levels
	const U32 PVRTEX_TWIDDLE		    = (1<<9);		  // is twiddled
	const U32 PVRTEX_BUMPMAP		    = (1<<10);		// has normals encoded for a bump map
	const U32 PVRTEX_TILING		      = (1<<11);		// is bordered for tiled pvr
	const U32 PVRTEX_CUBEMAP		    = (1<<12);		// is a cubemap/skybox
	const U32 PVRTEX_FALSEMIPCOL	  = (1<<13);		// are there false coloured MIP levels
	const U32 PVRTEX_VOLUME		      = (1<<14);		// is this a volume texture
	const U32 PVRTEX_ALPHA			    = (1<<15);		// v2.1 is there transparency info in the texture
	const U32 PVRTEX_VERTICAL_FLIP	= (1<<16);		// v2.1 is the texture vertically flipped

	const U32 PVRTEX_PIXELTYPE		  = 0xff;			  // pixel type is always in the last 16bits of the flags
	const U32 PVRTEX_IDENTIFIER	    = 0x21525650;	// the pvr identifier is the characters 'P','V','R'

	const U32 PVRTEX_V1_HEADER_SIZE = 44;			  // old header size was 44 for identification purposes

	const U32 PVRTC2_MIN_TEXWIDTH		= 16;
	const U32 PVRTC2_MIN_TEXHEIGHT	= 8;
	const U32 PVRTC4_MIN_TEXWIDTH		= 8;
	const U32 PVRTC4_MIN_TEXHEIGHT	= 8;
	const U32 ETC_MIN_TEXWIDTH			= 4;
	const U32 ETC_MIN_TEXHEIGHT		  = 4;
	const U32 DXT_MIN_TEXWIDTH			= 4;
	const U32 DXT_MIN_TEXHEIGHT		  = 4;

	#pragma pack(pop)

	/*
	====================
	LoadPVR

	load pvr image file
	====================
	*/
	Image* Image::LoadPVR(const GData* data)
	{
		CHECK(data);

		// get the data
		U32 data_pos = 0;
		const U8* data_ptr = (U8*)data->Ptr();

		// get the pvr header
		PVR_Texture_Header* pvr_header = (PVR_Texture_Header*)data_ptr;
		data_pos += pvr_header->HeaderSize;    

		// check the pvr magic number identifying
		CHECK(pvr_header->PVR == PVRTEX_IDENTIFIER);

		// get the image properties.
		U32 width = 0, height = 0;
		width = pvr_header->Width;
		height = pvr_header->Height;
		U32 num_surfs = 1;

		// perform checks for old PVR pvr_header
		if(pvr_header->HeaderSize!=sizeof(PVR_Texture_Header))
		{	
			// Header V1
			if(pvr_header->HeaderSize==PVRTEX_V1_HEADER_SIZE)
			{	
				// react to old pvr_header: i.e. fill in numsurfs as this is missing from old header
				GAssert("LoadPVR warning: this is an old pvr - you can use PVRTexTool to update its header.\n");
				if(pvr_header->pfFlags&PVRTEX_CUBEMAP) num_surfs = 6;
			}
			else
			{	
				// not a pvr at all
				GAssert("LoadPVR failed: not a valid pvr.\n");
				return NULL;
			}
		}
		else
		{	
			// Header V2
			if(pvr_header->NumSurfs<1)
			{	
				// encoded with old version of PVRTexTool before zero numsurfs bug found.
				if(pvr_header->pfFlags&PVRTEX_CUBEMAP) num_surfs = 6;
			}
			else
			{
				num_surfs = pvr_header->NumSurfs;
			}
		}

		// Only accept untwiddled data UNLESS texture format is PVRTC
		if( ((pvr_header->pfFlags & PVRTEX_TWIDDLE) == PVRTEX_TWIDDLE)
			&& ((pvr_header->pfFlags & PVRTEX_PIXELTYPE)!=OGL_PVRTC2)
			&& ((pvr_header->pfFlags & PVRTEX_PIXELTYPE)!=OGL_PVRTC4) )
		{
			// We need to load untwiddled textures -- GL will twiddle for us.
			GAssert("LoadPVR failed: texture should be untwiddled.\n");
			return NULL;
		}

		// the pixel info
		U32 data_type = 0, pixel_format	= 0, pixel_size = pvr_header->BitCount;
		U32 pvr_pixel_type = pvr_header->pfFlags & PVRTEX_PIXELTYPE;
		switch(pvr_pixel_type)
		{
		case OGL_RGBA_4444:
			pixel_format = PF_RGBA;
			data_type = DT_UNSIGNED_SHORT_4_4_4_4;      
			break;
		case OGL_RGBA_5551:
			pixel_format = PF_RGBA;
			data_type = DT_UNSIGNED_SHORT_5_5_5_1;
			break;
		case OGL_RGBA_8888:
			pixel_format = PF_RGBA;
			data_type = DT_UNSIGNED_BYTE;
			break;
		case OGL_RGB_565:
			pixel_format = PF_RGB;
			data_type = DT_UNSIGNED_SHORT_5_6_5;
			break;
		case OGL_RGB_888:
			pixel_format = PF_RGB;
			data_type = DT_UNSIGNED_BYTE;
			break;
		case OGL_I_8:
			pixel_format = PF_L;
			data_type = DT_UNSIGNED_BYTE;
			break;
		case OGL_AI_88:
			pixel_format = PF_LA;
			data_type = DT_UNSIGNED_BYTE;
			break;
		case OGL_A_8:
			pixel_format= PF_ALPHA;
			data_type = DT_UNSIGNED_BYTE;
			break;
		case MGLPT_PVRTC2:
		case OGL_PVRTC2:
			pixel_format = pvr_header->AlphaBitMask==0 ? PF_RGB_PVRTC_2BPPV1_IMG : PF_RGBA_PVRTC_2BPPV1_IMG ;	// PVRTC2
			break;
		case MGLPT_PVRTC4:
		case OGL_PVRTC4:
			pixel_format = pvr_header->AlphaBitMask==0 ? PF_RGB_PVRTC_4BPPV1_IMG : PF_RGBA_PVRTC_4BPPV1_IMG ;	// PVRTC4
			break;    
		default:  // NOT SUPPORTED
			GAssert("LoadPVR failed: pixel type not supported.\n");
			return NULL;
		}

		// the total of the pixel size
		U32 total_size = 0;
		U32 mipmap_count = (pvr_header->pfFlags & PVRTEX_MIPMAP)? pvr_header->MipMapCount : 0;

		// take care of mipmaps if any.
		for(U32 i = 0; i < num_surfs; i++)
		{
			U32 w = width, h = height;
			for(U32 level = 0; level <= mipmap_count; level++)
			{
				if(w == 0) w = 1;
				if(h == 0) h = 1;

				// calculate how many bytes this mip level occupies
				if((pvr_header->pfFlags & PVRTEX_PIXELTYPE)==OGL_PVRTC2)
				{
					total_size += (MAX(w, PVRTC2_MIN_TEXWIDTH) * MAX(h, PVRTC2_MIN_TEXHEIGHT) * pixel_size) / 8;
				}
				else if((pvr_header->pfFlags & PVRTEX_PIXELTYPE)==OGL_PVRTC4)
				{
					total_size += (MAX(w, PVRTC4_MIN_TEXWIDTH) * MAX(h, PVRTC4_MIN_TEXHEIGHT) * pixel_size) / 8;
				}
				else
				{
					total_size += w * h * pixel_size / 8;
				}
				w >>= 1; h >>= 1;
			}
		}		

		// build the image
		Image* image = GNEW(Image); CHECK(image);
		image->Type(num_surfs==1 ? IMAGE_NORMAL : IMAGE_CUBEMAP);
		image->Width(width);
		image->Height(height);
		image->BitCount(pixel_size);
		image->PixelFormat(pixel_format);
		image->DataType(data_type);
		image->MipmapCount(mipmap_count);
		image->Mipmap(data_ptr+data_pos,total_size);
		return image;
	}

	//////////////////////////////////////////////////////////////////////////

	/*
	====================
	Load

	load tga and pvr file
	====================
	*/
	Image* Image::Load(const GData* data)
	{
		CHECK(data);

		// check the magic number identifying
		if(data->Size() > sizeof(PVR_Texture_Header) && ((PVR_Texture_Header*)data->Ptr())->PVR == PVRTEX_IDENTIFIER)
		{
			return LoadPVR(data);
		}
		else if(*(U32*)data->Ptr() == MAKEFOURCC('D','D','S',' '))
		{
			return LoadDDS(data);
		}
		else
		{
			return LoadTGA(data);
		}

		return NULL;
	}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
