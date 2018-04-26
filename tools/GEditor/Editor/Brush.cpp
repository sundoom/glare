//////////////////////////////////////////////////////////////////////////

#include "Editor.h"

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

Brush::Brush(const Map* map)
{
	mName = "map/grass.tga";	
	mScale.set(1.0f,1.0f);
	mRadius = 1.0f;
	mSoftness = 1.0f;
	mStrength = 1.0f;
	mMapPtr = (Map*)map;
}

Brush::~Brush()
{
	
}

/*
====================
Name
====================
*/
VOID Brush::Name(const CHAR*name)
{
	CHECK(name);
	mName = name;
}

const CHAR* Brush::Name() const
{
	return mName.c_str();
}

/*
====================
Scale
====================
*/
VOID Brush::Scale(const Vector2& s)
{
	mScale = s;
}

const Vector2& Brush::Scale() const
{
	return mScale;
}

/*
====================
Radius
====================
*/
VOID Brush::Radius(F32 r)
{
	mRadius = r;
	mRadius = GMath::clamp(mRadius, 100.0f, 1.0f);
}

F32 Brush::Radius() const
{
	return mRadius;
}

/*
====================
Softness
====================
*/
VOID Brush::Softness(F32 s)
{
	mSoftness = s;
	mSoftness = GMath::clamp(mSoftness, 1.0f, 0.1f);
}

F32 Brush::Softness() const
{
	return mSoftness;
}

/*
====================
Strength
====================
*/
VOID Brush::Strength(F32 s)
{
	mStrength = s;
	mStrength = GMath::clamp(mStrength, 1.0f, 0.0f);
}

F32 Brush::Strength() const
{
	return mStrength;
}

/*
====================
DoMove
====================
*/
VOID Brush::DoMove(const Vector3& pt)
{
	mPos = pt;
}

//////////////////////////////////////////////////////////////////////////
// BrushCircle
//////////////////////////////////////////////////////////////////////////

BrushCircle::BrushCircle(const Map* map) : Brush(map)
{
	mLinePtr = GNEW(Line);
	if(mLinePtr)
	{
		#define CYLINDER_SEGMENTS 20
		F32 angle_delta = 2.0f*PI/(F32)CYLINDER_SEGMENTS;
		F32 angle = 0.0f;
		for(U32 i=0; i<CYLINDER_SEGMENTS; ++i,angle+=angle_delta)
		{
			F32 c = GMath::cos(angle);
			F32 s = GMath::sin(angle);
			Vector3 v(c, s, 0.0f);
			v.normalize();
			mLinePtr->add(v);
			mLinePtr->add(i%CYLINDER_SEGMENTS);
			mLinePtr->add((i+1)%CYLINDER_SEGMENTS);
		}
		mLinePtr->color(Vector4(1,1,0,1));
		mLinePtr->build();
	}
}

BrushCircle::~BrushCircle()
{

}

/*
====================
DoDrag
====================
*/
VOID BrushCircle::DoDrag(const Vector3& pt)
{
	GUARD(BrushCircle::DoDrag);

	mPos = pt;
	if(mMapPtr) Flush(mMapPtr.Ptr(), pt);

	UNGUARD;
}

