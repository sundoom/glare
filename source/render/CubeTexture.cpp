/*
 *  CubeTexture.cpp
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#include "CubeTexture.h"
#include "State.h"

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Render)

//////////////////////////////////////////////////////////////////////////		

	CubeTexture::CubeTexture()
	{
		mTarget = GL_TEXTURE_CUBE_MAP;
	}

	CubeTexture::~CubeTexture() 
	{

	}

	//////////////////////////////////////////////////////////////////////////

  /*
	====================
	Load
	====================
	*/
	VOID CubeTexture::Load(U32 level, U32 width, U32 height, U32 bits, U32 format, U32 type, const U8* pixels)
	{
		// store the info
		mWidth = width;
		mHeight = height;
		mFormat = format;

		// generate the texture object
		CHECK(mObject == 0);
		glGenTextures(1, &mObject); CHECK(mObject);

		// bind the texture
		BindTexture(mTarget, mObject);

		// commit the pixel data to the gpu memory
		for(U32 j = 0; j < 6; j++)
		{
			width = mWidth; height = mHeight;
			for(U32 i = 0; i <= level; i++)
			{
				if(width == 0) width = 1;
				if(height == 0) height = 1;
				U32 pixel_size = 0;
				switch(format)
				{
				case PF_ALPHA:
					pixel_size = width * height * bits / 8;
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+j, i, format, width, height, 0, format, type,  pixels);
					break;
				case PF_RGB:
					pixel_size = width * height * bits / 8;
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+j, i, format, width, height, 0, format, type,  pixels);
					break;
				case PF_RGBA:
					pixel_size = width * height * bits / 8;
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+j, i, format, width, height, 0, format, type,  pixels);
					break;
				case PF_L:
					pixel_size = width * height * bits / 8;
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+j, i, format, width, height, 0, format, type,  pixels);
					break;
				case PF_LA:
					pixel_size = width * height * bits / 8;
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+j, i, format, width, height, 0, format, type,  pixels);
					break;
				case PF_RGB_DXT1:
					pixel_size = (((width+3)/4) * ((height+3)/4) * 8);
					glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+j, i, format, width, height, 0, pixel_size, pixels);
					break;
				case PF_RGBA_DXT1:
					pixel_size = (((width+3)/4) * ((height+3)/4) * 8);
					glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+j, i, format, width, height, 0, pixel_size, pixels);
					break;
				case PF_RGBA_DXT3:      
					pixel_size = (((width+3)/4) * ((height+3)/4) * 16);
					glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+j, i, format, width, height, 0, pixel_size, pixels);
					break;
				case PF_RGBA_DXT5:
					pixel_size = (((width+3)/4) * ((height+3)/4) * 16);
					glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+j, i, format, width, height, 0, pixel_size, pixels);
					break;
				case PF_RGB_PVRTC_2BPPV1_IMG:
					pixel_size = (MAX(width, 16) * MAX(height, 8) * bits) / 8;
					glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+j, i, format, width, height, 0, pixel_size, pixels);
					break;
				case PF_RGBA_PVRTC_2BPPV1_IMG:
					pixel_size = (MAX(width, 16) * MAX(height, 8) * bits) / 8;
					glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+j, i, format, width, height, 0, pixel_size, pixels);
					break;
				case PF_RGB_PVRTC_4BPPV1_IMG:
					pixel_size = (MAX(width, 8) * MAX(height, 8) * bits) / 8;
					glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+j, i, format, width, height, 0, pixel_size, pixels);
					break;
				case PF_RGBA_PVRTC_4BPPV1_IMG:
					pixel_size = (MAX(width, 8) * MAX(height, 8) * bits) / 8;
					glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+j, i, format, width, height, 0, pixel_size, pixels);
					break;
				default:
					GAssert(VA("The texture`s format(%d) is unknown.\n", format));
					break;
				}
				pixels += pixel_size;
				width >>= 1; height >>= 1;
			}
		}
	}

	/*
	====================
	Update
	====================
	*/
	VOID CubeTexture::Update(U32 level, I32 x, I32 y, U32 width, U32 height, U32 bits, U32 format, U32 type, const U8* pixels)
	{
		// bind the texture
		CHECK(mObject);
		BindTexture(mTarget, mObject);

		// commit the pixel data to the gpu memory
		for(U32 j = 0; j < 6; j++)
		{
			width = mWidth; height = mHeight;
			for(U32 i = 0; i <= level; i++)
			{
				if(width == 0) width = 1;
				if(height == 0) height = 1;
				U32 pixel_size = 0;
				switch(format)
				{
				case PF_ALPHA:
					pixel_size = width * height * bits / 8;
					glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+j, i, x, y, width, height, format, type, pixels);
					break;
				case PF_RGB:
					pixel_size = width * height * bits / 8;
					glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+j, i, x, y, width, height, format, type, pixels);
					break;
				case PF_RGBA:
					pixel_size = width * height * bits / 8;
					glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+j, i, x, y, width, height, format, type, pixels);
					break;
				case PF_L:
					pixel_size = width * height * bits / 8;
					glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+j, i, x, y, width, height, format, type, pixels);
					break;
				case PF_LA:
					pixel_size = width * height * bits / 8;
					glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+j, i, x, y, width, height, format, type, pixels);
					break;
				case PF_RGB_DXT1:
					pixel_size = (((width+3)/4) * ((height+3)/4) * 8);
					glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+j, i, x, y, width, height, format, pixel_size, pixels);
					break;
				case PF_RGBA_DXT1:
					pixel_size = (((width+3)/4) * ((height+3)/4) * 8);
					glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+j, i, x, y, width, height, format, pixel_size, pixels);
					break;
				case PF_RGBA_DXT3:
					pixel_size = (((width+3)/4) * ((height+3)/4) * 16);
					glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+j, i, x, y, width, height, format, pixel_size, pixels);
					break;
				case PF_RGBA_DXT5:
					pixel_size = (((width+3)/4) * ((height+3)/4) * 16);
					glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+j, i, x, y, width, height, format, pixel_size, pixels);
					break;
				case PF_RGB_PVRTC_2BPPV1_IMG:
					pixel_size = (MAX(width, 16) * MAX(height, 8) * bits) / 8;
					glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+j, i, x, y, width, height, format, pixel_size, pixels);
					break;
				case PF_RGBA_PVRTC_2BPPV1_IMG:
					pixel_size = (MAX(width, 16) * MAX(height, 8) * bits) / 8;
					glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+j, i, x, y, width, height, format, pixel_size, pixels);
					break;
				case PF_RGB_PVRTC_4BPPV1_IMG:
					pixel_size = (MAX(width, 8) * MAX(height, 8) * bits) / 8;
					glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+j, i, x, y, width, height, format, pixel_size, pixels);
					break;
				case PF_RGBA_PVRTC_4BPPV1_IMG:
					pixel_size = (MAX(width, 8) * MAX(height, 8) * bits) / 8;
					glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+j, i, x, y, width, height, format, pixel_size, pixels);
					break;
				default:
					GAssert(VA("The texture`s format(%d) is unknown.\n", format));
					break;
				}
				pixels += pixel_size;
				width >>= 1; height >>= 1;
			}
		}
	}
	
//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
