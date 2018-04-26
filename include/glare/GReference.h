
/*
 *  GReference.h
 *  glare
 *
 *  Created by sunjun on 11-6-10.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __GReference__
#define __GReference__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

// base class from providing referencing counted objects.
class GReference
{
public:
	GReference();		

	// increment the reference count by one, indicating that 
	// this object has another pointer which is referencing it.
	VOID IncRef() const;

	// decrement the reference count by one, indicating that 
	// a pointer to this object is referencing it.  If the
	// reference count goes to zero, it is assumed that this object
	// is no longer referenced and is automatically deleted.
	VOID DecRef() const;

	// return the number pointers currently referencing this object.
	I32 RefCount() const;

protected:
	virtual ~GReference();

protected:
	volatile mutable I32 mCount;
};

//////////////////////////////////////////////////////////////////////////

typedef GPtr<GReference>GReferencePtr;

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif // __GReference__

//////////////////////////////////////////////////////////////////////////
