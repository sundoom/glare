//////////////////////////////////////////////////////////////////////////

#include "Editor.h"

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

EChunk2D::EChunk2D()
{
	
}

EChunk2D::~EChunk2D()
{
	
}

//////////////////////////////////////////////////////////////////////////

/*
====================
Load
====================
*/
VOID EChunk2D::Load(const VOID* data)
{
	GUARD(EChunk2D::Load);

	// build the vertex buffer
	mVBPtr = GNEW(VertexBuffer); CHECK(mVBPtr);
	{
		DVT vertexes[] = 
		{
			{ 0.0,				0.0,				0.0,	0.5,	1.0},
			{ CHUNK_STRIDE/2,	CHUNK_STRIDE/4,		0.0,	1.0,	0.5},
			{ 0.0,				CHUNK_STRIDE/2,		0.0,	0.5,	0.0},
			{-CHUNK_STRIDE/2,	CHUNK_STRIDE/4,		0.0,	0.0,	0.5},
		};

		GDataPtr vd_ptr = GNEW(GData); CHECK(vd_ptr);
		vd_ptr->Size(3*sizeof(U32) + 3*sizeof(U8) + sizeof(vertexes));
		U8*data_ptr = (U8*)vd_ptr->Ptr();
		*(U32*)data_ptr = MAKEFOURCC('G','V','B','O');
		data_ptr += sizeof(U32);
		*(U32*)data_ptr = sizeof(vertexes)/sizeof(DVT); 
		data_ptr += sizeof(U32);
		*(U32*)data_ptr = sizeof(DVT); 
		data_ptr += sizeof(U32);
		*(U8*)data_ptr = 2;
		data_ptr += sizeof(U8);
		*(U8*)data_ptr = VertexBuffer::VT_3F;
		data_ptr += sizeof(U8);
		*(U8*)data_ptr = VertexBuffer::VT_2F;
		data_ptr += sizeof(U8);
		::memcpy(data_ptr, vertexes, sizeof(vertexes));
		data_ptr += sizeof(vertexes);
		mVBPtr->Load(vd_ptr.Ptr());
	}		

	// build the index
	mIBPtr = GNEW(IndexBuffer); CHECK(mIBPtr);
	{
		const U16 indexes[] = { 3, 0, 2, 2, 0, 1 };
		GDataPtr id_ptr = GNEW(GData); CHECK(id_ptr);
		id_ptr->Size(3*sizeof(U32) + sizeof(indexes));
		U8*data_ptr = (U8*)id_ptr->Ptr();
		*(U32*)data_ptr = MAKEFOURCC('G','I','B','O');
		data_ptr += sizeof(U32);
		*(U32*)data_ptr = sizeof(indexes)/sizeof(U16); 
		data_ptr += sizeof(U32);
		*(U32*)data_ptr = sizeof(U16); 
		data_ptr += sizeof(U32);
		::memcpy(data_ptr, indexes, sizeof(indexes));
		data_ptr += sizeof(indexes);
		mIBPtr->Load(id_ptr.Ptr());
	}

	// build the shaders
	Str shader_name = "shader/chunk.xml";
	mShaderKey = Key::Find(shader_name.c_str());
	if(mShaderKey == NULL)
	{
		Shader*shader = GNEW(Shader); CHECK(shader);
		shader->Load(GLoad(shader_name.c_str()));
		mShaderKey = GNEW(Key(shader_name.c_str(), shader)); CHECK(mShaderKey);
	}

	// load the data
	U8*data_ptr = (U8*)data;

	// check the chunk header		
	CHECK(*(U32*)data_ptr == (MAKEFOURCC('G','C','H','K')));
	data_ptr += sizeof(U32);

	// get the layer
	U32 num_layer = *(U32*)data_ptr;
	data_ptr += sizeof(U32);
	for(U32 k = 0; k < num_layer; k++)
	{
		Layer layer;
		layer.total = 0;

		// get the primitive
		layer.primitive = GNEW(Primitive); CHECK(layer.primitive);
		layer.primitive->SetType(Primitive::PT_TRIANGLES);
		layer.primitive->SetVertexBuffer(mVBPtr.Ptr());
		layer.primitive->SetIndexBuffer(mIBPtr.Ptr());	
		layer.primitive->SetConstant("gWVP",GNEW(Constant(Matrix())));

		// get the color texture			
		Str texture_name;
		U32 len = *(U32*)data_ptr;
		data_ptr += sizeof(U32);
		texture_name.resize(len, 0);
		memcpy(&texture_name[0], data_ptr, len*sizeof(CHAR));
		data_ptr += len*sizeof(CHAR);
		KeyPtr texture_key_ptr = Key::Find(texture_name.c_str());
		if(texture_key_ptr == NULL)
		{
			const Image* image = Image::Load(GLoad(texture_name.c_str())); CHECK(image);
			BaseTexture* base_texture = GNEW(Texture); CHECK(base_texture);
			base_texture->Load(image);
			texture_key_ptr = GNEW(Key(texture_name.c_str(), base_texture)); CHECK(texture_key_ptr);
		}
		mKeys.push_back(texture_key_ptr);
		layer.primitive->SetConstant("gColorTex",GNEW(Constant((BaseTexture*)texture_key_ptr->Ptr())));
		layer.name = texture_name;

		// get the scale st
		F32 s = *(F32*)data_ptr;
		data_ptr += sizeof(F32);
		F32 t = *(F32*)data_ptr;
		data_ptr += sizeof(F32);
		layer.primitive->SetConstant("gScaleST",GNEW(Constant(Vector4(s,t,0,0))));
		layer.st.set(s, t);

		// get the alpha
		U32 width = *(U32*)data_ptr;
		data_ptr += sizeof(U32);
		U32 height = *(U32*)data_ptr;
		data_ptr += sizeof(U32);
		if(width==U2P(ALPHA_STRIDE) && height==U2P(ALPHA_STRIDE))
		{
			layer.alpha.resize(width*height*sizeof(U8));
			::memcpy(&layer.alpha[0], data_ptr, width*height*sizeof(U8));
			data_ptr += width*height*sizeof(U8);
		}
		else
		{
			U8 alpha = *(U8*)data_ptr;
			data_ptr += sizeof(U8);
			width = U2P(ALPHA_STRIDE);
			height = U2P(ALPHA_STRIDE);
			layer.alpha.resize(width*height*sizeof(U8),alpha);
		}
		if(k==0)
		{
			mMask.resize(width*height);
			for(U32 j = 0; j < height; j++)
			{
				for( U32 i = 0; i < width; i++)
				{
					U8& alpha = layer.alpha[i+j*width];
					U8& mask = mMask[i+j*width];
					if(alpha <= 127) 
					{
						alpha = (U8)((F32)alpha/127.0f*255.0f);
						mask = 0;
					}
					else 
					{
						alpha = (U8)((F32)(alpha-128)/127.0f*255.0f);
						mask = 1;
					}
					layer.total += alpha;
				}
			}
		}
		else
		{
			for(U32 j = 0; j < height; j++)
			{
				for( U32 i = 0; i < width; i++)
				{
					layer.total += layer.alpha[i+j*width];
				}
			}
		}
		BaseTexture *alpha_texture = GNEW(Texture); CHECK(alpha_texture);
		layer.primitive->SetConstant("gAlphaTex",GNEW(Constant(alpha_texture)));
		layer.texture = alpha_texture;

		// load the shader
		if(k==0) layer.primitive->SetShader(dynamic_cast<Shader*>(mShaderKey->Ptr()), "base");
		else layer.primitive->SetShader(dynamic_cast<Shader*>(mShaderKey->Ptr()), "layer");

		// add the layer to the table
		mLayers.push_back(layer);
	}

	// update the layer
	Update();

	UNGUARD;
}

