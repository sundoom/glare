//////////////////////////////////////////////////////////////////////////

#include "Editor.h"

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

EMap2D::EMap2D()
{

}

EMap2D::~EMap2D()
{

}

//////////////////////////////////////////////////////////////////////////	

/*
====================
Load
====================
*/
VOID EMap2D::Load(const GData* data)
{
	GUARD(EMap2D::Load);

	CHECK(data);
	U8*data_ptr = (U8*)data->Ptr();

	// check the map header
	CHECK(*(U32*)data_ptr == (MAKEFOURCC('G','M','A','P')));
	data_ptr += sizeof(U32);

	// get the stride of the map
	mWidth = *(U32*)data_ptr;
	data_ptr += sizeof(U32);
	mHeight = *(U32*)data_ptr;
	data_ptr += sizeof(U32);

	// load all of the chunks
	for(U32 j = 0; j < mHeight; j++)
	{
		for(U32 i = 0; i < mWidth; i++)
		{
			Chunk2DPtr chunk = GNEW(EChunk2D); CHECK(chunk);
			chunk->Load(data_ptr);
			mChunks.push_back(chunk);
		}
	}
/*
	// build the reference line
	I32 offset = mWidth*U2P(CHUNK_STRIDE)/2;
	if(mReferenceLine) mReferenceLine.Release();
	mReferenceLine = GNEW(Line); CHECK(mReferenceLine);
	U32 grid_width = mWidth*U2P(CHUNK_STRIDE)/TILE_WIDTH;
	U32 grid_height =mHeight*U2P(CHUNK_STRIDE)/TILE_HEIGHT;
	for(I32 j = 0; j < grid_width; j++)
	{
		for(I32 i = 0; i < grid_width; i++)
		{
			I32 px, py; T2P(px, py, i, j);
			mReferenceLine->add(Vector3(P2U(px),					P2U(py),				0.0f));
			mReferenceLine->add(Vector3(P2U(px+TILE_WIDTH/2),	P2U(py+TILE_HEIGHT/2),	0.0f));
			mReferenceLine->add(Vector3(P2U(px),					P2U(py+TILE_HEIGHT),	0.0f));
			mReferenceLine->add(Vector3(P2U(px-TILE_WIDTH/2),	P2U(py+TILE_HEIGHT/2),	0.0f));
			U32 start_vertex = (i + j * grid_width)*4;
			mReferenceLine->add(start_vertex+0); mReferenceLine->add(start_vertex+1);
			mReferenceLine->add(start_vertex+1); mReferenceLine->add(start_vertex+2);
			mReferenceLine->add(start_vertex+2); mReferenceLine->add(start_vertex+3);
			mReferenceLine->add(start_vertex+3); mReferenceLine->add(start_vertex+0);			
		}
	}
	mReferenceLine->color(Vector4(0,1,0,1));
	mReferenceLine->build();
*/
	UNGUARD;
}

//////////////////////////////////////////////////////////////////////////

/*
====================
update
====================
*/
VOID EMap2D::Update(const Vector4& viewport)
{
	GUARD(Map::Update);

	Rect src, dst, clip;

	// compute the view
	src.left	= U2P(viewport[0])/U2P(CHUNK_STRIDE);
	src.top		= U2P(viewport[1])/U2P(CHUNK_STRIDE);
	src.right	= (U2P(viewport[2])%U2P(CHUNK_STRIDE)) ? (U2P(viewport[2])/U2P(CHUNK_STRIDE)+1) : U2P(viewport[2])/U2P(CHUNK_STRIDE);
	src.bottom	= (U2P(viewport[3])%U2P(CHUNK_STRIDE)) ? (U2P(viewport[3])/U2P(CHUNK_STRIDE)+1) : U2P(viewport[3])/U2P(CHUNK_STRIDE);

	dst.left	= 0;
	dst.top		= 0;
	dst.right	= mWidth;
	dst.bottom	= mHeight;	

	IntersectRect(&clip, &dst, &src);

	// update all of the chunk		
	if(mViewport[0]!=clip.left || mViewport[1]!=clip.top || mViewport[2]!=clip.right || mViewport[3]!=clip.bottom)
	{
		// store the view rect
		mViewport[0] = clip.left;
		mViewport[1] = clip.top;
		mViewport[2] = clip.right;
		mViewport[3] = clip.bottom;

		// build the reference line
		mLines.clear();
		for(U32 j = mViewport[1]; j < mViewport[3]; j++)
		{
			for(U32 i = mViewport[0]; i < mViewport[2]; i++)
			{
				I32 ox, oy; T2P(ox, oy, i, j);
				I32 w = U2P(CHUNK_STRIDE)/TILE_WIDTH;
				I32 h = U2P(CHUNK_STRIDE/2)/TILE_HEIGHT;
				Line* line_ptr = GNEW(Line); CHECK(line_ptr);
				for(U32 y = 0; y < h; y++)
				{
					for(U32 x = 0; x < w; x++)
					{
						I32 px, py; T2P(px, py, x, y);
						line_ptr->add(Vector3(P2U(ox+px),				P2U(oy+py),					0.0f));
						line_ptr->add(Vector3(P2U(ox+px+TILE_WIDTH/2),	P2U(oy+py+TILE_HEIGHT/2),	0.0f));
						line_ptr->add(Vector3(P2U(ox+px),				P2U(oy+py+TILE_HEIGHT),		0.0f));
						line_ptr->add(Vector3(P2U(ox+px-TILE_WIDTH/2),	P2U(oy+py+TILE_HEIGHT/2),	0.0f));
						U32 start_vertex = (x + y * w)*4;
						line_ptr->add(start_vertex+0); line_ptr->add(start_vertex+1);
						line_ptr->add(start_vertex+1); line_ptr->add(start_vertex+2);
						line_ptr->add(start_vertex+2); line_ptr->add(start_vertex+3);
						line_ptr->add(start_vertex+3); line_ptr->add(start_vertex+0);
					}
				}
				line_ptr->color(Vector4(0,1,0,1));
				line_ptr->build();
				mLines.push_back(line_ptr);
			}
		}
	}

	UNGUARD;
}

//////////////////////////////////////////////////////////////////////////

/*
====================
Draw
====================
*/
VOID EMap2D::DrawReferenceLine(Graph *gc)
{
	GUARD(EMap2D::DrawReferenceLine);

	// draw reference line
	// if(mReferenceLine)mReferenceLine->draw(gc);
	for(std::list<LinePtr>::iterator it = mLines.begin(); it != mLines.end(); ++it)
	{
		(*it)->draw(gc);
	}

	UNGUARD;
}

//////////////////////////////////////////////////////////////////////////