/*
====================
Flush
====================
*/
VOID BrushCircle::Flush(const Map* map, const Vector3& pt)
{
	GUARD(BrushCircle::Flush);

	CHECK(map);

	// compute the view
	F32 radius = this->Radius();
	Vector4 rect(pt[0]-radius, pt[1]-radius, pt[0]+radius, pt[1]+radius);
	Rect src, dst, clip;
/*	
	src.left	= U2P(rect[0])/U2P(CHUNK_STRIDE);
	src.top		= U2P(rect[1])/U2P(CHUNK_STRIDE);
	src.right	= (U2P(rect[2])%U2P(CHUNK_STRIDE)) ? (U2P(rect[2])/U2P(CHUNK_STRIDE)+1) : U2P(rect[2])/U2P(CHUNK_STRIDE);
	src.bottom	= (U2P(rect[3])%U2P(CHUNK_STRIDE)) ? (U2P(rect[3])/U2P(CHUNK_STRIDE)+1) : U2P(rect[3])/U2P(CHUNK_STRIDE);
*/
	src.left	= (U2P(rect[0])%U2P(CHUNK_STRIDE)) ? (U2P(rect[0])/U2P(CHUNK_STRIDE)-1) : U2P(rect[0])/U2P(CHUNK_STRIDE);
	src.top		= (U2P(rect[1])%U2P(CHUNK_STRIDE)) ? (U2P(rect[1])/U2P(CHUNK_STRIDE)-1) : U2P(rect[1])/U2P(CHUNK_STRIDE);
	src.right	= (U2P(rect[2])%U2P(CHUNK_STRIDE)) ? (U2P(rect[2])/U2P(CHUNK_STRIDE)+1) : U2P(rect[2])/U2P(CHUNK_STRIDE);
	src.bottom	= (U2P(rect[3])%U2P(CHUNK_STRIDE)) ? (U2P(rect[3])/U2P(CHUNK_STRIDE)+1) : U2P(rect[3])/U2P(CHUNK_STRIDE);

	dst.left	= -map->mWidth/2;
	dst.top		= 0;
	dst.right	= map->mWidth/2;
	dst.bottom	= map->mHeight;	
	IntersectRect(&clip, &dst, &src);
	for(I32 j = clip.top; j < clip.bottom; j++)
	{
		for(I32 i = clip.left; i < clip.right; i++)
		{
			std::pair<I32,I32> coord(i,j);
			std::map< std::pair<I32,I32>, ChunkPtr >::const_iterator it = map->mChunks.find(coord);
			if(it == map->mChunks.end()) continue;
			EChunkPtr chunk =  dynamic_ptr_cast<EChunk>(it->second);
			Matrix world = Matrix::makeTranslate(i*CHUNK_STRIDE,j*CHUNK_STRIDE,0);
			Matrix world_inv; world_inv.invert(world);
			Vector3 point = pt * world_inv;
			this->Flush(chunk.Ptr(), point);
		}
	}

	UNGUARD;
}

//////////////////////////////////////////////////////////////////////////

// flush the chunk
VOID BrushCircle::Flush(const Chunk* c, const Vector3& pt)
{
	GUARD(BrushCircle::Flush);

	EChunk*chunk = dynamic_cast<EChunk*>((Chunk*)c);
	CHECK(chunk);

	F32 radius = this->Radius();
	F32 softness = this->Softness();
	F32 strength = this->Strength();	

	// compute the valid area
	Rect dst, src, clip;
	dst.left	= 0;
	dst.top		= 0;
	dst.right	= U2P(CHUNK_STRIDE);
	dst.bottom	= U2P(CHUNK_STRIDE);
	src.left	= U2P(pt[0]-radius);
	src.top		= U2P(pt[1]-radius);
	src.right	= U2P(pt[0]+radius);
	src.bottom	= U2P(pt[1]+radius);
	if(!IntersectRect(&clip, &dst, &src)) return;

	I32 scale = U2P(CHUNK_STRIDE)/U2P(ALPHA_STRIDE);
	clip.left	= clip.left/scale;
	clip.top	= clip.top/scale;
	clip.right	= clip.right/scale;
	clip.bottom	= clip.bottom/scale;
	if(IsRectEmpty(&clip)) return;

	U32 width = U2P(ALPHA_STRIDE);
	U32 height = U2P(ALPHA_STRIDE);

	// find the current layer
	U32 layer_cur = chunk->Find(this->Name(), this->Scale());

	// calculate the alpha
	for(I32 j = clip.top; j < clip.bottom; j++)
	{
		for(I32 i = clip.left; i < clip.right; i++)
		{
			F32 dist = Vector2(pt[0]-P2U(i*scale) ,pt[1]-P2U(j*scale)).length();
			if(dist >= radius) continue;
			U32 n = i + (height - 1 - j) * width;

			// calculate the alpha value
			F32 length = radius*strength;
			F32 ratio = (dist-length)/(radius-length);
			ratio = 1.0f - GMath::clamp(ratio, 1.0f, 0.0f);
			ratio *= softness;
			U8 alpha = (U8)(ratio * 255.0f);

			// set the value for the current layer			
			{
				EChunk::Layer& layer = chunk->mLayers[layer_cur];
				U8& alpha_cur = layer.alpha[n];
				U8 alpha_old = alpha_cur;
				alpha = (U8)GMath::max((I32)alpha, (I32)alpha_cur);
				alpha_cur = alpha;
				layer.total += ((I32)alpha_cur - (I32)alpha_old);
			}

			// set the alpha of the other layer
			if(alpha == 255)
			{
				// if the current layer is fully saturated then the other layers must be zero
				for(I32 k = 0; k < chunk->mLayers.size(); k++)
				{
					if(k == layer_cur) continue;
					EChunk::Layer& layer = chunk->mLayers[k];
					U8&alpha_cur = layer.alpha[n];
					U8 alpha_old = alpha_cur;
					alpha_cur = 0;
					layer.total += ((I32)alpha_cur - (I32)alpha_old);
				}
			}
			else
			{
				// normalize the remaining layers not yet considered.
				F32 sum_other = 0;
				for(I32 k = 0; k < chunk->mLayers.size(); k++)
				{
					if(k == layer_cur) continue;
					U8&alpha_cur = chunk->mLayers[k].alpha[n];
					sum_other += (F32)alpha_cur;
				}
				if(sum_other != 0)
				{
					F32 ratio = (F32)(255.0f - (F32)alpha) / sum_other;
					for(I32 k = 0; k < chunk->mLayers.size(); k++)
					{
						if(k == layer_cur) continue;
						EChunk::Layer& layer = chunk->mLayers[k];
						U8&alpha_cur = layer.alpha[n];
						U8 alpha_old = alpha_cur;
						alpha_cur = (U8)(((F32)alpha_cur)*ratio);
						layer.total += ((I32)alpha_cur - (I32)alpha_old);
					}
				}
			}
		}
	}

	// update the chunk
	chunk->Update();

	UNGUARD;
}

