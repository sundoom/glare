/*
 *  EMap2D.h
 *  glow
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __EMap2D__
#define __EMap2D__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

// this class provides an object-oriented EMap2D interface.
class EMap2D : public Map2D
{
public:
	// constructor
	EMap2D();

	// destructor
	virtual ~EMap2D();

	// load the data
	VOID Load(const GData* data);

	// update the map
	VOID Update(const Vector4& viewport);

	// draw the reference line
	VOID DrawReferenceLine(Graph *gc);

private:
	// LinePtr mReferenceLine;
	std::list<LinePtr>mLines;
};

//////////////////////////////////////////////////////////////////////////

typedef GPtr<class EMap2D>EMap2DPtr;

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __EMap2D__

//////////////////////////////////////////////////////////////////////////