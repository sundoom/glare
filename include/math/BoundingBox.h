/*
 *  BoundingBox.h
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __BoundingBox__
#define __BoundingBox__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

#include <Plane.h>

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Math)

//////////////////////////////////////////////////////////////////////////
		
	class BoundingBox
	{
	public:
		// constructor
		BoundingBox();

		// constructor from the given extents
		BoundingBox(const Vector3& min,const Vector3& max);

		// constructor from the given extents
		BoundingBox(F32 xmin, F32 ymin, F32 zmin, F32 xmax, F32 ymax, F32 zmax);

		// copy constructor
		BoundingBox(const BoundingBox& box);

		// Destructor
		~BoundingBox();

		// returns true if the bounding box extents are valid, false otherwise.
		BOOL valid() const;

		// sets the bounding box extents.
		VOID set(const Vector3& min,const Vector3& max);
		VOID set(F32 xmin, F32 ymin, F32 zmin, F32 xmax, F32 ymax, F32 zmax);

		const BoundingBox& operator=(const BoundingBox& box);

		Vector3& min();
		Vector3 min() const;

		Vector3& max();
		Vector3 max() const;

		// calculates and returns the bounding box center.
		const Vector3 center() const;

		// calculates and returns the bounding box radius.
		F32 radius() const;

		// expands the bounding box to include the given coordinate.
		VOID expand(const Vector3& v);

		// expands the bounding box to include the given coordinate.
		VOID expand(F32 x,F32 y,F32 z);

		// expands this bounding box to include the given bounding box.
		VOID expand(const BoundingBox& bb);

		// returns the intersection of this bounding box and the specified bounding box.
		BoundingBox cut(const BoundingBox& bb) const;

		// return true if this bounding box intersects the specified bounding box.
		BOOL intersect(const BoundingBox& bb) const;

		// returns true if this bounding box contains the specified coordinate.
		BOOL contains(const Vector3& v) const;

		// clip the line
		BOOL clip(Vector3& s, Vector3& e) const;

	private: 
		Vector3 _min, _max;
	};

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////