//////////////////////////////////////////////////////////////////////////

/*
====================
Draw
====================
*/
VOID BrushCircle::Draw(Graph *gc)
{
	GUARD(BrushCircle::Draw);

	// set the world space
	AUTO_LOCK_WORLD(gc);
	gc->world(Matrix::makeScale(this->Radius(),this->Radius(),this->Radius())*Matrix::makeTranslate(mPos)*gc->world());

	// draw the line
	if(mLinePtr)mLinePtr->draw(gc);

	UNGUARD;
}

//////////////////////////////////////////////////////////////////////////
// BrushLozenge
//////////////////////////////////////////////////////////////////////////

BrushLozenge::BrushLozenge(const Map* map) : Brush(map)
{
	mLinePtr = GNEW(Line);
	if(mLinePtr)
	{		
		mLinePtr->add(Vector3(0.0f,					0.0f,				0.0f));
		mLinePtr->add(Vector3(P2U(TILE_WIDTH/2),	P2U(TILE_HEIGHT/2),	0.0f));
		mLinePtr->add(Vector3(0.0f,					P2U(TILE_HEIGHT),	0.0f));
		mLinePtr->add(Vector3(-P2U(TILE_WIDTH/2),	P2U(TILE_HEIGHT/2),	0.0f));
		mLinePtr->add(0); mLinePtr->add(1);
		mLinePtr->add(1); mLinePtr->add(2);
		mLinePtr->add(2); mLinePtr->add(3);
		mLinePtr->add(3); mLinePtr->add(0);
		mLinePtr->color(Vector4(1,1,0,1));
		mLinePtr->build();
	}
}

BrushLozenge::~BrushLozenge()
{

}

/*
====================
DoMove
====================
*/
VOID BrushLozenge::DoMove(const Vector3& pt)
{
	GUARD(BrushLozenge::DoMove);

	I32 px, py, tx, ty;
	px = U2P(pt[0]);
	py = U2P(pt[1]);
	P2T(tx, ty, px, py);
	T2P(px, py, tx, ty);
	mPos.set(P2U(px), P2U(py),0.0f);

	UNGUARD;
}