//////////////////////////////////////////////////////////////////////////

/*
====================
Draw
====================
*/
VOID EChunk2D::Draw(Graph *gc)
{
	GUARD(EChunk2D::Draw);

	// get the frustum
	Frustum frustum(gc->wvp());
	BoundingBox box(0.0,0.0,0.0,CHUNK_STRIDE,CHUNK_STRIDE,1.0);
	if(!frustum.boxInFrustum(box.min(),box.max())) return;

	// draw all of the layer
	for(U32 k = 0; k < mLayers.size(); k++)
	{
		Layer& layer = mLayers[k];
		layer.primitive->GetConstant("gWVP")->SetMatrix(gc->wvp());
		gc->draw(layer.primitive.Ptr());
	}
	
	UNGUARD;
}

//////////////////////////////////////////////////////////////////////////

/*
====================
Update
====================
*/
VOID EChunk2D::Update()
{
	GUARD(EChunk2D::Update);

	// erase the layer that is invalid
	std::vector<Layer>::iterator it = mLayers.begin();
	while(it != mLayers.end())
	{
		Layer& layer = (*it);
		if(layer.total==0)
		{
			it = mLayers.erase(it);
		}
		else
		{
			++it;
		}
	}

	// update the alpha texture
	U32 width = U2P(ALPHA_STRIDE);
	U32 height = U2P(ALPHA_STRIDE);
	for(I32 k = 0; k < mLayers.size(); k++)
	{
		Layer& layer = mLayers[k];
		Image* image = GNEW(Image); CHECK(image);
		image->Width(width);
		image->Height(height);
		image->PixelFormat(PF_ALPHA);
		image->DataType(DT_UNSIGNED_BYTE);
		image->MipmapCount(1);
		if(k==0)
		{
			image->Mipmap(0,NULL,width*height*sizeof(U8));
			U8* data = (U8*)image->Mipmap(0);
			for(U32 j = 0; j < height; j++)
			{
				for( U32 i = 0; i < width; i++)
				{
					U8& alpha = layer.alpha[i+j*width];
					U8& mask = mMask[i+j*width];
					if(mask == 0) data[i+j*width] = (U8)(((F32)alpha)/255.0f*127.0f);
					else data[i+j*width] = (U8)(((F32)alpha)/255.0f*127.0f)+128;
				}
			}
			layer.texture->Load(image);
			layer.primitive->SetShader(dynamic_cast<Shader*>(mShaderKey->Ptr()), "base");
		}
		else
		{
			image->Mipmap(0,&layer.alpha[0],width*height*sizeof(U8));
			layer.texture->Load(image);
			layer.primitive->SetShader(dynamic_cast<Shader*>(mShaderKey->Ptr()), "layer");
		}
	}

	UNGUARD;
}

