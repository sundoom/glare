/*
 *  Frustum.h
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////////

#ifndef __Frustum__
#define __Frustum__

//////////////////////////////////////////////////////////////////////////

#if defined __cplusplus

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Math)

//////////////////////////////////////////////////////////////////////////
		
	class Frustum
	{
	public:
		// constructor
		Frustum();

		// constructor from a cull matrix
		Frustum(const Matrix& m );

		// copy constructor
		Frustum(const Frustum& frustum);

		// Destructor
		~Frustum();

		const Frustum& operator=(const Frustum& frustum);

		Frustum& set( const Matrix& cull );

		// check an point in the frustum
		BOOL pointInFrustum(const Vector3& p) const;

		// check a sphere in the frustum
		BOOL sphereInFrustum(const Vector3 &p, F32 radius);

		// check a box in the frustum
		BOOL boxInFrustum( const Vector3 & mins, const Vector3 & maxs ) const;

	private:
		Plane	planes[6];		// the frustum plane
	};

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////