/*
====================
DoDrag
====================
*/
VOID BrushLozenge::DoDrag(const Vector3& pt)
{
	GUARD(BrushLozenge::DoDrag);

	I32 px, py, tx, ty;
	px = U2P(pt[0]);
	py = U2P(pt[1]);
	P2T(tx, ty, px, py);
	T2P(px, py, tx, ty);
	mPos.set(P2U(px), P2U(py),0.0f);	

	UNGUARD;
}

//////////////////////////////////////////////////////////////////////////
/*
====================
Draw
====================
*/
VOID BrushLozenge::Draw(Graph *gc)
{
	GUARD(BrushLozenge::Draw);	

	// set the world space
	AUTO_LOCK_WORLD(gc);
	gc->world(Matrix::makeScale(this->Radius(),this->Radius(),this->Radius())*Matrix::makeTranslate(mPos)*gc->world());

	// draw the line
	if(mLinePtr)mLinePtr->draw(gc);

	UNGUARD;
}

//////////////////////////////////////////////////////////////////////////

/*
====================
Flush
====================
*/
VOID BrushLozenge::Flush(const Map* map, const Vector3& pt)
{
	GUARD(BrushLozenge::Flush);

	// compute the view
	F32 radius = this->Radius();
	Vector4 rect(pt[0]-P2U(TILE_WIDTH/2), pt[1], pt[0]+P2U(TILE_WIDTH/2), pt[1]+P2U(TILE_HEIGHT));
	Rect src, dst, clip;
/*	
	src.left	= U2P(rect[0])/U2P(CHUNK_STRIDE);
	src.top		= U2P(rect[1])/U2P(CHUNK_STRIDE);
	src.right	= (U2P(rect[2])%U2P(CHUNK_STRIDE)) ? (U2P(rect[2])/U2P(CHUNK_STRIDE)+1) : U2P(rect[2])/U2P(CHUNK_STRIDE);
	src.bottom	= (U2P(rect[3])%U2P(CHUNK_STRIDE)) ? (U2P(rect[3])/U2P(CHUNK_STRIDE)+1) : U2P(rect[3])/U2P(CHUNK_STRIDE);
*/
	src.left	= (U2P(rect[0])%U2P(CHUNK_STRIDE)) ? (U2P(rect[0])/U2P(CHUNK_STRIDE)-1) : U2P(rect[0])/U2P(CHUNK_STRIDE);
	src.top		= (U2P(rect[1])%U2P(CHUNK_STRIDE)) ? (U2P(rect[1])/U2P(CHUNK_STRIDE)-1) : U2P(rect[1])/U2P(CHUNK_STRIDE);
	src.right	= (U2P(rect[2])%U2P(CHUNK_STRIDE)) ? (U2P(rect[2])/U2P(CHUNK_STRIDE)+1) : U2P(rect[2])/U2P(CHUNK_STRIDE);
	src.bottom	= (U2P(rect[3])%U2P(CHUNK_STRIDE)) ? (U2P(rect[3])/U2P(CHUNK_STRIDE)+1) : U2P(rect[3])/U2P(CHUNK_STRIDE);

	dst.left	= -map->mWidth/2;
	dst.top		= 0;
	dst.right	= map->mWidth/2;
	dst.bottom	= map->mHeight;	
	IntersectRect(&clip, &dst, &src);
	for(I32 j = clip.top; j < clip.bottom; j++)
	{
		for(I32 i = clip.left; i < clip.right; i++)
		{
			std::pair<I32,I32> coord(i,j);
			std::map< std::pair<I32,I32>, ChunkPtr >::const_iterator it = map->mChunks.find(coord);
			if(it == map->mChunks.end()) continue;
			EChunkPtr chunk =  dynamic_ptr_cast<EChunk>(it->second);
			Matrix world = Matrix::makeTranslate(i*CHUNK_STRIDE,j*CHUNK_STRIDE,0);
			Matrix world_inv; world_inv.invert(world);
			Vector3 point = pt * world_inv;
			this->Flush(chunk.Ptr(), point);
		}
	}

	UNGUARD;
}

VOID BrushLozenge::Flush(const Chunk* c, const Vector3& pt)
{
	GUARD(BrushLozenge::Flush);

	UNGUARD;
}

//////////////////////////////////////////////////////////////////////////