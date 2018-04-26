/*
 *  Brush2D.h
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __Brush2D__
#define __Brush2D__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

// this class provides an object-oriented Brush2D interface.
class Brush2D : public GReference
{
public:
	// constructor
	Brush2D(const Map2D* map);

	// destructor
	virtual ~Brush2D();

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
	Map2DPtr mMapPtr;
};

typedef GPtr<class Brush2D>Brush2DPtr;

//////////////////////////////////////////////////////////////////////////

// this class provides an object-oriented brush circle interface.
class Brush2DCircle : public Brush2D
{
public:
	// constructor
	Brush2DCircle(const Map2D* map);

	// destructor
	virtual ~Brush2DCircle();

	// drag the mouse
	VOID DoDrag(const Vector3& pt);

	// draw the brush
	VOID Draw(Graph *gc);

private:
	// flush the map
	VOID Flush(const Map2D* map, const Vector3& pt);

	// flush the chunk
	VOID Flush(const Chunk2D* chunk, const Vector3& pt);

	// build the brush
	VOID Build();

private:
	LinePtr mLinePtr;
};

typedef GPtr<class Brush2DCircle>Brush2DCirclePtr;

//////////////////////////////////////////////////////////////////////////

// this class provides an object-oriented brush lozenge interface.
class Brush2DLozenge : public Brush2D
{
public:
	// constructor
	Brush2DLozenge(const Map2D* map);

	// destructor
	virtual ~Brush2DLozenge();

	// move the mouse
	VOID DoMove(const Vector3& pt);	

	// drag the mouse
	VOID DoDrag(const Vector3& pt);	

	// draw the brush
	VOID Draw(Graph *gc);

private:
	// flush the chunk
	VOID Flush(const Map2D* map, const Vector3& pt);

private:
	LinePtr mLinePtr;
};

typedef GPtr<class Brush2DLozenge>Brush2DLozengePtr;

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __Brush2D__

//////////////////////////////////////////////////////////////////////////