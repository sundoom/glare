//////////////////////////////////////////////////////////////////////////

#include "Editor.h"

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

Brush2D::Brush2D(const Map2D* map)
{
	mName = "map/grass.tga";	
	mScale.set(1.0f,1.0f);
	mRadius = 1.0f;
	mSoftness = 1.0f;
	mStrength = 1.0f;
	mMapPtr = (Map2D*)map;
}

Brush2D::~Brush2D()
{
	
}

/*
====================
Name
====================
*/
VOID Brush2D::Name(const CHAR*name)
{
	CHECK(name);
	mName = name;
}

const CHAR* Brush2D::Name() const
{
	return mName.c_str();
}

/*
====================
Scale
====================
*/
VOID Brush2D::Scale(const Vector2& s)
{
	mScale = s;
}

const Vector2& Brush2D::Scale() const
{
	return mScale;
}

/*
====================
Radius
====================
*/
VOID Brush2D::Radius(F32 r)
{
	mRadius = r;
	mRadius = GMath::clamp(mRadius, 100.0f, 1.0f);
}

F32 Brush2D::Radius() const
{
	return mRadius;
}

/*
====================
Softness
====================
*/
VOID Brush2D::Softness(F32 s)
{
	mSoftness = s;
	mSoftness = GMath::clamp(mSoftness, 1.0f, 0.1f);
}

F32 Brush2D::Softness() const
{
	return mSoftness;
}

/*
====================
Strength
====================
*/
VOID Brush2D::Strength(F32 s)
{
	mStrength = s;
	mStrength = GMath::clamp(mStrength, 1.0f, 0.0f);
}

F32 Brush2D::Strength() const
{
	return mStrength;
}

/*
====================
DoMove
====================
*/
VOID Brush2D::DoMove(const Vector3& pt)
{
	mPos = pt;
}

//////////////////////////////////////////////////////////////////////////
// Brush2DCircle
//////////////////////////////////////////////////////////////////////////

Brush2DCircle::Brush2DCircle(const Map2D* map) : Brush2D(map)
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

Brush2DCircle::~Brush2DCircle()
{

}

/*
====================
DoDrag
====================
*/
VOID Brush2DCircle::DoDrag(const Vector3& pt)
{
	GUARD(Brush2DCircle::DoDrag);

	mPos = pt;
	if(mMapPtr) Flush(mMapPtr.Ptr(), pt);

	UNGUARD;
}

/*
====================
Flush
====================
*/
VOID Brush2DCircle::Flush(const Map2D* map, const Vector3& pt)
{
	GUARD(Brush2DCircle::Flush);

	CHECK(map);

	// compute the view
	F32 radius = this->Radius();
	Vector4 rect(pt[0]-radius, pt[1]-radius, pt[0]+radius, pt[1]+radius);
	Rect src, dst, clip;
	src.left	= U2P(rect[0])/U2P(CHUNK_STRIDE);
	src.top		= U2P(rect[1])/U2P(CHUNK_STRIDE);
	src.right	= (U2P(rect[2])%U2P(CHUNK_STRIDE)) ? (U2P(rect[2])/U2P(CHUNK_STRIDE)+1) : U2P(rect[2])/U2P(CHUNK_STRIDE);
	src.bottom	= (U2P(rect[3])%U2P(CHUNK_STRIDE)) ? (U2P(rect[3])/U2P(CHUNK_STRIDE)+1) : U2P(rect[3])/U2P(CHUNK_STRIDE);
	dst.left	= 0;
	dst.top		= 0;
	dst.right	= map->mWidth;
	dst.bottom	= map->mHeight;	
	IntersectRect(&clip, &dst, &src);
	for(U32 j = clip.top; j < clip.bottom; j++)
	{
		for(U32 i = clip.left; i < clip.right; i++)
		{
			EChunk2DPtr chunk =  dynamic_ptr_cast<EChunk2D>(map->mChunks[i+j*map->mWidth]);
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
VOID Brush2DCircle::Flush(const Chunk2D* c, const Vector3& pt)
{
	GUARD(Brush2DCircle::Flush);

	EChunk2D*chunk = dynamic_cast<EChunk2D*>((Chunk2D*)c);
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
				EChunk2D::Layer& layer = chunk->mLayers[layer_cur];
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
					EChunk2D::Layer& layer = chunk->mLayers[k];
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
						EChunk2D::Layer& layer = chunk->mLayers[k];
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
VOID Brush2DCircle::Draw(Graph *gc)
{
	GUARD(Brush2DCircle::Draw);

	// set the world space
	AUTO_LOCK_WORLD(gc);
	gc->world(Matrix::makeScale(this->Radius(),this->Radius(),this->Radius())*Matrix::makeTranslate(mPos)*gc->world());

	// draw the line
	if(mLinePtr)mLinePtr->draw(gc);

	UNGUARD;
}

//////////////////////////////////////////////////////////////////////////
// Brush2DLozenge
//////////////////////////////////////////////////////////////////////////

Brush2DLozenge::Brush2DLozenge(const Map2D* map) : Brush2D(map)
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

Brush2DLozenge::~Brush2DLozenge()
{

}

/*
====================
DoMove
====================
*/
VOID Brush2DLozenge::DoMove(const Vector3& pt)
{
	GUARD(Brush2DLozenge::DoMove);

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
VOID Brush2DLozenge::DoDrag(const Vector3& pt)
{
	GUARD(Brush2DLozenge::DoDrag);

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
VOID Brush2DLozenge::Draw(Graph *gc)
{
	GUARD(Brush2DLozenge::Draw);	

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
VOID Brush2DLozenge::Flush(const Map2D* map, const Vector3& pt)
{
	GUARD(Brush2DLozenge::Flush);

	UNGUARD;
}


//////////////////////////////////////////////////////////////////////////