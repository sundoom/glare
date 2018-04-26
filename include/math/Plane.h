/*
 *  Plane.h
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __Plane__
#define __Plane__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Math)

//////////////////////////////////////////////////////////////////////////
	
	// a*x + b*y + c*z + d = 0
	class Plane
	{
	public:
		enum { SIDE_FRONT = 1, SIDE_BACK = 2, SIDE_CROSS = SIDE_FRONT | SIDE_BACK };

		// constructor
		Plane();

		// constructor from a normal and a distance
		Plane(F32 nx, F32 ny, F32 nz, F32 d);		

		// constructor from a point on the plane and a normal
		Plane(const Vector3& p, const Vector3& n);

		// Constructor from three points
		Plane(const Vector3& p0, const Vector3& p1, const Vector3& p2);

		// constructor from a normal and a distance
		Plane(const Vector3& n, F32 d);

		// copy constructor
		Plane(const Plane& plane);

		// destructor
		~Plane();		

		Plane& zero();

		Plane& set(F32 nx, F32 ny, F32 nz, F32 d);

		Plane& set(const Vector3& normal, F32 d);

		Plane& set(const Vector3& p, const Vector3& n);

		// Computes the plane equation from 3 points.
		Plane& set(const Vector3& p0, const Vector3& p1, const Vector3& p2);

		F32 distance(const Vector3& p) const;

		BOOL belongs(const Vector3& p) const;

		// projects p into the plane
		Vector3 project(const Vector3 & p) const;

		// find an arbitrary point in the plane
		Vector3 pointInPlane() const;

		VOID normalize();

		I32 boxOnSide( const Vector3 & mins, const Vector3 & maxs ) const;

		// p = p0
		const Plane& operator=(const Plane& plane);

		// p = p0 * m
		Plane operator * (const class Matrix & right) const;

	private:
		Vector3	_n;	// the normal to the plane
		F32		_d;	// the distance from the origin

	private: friend class Frustum;
	};	

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////