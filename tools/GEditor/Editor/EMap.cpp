//////////////////////////////////////////////////////////////////////////

#include "Editor.h"

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

EMap::EMap()
{

}

EMap::~EMap()
{

}

//////////////////////////////////////////////////////////////////////////	

/*
====================
Load
====================
*/
VOID EMap::Load(const GData* data)
{
	GUARD(Map::Load);

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
	for(I32 j = 0; j < mHeight; j++)
	{
		for(I32 i = -mWidth/2; i < mWidth/2; i++)
		{
			std::pair<I32,I32> coord(i,j);
			ChunkPtr chunk = GNEW(EChunk); CHECK(chunk);
			chunk->Load(data_ptr);
			mChunks.insert(std::make_pair(coord,chunk));
		}
	}

	// build the reference line
	// I32 offset = mWidth*U2P(CHUNK_STRIDE)/2;
	mLines.clear();
	Line* line_ptr = GNEW(Line); CHECK(line_ptr);
	U32 grid_width = mWidth*U2P(CHUNK_STRIDE)/TILE_WIDTH;
	U32 grid_height = mHeight*U2P(CHUNK_STRIDE)/TILE_HEIGHT;
	for(I32 j = 0; j < grid_width; j++)
	{
		for(I32 i = 0; i < grid_width; i++)
		{
			I32 px, py; T2P(px, py, i, j);
			line_ptr->add(Vector3(P2U(px),				P2U(py),				0.0f));
			line_ptr->add(Vector3(P2U(px+TILE_WIDTH/2),	P2U(py+TILE_HEIGHT/2),	0.0f));
			line_ptr->add(Vector3(P2U(px),				P2U(py+TILE_HEIGHT),	0.0f));
			line_ptr->add(Vector3(P2U(px-TILE_WIDTH/2),	P2U(py+TILE_HEIGHT/2),	0.0f));
			U32 start_vertex = (i + j * grid_width)*4;
			line_ptr->add(start_vertex+0); line_ptr->add(start_vertex+1);
			line_ptr->add(start_vertex+1); line_ptr->add(start_vertex+2);
			line_ptr->add(start_vertex+2); line_ptr->add(start_vertex+3);
			line_ptr->add(start_vertex+3); line_ptr->add(start_vertex+0);			
		}
	}
	line_ptr->color(Vector4(0,1,0,1));
	line_ptr->build();
	mLines.push_back(line_ptr);

	UNGUARD;
}

//////////////////////////////////////////////////////////////////////////

/*
====================
Draw
====================
*/
VOID EMap::DrawReferenceLine(Graph *gc)
{
	GUARD(EMap::DrawReferenceLine);

	// draw reference line
	for(std::list<LinePtr>::iterator it = mLines.begin(); it != mLines.end(); ++it)
	{
		(*it)->draw(gc);
	}

	UNGUARD;
}

//////////////////////////////////////////////////////////////////////////