/*
 *  Frustum.cpp
 *  glare
 *
 *  Created by sunjun on 11-6-14.
 *  Copyright 2011 sunstdio. All rights reserved.
 *
 */
//////////////////////////////////////////////////////////////////////////

#include <GMath.h>

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(Math)

//////////////////////////////////////////////////////////////////////////

	// constructor
	Frustum::Frustum()
	{

	}

	// constructor from a cull matrix
	Frustum::Frustum(const Matrix& m )
	{
		set( m );
	}

	// copy constructor
	Frustum::Frustum(const Frustum& frustum)
	{
		planes[0] = frustum.planes[0];
		planes[1] = frustum.planes[1];
		planes[2] = frustum.planes[2];
		planes[3] = frustum.planes[3];
		planes[4] = frustum.planes[4];
		planes[5] = frustum.planes[5];
	}

	// Destructor
	Frustum::~Frustum()
	{
	}

	const Frustum& Frustum::operator=(const Frustum& frustum)
	{
		planes[0] = frustum.planes[0];
		planes[1] = frustum.planes[1];
		planes[2] = frustum.planes[2];
		planes[3] = frustum.planes[3];
		planes[4] = frustum.planes[4];
		planes[5] = frustum.planes[5];
		return *this;
	}

	Frustum& Frustum::set( const Matrix& cull )
	{
		const F32 * mat = cull.ptr();

		// right plane
		planes[0]._n[0] = mat[3] - mat[0];
		planes[0]._n[1] = mat[7] - mat[4];
		planes[0]._n[2] = mat[11] - mat[8];
		planes[0]._d	= mat[15] - mat[12];
		planes[0].normalize();
		// left plane
		planes[1]._n[0] = mat[3] + mat[0];
		planes[1]._n[1] = mat[7] + mat[4];
		planes[1]._n[2] = mat[11] + mat[8];
		planes[1]._d	= mat[15] + mat[12];
		planes[1].normalize();
		// bottom plane
		planes[2]._n[0] = mat[3] + mat[1];
		planes[2]._n[1] = mat[7] + mat[5];
		planes[2]._n[2] = mat[11] + mat[9];
		planes[2]._d	= mat[15] + mat[13];
		planes[2].normalize();
		// top plane
		planes[3]._n[0] = mat[3] - mat[1];
		planes[3]._n[1] = mat[7] - mat[5];
		planes[3]._n[2] = mat[11] - mat[9];
		planes[3]._d	= mat[15] - mat[13];
		planes[3].normalize();
		// near plane
		planes[4]._n[0] = mat[3] + mat[2];
		planes[4]._n[1] = mat[7] + mat[6];
		planes[4]._n[2] = mat[11] + mat[10];
		planes[4]._d	= mat[15] + mat[14];
		planes[4].normalize();
		// far plane
		planes[5]._n[0] = mat[3] - mat[2];
		planes[5]._n[1] = mat[7] - mat[6];
		planes[5]._n[2] = mat[11] - mat[10];
		planes[5]._d	= mat[15] - mat[14];
		planes[5].normalize();
		return *this;
	}		

	// check an point in the frustum
	BOOL Frustum::pointInFrustum(const Vector3& p) const
	{
		for(I32 i = 0; i < 6; i++ )
		{
			if( planes[i].distance( p ) < 0 ) return FALSE;
		}
		return TRUE;
	}

	// check a sphere in the frustum
	BOOL Frustum::sphereInFrustum(const Vector3 &p, F32 radius)
	{
		for(I32 i=0; i < 6; i++) 
		{
			F32 distance = planes[i].distance(p);
			if( distance < -radius )
				return FALSE;
		}
		return TRUE;
	}

	BOOL Frustum::boxInFrustum( const Vector3 & mins, const Vector3 & maxs ) const
	{
		for( I32 i = 0; i < 6; i++ )
		{
			if( planes[i].boxOnSide( mins, maxs ) == Plane::SIDE_BACK )
				return FALSE;
		}
		return TRUE;
	}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
