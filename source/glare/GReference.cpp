/*
 *  GReference.cpp
 *  glare
 *
 *  Created by sunjun on 11-6-10.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#include <Glare.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

GReference::GReference() : mCount(0)
{

}

GReference::~GReference()
{

}

/*
====================
IncRef
====================
*/
VOID GReference::IncRef() const
{
	++mCount;
}

/*
====================
DecRef
====================
*/
VOID GReference::DecRef() const
{
	if(--mCount == 0) GDELETE(this);
}

/*
====================
RefCount
====================
*/
I32 GReference::RefCount() const
{
	return mCount;
}

//////////////////////////////////////////////////////////////////////////