//////////////////////////////////////////////////////////////////////////
/*
====================
Find
====================
*/
U32 EChunk2D::Find(const CHAR* name, const Vector2& st)
{
	GUARD(EChunk2D::Find);	

	CHECK(name);
	
	// searche the layer table
	for(U32 i = 0; i < mLayers.size(); i++)
	{
		if(mLayers[i].name == name && mLayers[i].st == st) return i;
	}

	// create the new layer
	Layer layer;
	layer.name = name;
	layer.st = st;
	layer.alpha.resize(U2P(ALPHA_STRIDE)*U2P(ALPHA_STRIDE), 0);
	layer.total = 0;

	// create the new primitive
	layer.primitive = GNEW(Primitive); CHECK(layer.primitive);
	layer.primitive->SetType(Primitive::PT_TRIANGLES);
	layer.primitive->SetVertexBuffer(mVBPtr.Ptr());
	layer.primitive->SetIndexBuffer(mIBPtr.Ptr());	
	layer.primitive->SetConstant("gWVP",GNEW(Constant(Matrix())));

	// get the color texture
	KeyPtr texture_key_ptr = Key::Find(name);
	if(texture_key_ptr == NULL)
	{
		const Image* image = Image::Load(GLoad(name)); CHECK(image);
		BaseTexture* base_texture = GNEW(Texture); CHECK(base_texture);
		base_texture->Load(image);
		texture_key_ptr = GNEW(Key(name, base_texture)); CHECK(texture_key_ptr);
	}
	mKeys.push_back(texture_key_ptr);
	layer.primitive->SetConstant("gColorTex",GNEW(Constant((BaseTexture*)texture_key_ptr->Ptr())));

	// set the scale st	
	layer.primitive->SetConstant("gScaleST",GNEW(Constant(Vector4(st[0],st[1],0,0))));	

	// set the alpha
	U32 width = U2P(ALPHA_STRIDE);
	U32 height = U2P(ALPHA_STRIDE);
	Image* image = GNEW(Image); CHECK(image);
	image->Width(width);
	image->Height(height);
	image->PixelFormat(PF_ALPHA);
	image->DataType(DT_UNSIGNED_BYTE);
	image->MipmapCount(1);
	image->Mipmap(0,&layer.alpha[0],width*height*sizeof(U8));		
	BaseTexture *alpha_texture = GNEW(Texture); CHECK(alpha_texture);
	alpha_texture->Load(image);
	layer.primitive->SetConstant("gAlphaTex",GNEW(Constant(alpha_texture)));
	layer.texture = alpha_texture;

	// load the shader
	layer.primitive->SetShader(dynamic_cast<Shader*>(mShaderKey->Ptr()), "layer");
	
	// add the layer to the table
	mLayers.push_back(layer);
	return mLayers.size() - 1;

	UNGUARD;
}

