/*
 *  Plane.cpp
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

	Plane::Plane()
	{

	}

	// constructor from a normal and a distance
	Plane::Plane(F32 nx, F32 ny, F32 nz, F32 d)
	{
		set(nx, ny, nz, d);
	}

	// constructor from a point on the plane and a normal
	Plane::Plane(const Vector3& p, const Vector3& n)
	{
		set(p, n);
	}

	// constructor from three points
	Plane::Plane(const Vector3& p0, const Vector3& p1, const Vector3& p2)
	{
		set(p0, p1, p2);
	}

	// constructor from a normal and a distance
	Plane::Plane(const Vector3& n, F32 d) : _n(n), _d(d)
	{
	}

	// copy constructor
	Plane::Plane(const Plane& plane) : _n(plane._n), _d(plane._d)
	{
	}

	// destructor
	Plane::~Plane()
	{
	}	

	Plane& Plane::zero()
	{
		_n.set(0,0,0);
		_d = 0.0f;
		return *this;
	}

	Plane& Plane::set(F32 nx, F32 ny, F32 nz, F32 d)
	{
		_n.set(nx, ny, nz);
		_d = d;
		return *this;
	}

	Plane& Plane::set(const Vector3& n, F32 d)
	{
		_n = n;
		_d = d;
		return *this;
	}

	Plane& Plane::set(const Vector3& p, const Vector3& n)
	{
		_n = n;
		// Plane equation: a*x + b*y + c*z + d = 0
		// p belongs to plane so:
		//     a*p.x + b*p.y + c*p.z + d = 0
		// <=> (n|p) + d = 0
		// <=> d = - (n|p)
		_d = -(p*n);
		return *this;
	}

	/**
	Computes the plane equation from 3 points.
	*/
	Plane& Plane::set(const Vector3& p0, const Vector3& p1, const Vector3& p2)
	{
		Vector3 Edge0 = p1 - p0;
		Vector3 Edge1 = p2 - p0;

		_n = Edge0^Edge1;
		_n.normalize();

		// See comments in set() for computation of d
		_d = -(p0*_n);

		return	*this;
	}

	F32 Plane::distance(const Vector3& p) const
	{
		// Valid for plane equation a*x + b*y + c*z + d = 0
		return p*_n + _d;
	}

	BOOL Plane::belongs(const Vector3& p) const
	{
		return (GMath::abs(distance(p)) < (1.0e-7f)) ? TRUE : FALSE;
	}

	/**
	projects p into the plane
	*/
	Vector3 Plane::project(const Vector3 & p) const
	{
		// Pretend p is on positive side of plane, i.e. plane.distance(p)>0.
		// To project the point we have to go in a direction opposed to plane's normal, i.e.:
		return p - _n * distance(p);
		// return p + normal * distance(p);
	}

	/**
	find an arbitrary point in the plane
	*/
	Vector3 Plane::pointInPlane() const
	{
		// Project origin (0,0,0) to plane:
		// (0) - normal * distance(0) = - normal * ((p|(0)) + d) = -normal*d
		return - _n * _d;
		// return normal * d;
	}

	VOID Plane::normalize()
	{
		F32 denom = 1.0f / _n.length();
		_n[0] *= denom;
		_n[1] *= denom;
		_n[2] *= denom;
		_d *= denom;
	}

	I32 Plane::boxOnSide( const Vector3 & mins, const Vector3 & maxs ) const
	{
		Vector3 corners[2];
		for( I32 i = 0; i < 3 ; i++ )
		{
			if( _n[i] < 0 )
			{
				corners[0][i] = mins[i];
				corners[1][i] = maxs[i];
			}
			else
			{
				corners[1][i] = mins[i];
				corners[0][i] = maxs[i];
			}
		}
		F32 dist1 = distance( corners[0] );
		F32 dist2 = distance( corners[1] );
		I32 sides = 0;
		if(dist1 >= 0) sides = SIDE_FRONT;
		if (dist2 < 0) sides |= SIDE_BACK;
		return sides;
	}

	const Plane& Plane::operator=(const Plane& plane)
	{
		_n = plane._n;
		_d = plane._d;
		return *this;
	}

	Plane Plane::operator * (const Matrix & right) const
	{
		/*	MxMat m = right.inverse();
		Plane dst;
		dst.normal.x = normal.x * m.m[0] + normal.y * m.m[1] + normal.z * m.m[2] + d * m.m[3];
		dst.normal.y = normal.x * m.m[4] + normal.y * m.m[5] + normal.z * m.m[6] + d * m.m[7];
		dst.normal.z = normal.x * m.m[8] + normal.y * m.m[9] + normal.z * m.m[10] + d * m.m[11];
		dst.d		 = normal.x * m.m[12] + normal.y * m.m[13] + normal.z * m.m[14] + d * m.m[15];
		dst.normal.normalize();
		*/

		const F32 *mat = right.ptr();

		Plane dst;
		dst._n[0] = mat[0] * _n[0] + mat[4] * _n[1] + mat[8] * _n[2];
		dst._n[1] = mat[1] * _n[0] + mat[5] * _n[1] + mat[9] * _n[2];
		dst._n[2] = mat[2] * _n[0] + mat[6] * _n[1] + mat[10] * _n[2];
		dst._d = _d - (mat[12] * dst._n[0] + mat[13] * dst._n[1] + mat[14] * dst._n[2]);
		return dst;
	}


//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
