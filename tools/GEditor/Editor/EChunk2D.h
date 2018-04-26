/*
 *  EChunk2D.h
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __EChunk2D__
#define __EChunk2D__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

// this class provides an object-oriented EChunk2D interface.
class EChunk2D : public Chunk2D
{
public:
	// constructor
	EChunk2D();

	// destructor
	virtual ~EChunk2D();

	// load the data
	VOID Load(const VOID* data);

	// draw the chunk
	VOID Draw(Graph *gc);

	// update the chunk
	VOID Update();

	// save the chunk
	VOID Save(GFile& file);

public:
	struct Layer
	{
		Str name;
		Vector2 st;
		std::vector<U8>alpha;
		BaseTexturePtr texture;
		PrimitivePtr primitive;
		I32 total;
	};

public:
	// find a layer
	U32 Find(const CHAR* name, const Vector2& st);

public:
	std::vector<U8>mMask;
	std::vector<Layer>mLayers;

private:
	VertexBufferPtr mVBPtr;
	IndexBufferPtr mIBPtr;
	KeyPtr mShaderKey;

private:friend class EMap2D;
};

//////////////////////////////////////////////////////////////////////////

typedef GPtr<class EChunk2D>EChunk2DPtr;

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __EChunk2D__

//////////////////////////////////////////////////////////////////////////