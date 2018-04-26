/*
 *  Brush.h
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __Brush__
#define __Brush__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

// this class provides an object-oriented Brush interface.
class Brush : public GReference
{
public:
	// constructor
	Brush(const Map* map);

	// destructor
	virtual ~Brush();

	// set/get the texture name of the brush
	VOID Name(const CHAR*name);
	const CHAR* Name() const;

	// set/get the texture scale of the brush
	VOID Scale(const Vector2& s);
	const Vector2& Scale() const;

	// set/get the radius of the brush
	VOID Radius(F32 r);
	F32 Radius() const;

	// set/get the softness of the brush
	VOID Softness(F32 s);
	F32 Softness() const;

	// set/get the strength of the brush
	VOID Strength(F32 s);
	F32 Strength() const;	

	// move the mouse
	virtual VOID DoMove(const Vector3& pt);	

	// drag the mouse
	virtual VOID DoDrag(const Vector3& pt) = 0;

	// draw the brush
	virtual VOID Draw(Graph *gc) = 0;

protected:
	Str mName;	
	Vector2 mScale;
	F32 mRadius;
	F32 mSoftness;
	F32 mStrength;
	Vector3 mPos;
	MapPtr mMapPtr;
};

typedef GPtr<class Brush>BrushPtr;

//////////////////////////////////////////////////////////////////////////

// this class provides an object-oriented brush circle interface.
class BrushCircle : public Brush
{
public:
	// constructor
	BrushCircle(const Map* map);

	// destructor
	virtual ~BrushCircle();

	// drag the mouse
	VOID DoDrag(const Vector3& pt);

	// draw the brush
	VOID Draw(Graph *gc);

private:
	// flush the map
	VOID Flush(const Map* map, const Vector3& pt);

	// flush the chunk
	VOID Flush(const Chunk* chunk, const Vector3& pt);

	// build the brush
	VOID Build();

private:
	LinePtr mLinePtr;
};

typedef GPtr<class BrushCircle>BrushCirclePtr;

//////////////////////////////////////////////////////////////////////////

// this class provides an object-oriented brush lozenge interface.
class BrushLozenge : public Brush
{
public:
	// constructor
	BrushLozenge(const Map* map);

	// destructor
	virtual ~BrushLozenge();

	// move the mouse
	VOID DoMove(const Vector3& pt);	

	// drag the mouse
	VOID DoDrag(const Vector3& pt);	

	// draw the brush
	VOID Draw(Graph *gc);

private:
	// flush the chunk
	VOID Flush(const Map* map, const Vector3& pt);

	// flush the chunk
	VOID Flush(const Chunk* chunk, const Vector3& pt);

private:
	LinePtr mLinePtr;
};

typedef GPtr<class BrushLozenge>BrushLozengePtr;

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __Brush__

//////////////////////////////////////////////////////////////////////////