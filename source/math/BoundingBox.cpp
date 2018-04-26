/*
 *  BoundingBox.cpp
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
	BoundingBox::BoundingBox()
	{
		_min.set(MAX_F32,MAX_F32,MAX_F32);
		_max.set(MIN_F32,MIN_F32,MIN_F32);
	}

	// constructor from the given extents
	BoundingBox::BoundingBox(const Vector3& min,const Vector3& max)
	{
		_min = min;
		_max = max;
	}

	// constructor from the given extents
	BoundingBox::BoundingBox(F32 xmin, F32 ymin, F32 zmin, F32 xmax, F32 ymax, F32 zmax)
	{
		_min.set(xmin,ymin,zmin);
		_max.set(xmax,ymax,zmax);
	}

	// copy constructor
	BoundingBox::BoundingBox(const BoundingBox& box)
	{
		_min = box._min;
		_max = box._max;
	}

	// Destructor
	BoundingBox::~BoundingBox()
	{
	}

	// returns true if the bounding box extents are valid, false otherwise.
	BOOL BoundingBox::valid() const
	{
		return (_max.x()>=_min.x() &&  _max.y()>=_min.y() &&  _max.z()>=_min.z()) ? TRUE : FALSE;
	}

	// sets the bounding box extents.
	VOID BoundingBox::set(const Vector3& min,const Vector3& max)
	{
		_min = min;
		_max = max;
	}

	VOID BoundingBox::set(F32 xmin, F32 ymin, F32 zmin, F32 xmax, F32 ymax, F32 zmax)
	{
		_min.set(xmin,ymin,zmin);
		_max.set(xmax,ymax,zmax);
	}

	const BoundingBox& BoundingBox::operator=(const BoundingBox& box)
	{
		_min = box._min;
		_max = box._max;
		return *this;
	}

	Vector3& BoundingBox::min() 
	{ 
		return _min; 
	}

	Vector3 BoundingBox::min() const 
	{ 
		return _min; 
	}

	Vector3& BoundingBox::max() 
	{ 
		return _max; 
	}

	Vector3 BoundingBox::max() const 
	{ 
		return _max; 
	}

	// calculates and returns the bounding box center.
	const Vector3 BoundingBox::center() const
	{
		return (_min+_max)*0.5;
	}

	// calculates and returns the bounding box radius.
	F32 BoundingBox::radius() const
	{
		return 0.5f*(_max-_min).length();
	}

	// expands the bounding box to include the given coordinate.
	VOID BoundingBox::expand(const Vector3& v)
	{
		if(v.x()<_min.x()) _min.x() = v.x();
		if(v.x()>_max.x()) _max.x() = v.x();

		if(v.y()<_min.y()) _min.y() = v.y();
		if(v.y()>_max.y()) _max.y() = v.y();

		if(v.z()<_min.z()) _min.z() = v.z();
		if(v.z()>_max.z()) _max.z() = v.z();
	}

	// expands the bounding box to include the given coordinate.
	VOID BoundingBox::expand(F32 x,F32 y,F32 z)
	{
		if(x<_min.x()) _min.x() = x;
		if(x>_max.x()) _max.x() = x;

		if(y<_min.y()) _min.y() = y;
		if(y>_max.y()) _max.y() = y;

		if(z<_min.z()) _min.z() = z;
		if(z>_max.z()) _max.z() = z;
	}

	// expands this bounding box to include the given bounding box.
	VOID BoundingBox::expand(const BoundingBox& bb)
	{
		if (!bb.valid()) return;

		if(bb._min.x()<_min.x()) _min.x() = bb._min.x();
		if(bb._max.x()>_max.x()) _max.x() = bb._max.x();

		if(bb._min.y()<_min.y()) _min.y() = bb._min.y();
		if(bb._max.y()>_max.y()) _max.y() = bb._max.y();

		if(bb._min.z()<_min.z()) _min.z() = bb._min.z();
		if(bb._max.z()>_max.z()) _max.z() = bb._max.z();
	}

	// returns the intersection of this bounding box and the specified bounding box.
	BoundingBox BoundingBox::cut(const BoundingBox& bb) const
	{    
		return BoundingBox(
			GMath::max(_min.x(),bb._min.x()),GMath::max(_min.y(),bb._min.y()),GMath::max(_min.z(),bb._min.z()), 
			GMath::min(_max.x(),bb._max.x()),GMath::min(_max.y(),bb._max.y()),GMath::min(_max.z(),bb._max.z()) );
	}
	
	// return true if this bounding box intersect the specified bounding box.
	BOOL BoundingBox::intersect(const BoundingBox& bb) const
	{
		return (GMath::max(_min.x(),bb._min.x()) <= GMath::min(_max.x(),bb._max.x()) &&
			GMath::max(_min.y(),bb._min.y()) <= GMath::min(_max.y(),bb._max.y()) &&
			GMath::max(_min.z(),bb._min.z()) <= GMath::min(_max.z(),bb._max.z())) ? TRUE : FALSE;
	}

	// returns true if this bounding box contains the specified coordinate.
	BOOL BoundingBox::contains(const Vector3& v) const
	{
		return (valid() && 
			(v.x()>=_min.x() && v.x()<=_max.x()) &&
			(v.y()>=_min.y() && v.y()<=_max.y()) &&
			(v.z()>=_min.z() && v.z()<=_max.z())) ? TRUE : FALSE;
	}

	// clip the line
	BOOL BoundingBox::clip(Vector3& s, Vector3& e) const
	{    
		Vector3 bb_min(_min);
		Vector3 bb_max(_max);
/*
		F32 epsilon = 1e-4;
		bb_min.x() -= epsilon;
		bb_min.y() -= epsilon;
		bb_min.z() -= epsilon;
		bb_max.x() += epsilon;
		bb_max.y() += epsilon;
		bb_max.z() += epsilon;
*/
		// compate s and e against the xMin to xMax range of bb.
		if (s.x()<=e.x())
		{
			// trivial reject of segment wholely outside.
			if (e.x()<bb_min.x()) return FALSE;
			if (s.x()>bb_max.x()) return FALSE;

			if (s.x()<bb_min.x())
			{
				// clip s to xMin.
				s = s+(e-s)*(bb_min.x()-s.x())/(e.x()-s.x());
			}

			if (e.x()>bb_max.x())
			{
				// clip e to xMax.
				e = s+(e-s)*(bb_max.x()-s.x())/(e.x()-s.x());
			}
		}
		else
		{
			if (s.x()<bb_min.x()) return FALSE;
			if (e.x()>bb_max.x()) return FALSE;

			if (e.x()<bb_min.x())
			{
				// clip s to xMin.
				e = s+(e-s)*(bb_min.x()-s.x())/(e.x()-s.x());
			}

			if (s.x()>bb_max.x())
			{
				// clip e to xMax.
				s = s+(e-s)*(bb_max.x()-s.x())/(e.x()-s.x());
			}
		}

		// compate s and e against the yMin to yMax range of bb.
		if (s.y()<=e.y())
		{
			// trivial reject of segment wholely outside.
			if (e.y()<bb_min.y()) return FALSE;
			if (s.y()>bb_max.y()) return FALSE;

			if (s.y()<bb_min.y())
			{
				// clip s to yMin.
				s = s+(e-s)*(bb_min.y()-s.y())/(e.y()-s.y());
			}

			if (e.y()>bb_max.y())
			{
				// clip e to yMax.
				e = s+(e-s)*(bb_max.y()-s.y())/(e.y()-s.y());
			}
		}
		else
		{
			if (s.y()<bb_min.y()) return FALSE;
			if (e.y()>bb_max.y()) return FALSE;

			if (e.y()<bb_min.y())
			{
				// clip s to yMin.
				e = s+(e-s)*(bb_min.y()-s.y())/(e.y()-s.y());
			}

			if (s.y()>bb_max.y())
			{
				// clip e to yMax.
				s = s+(e-s)*(bb_max.y()-s.y())/(e.y()-s.y());
			}
		}

		// compate s and e against the zMin to zMax range of bb.
		if (s.z()<=e.z())
		{
			// trivial reject of segment wholely outside.
			if (e.z()<bb_min.z()) return FALSE;
			if (s.z()>bb_max.z()) return FALSE;

			if (s.z()<bb_min.z())
			{
				// clip s to zMin.
				s = s+(e-s)*(bb_min.z()-s.z())/(e.z()-s.z());
			}

			if (e.z()>bb_max.z())
			{
				// clip e to zMax.
				e = s+(e-s)*(bb_max.z()-s.z())/(e.z()-s.z());
			}
		}
		else
		{
			if (s.z()<bb_min.z()) return FALSE;
			if (e.z()>bb_max.z()) return FALSE;

			if (e.z()<bb_min.z())
			{
				// clip s to zMin.
				e = s+(e-s)*(bb_min.z()-s.z())/(e.z()-s.z());
			}

			if (s.z()>bb_max.z())
			{
				// clip e to zMax.
				s = s+(e-s)*(bb_max.z()-s.z())/(e.z()-s.z());
			}
		}

		return TRUE;
	}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