//////////////////////////////////////////////////////////////////////////
/*
====================
Save
====================
*/
VOID EChunk2D::Save(GFile& file)
{
	GUARD(EChunk2D::Save);

	// write the chunk header
	U32 header = MAKEFOURCC('G','C','H','K');
	file.Write(&header, sizeof(U32));

	// write the number of layers
	U32 num_layer = mLayers.size();
	file.Write(&num_layer, sizeof(U32));

	// write all of the layers
	if(mLayers.size() == 1)
	{
		Layer& layer = mLayers[0];

		// write the texture
		U32 name_len = layer.name.size();
		file.Write(&name_len, sizeof(U32));
		file.Write(layer.name.c_str(), name_len*sizeof(CHAR));
		file.Write(layer.st.ptr(), 2*sizeof(F32));

		// write the alpha
		U32 width = 1;
		file.Write(&width, sizeof(U32));
		U32 height = 1;
		file.Write(&height, sizeof(U32));
		U8 alpha = 127;
		file.Write(&alpha, sizeof(U8));
	}
	else
	{
		for(I32 k = 0; k < mLayers.size(); k++)
		{
			Layer& layer = mLayers[k];

			// write the texture
			U32 name_len = layer.name.size();
			file.Write(&name_len, sizeof(U32));
			file.Write(layer.name.c_str(), name_len*sizeof(CHAR));
			file.Write(layer.st.ptr(), 2*sizeof(F32));

			// write the alpha
			U32 width = U2P(ALPHA_STRIDE);
			file.Write(&width, sizeof(U32));
			U32 height = U2P(ALPHA_STRIDE);
			file.Write(&height, sizeof(U32));
			if(k==0)
			{
				std::vector<U8>data(width*height);
				for(U32 j = 0; j < height; j++)
				{
					for( U32 i = 0; i < width; i++)
					{
						U8& alpha = layer.alpha[i+j*width];
						U8& mask = mMask[i+j*width];
						if(mask == 0) data[i+j*width] = (U8)(((F32)alpha)/255.0f*127.0f);
						else data[i+j*width] = (U8)(((F32)alpha)/255.0f*127.0f)+128;
					}
				}
				file.Write(&data[0], width*height*sizeof(U8));
			}
			else
			{
				file.Write(&layer.alpha[0], width*height*sizeof(U8));
			}
		}
	}	

	UNGUARD;
}

//////////////////////////////////////////////////////////////////////////