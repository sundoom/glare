/*
 *  Vector3.cpp
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

	Vector3::Vector3() 
	{ 
		_v[0]=0.0f; _v[1]=0.0f; _v[2]=0.0f;
	}

	Vector3::Vector3(F32 v)
	{
		_v[0]=v; _v[1]=v; _v[2]=v; 
	}

	Vector3::Vector3(F32 x,F32 y,F32 z) 
	{ 
		_v[0]=x; _v[1]=y; _v[2]=z; 
	}
	
	Vector3::Vector3(const Vector2& v2,F32 zz)
	{
		_v[0] = v2[0];
		_v[1] = v2[1];
		_v[2] = zz;
	}

	Vector3::Vector3(const F32 v[])
	{
		_v[0] = v[0];
		_v[1] = v[1];
		_v[2] = v[2];
	}
	
	Vector3::Vector3(const Vector3& rhs)
	{
		_v[0] = rhs._v[0];
		_v[1] = rhs._v[1];
		_v[2] = rhs._v[2];
	}

	const Vector3& Vector3::operator=(const Vector3& rhs)
	{
		_v[0] = rhs._v[0];
		_v[1] = rhs._v[1];
		_v[2] = rhs._v[2];
		return *this;
	}

	BOOL Vector3::equals(const Vector3 & v, F32 epsilon) const
	{
		return
			(::fabsf(_v[0]-v._v[0]) < epsilon &&
			::fabsf(_v[1]-v._v[1]) < epsilon &&
			::fabsf(_v[2]-v._v[2]) < epsilon) ? TRUE : FALSE;
	}

	BOOL Vector3::operator == (const Vector3& v) const 
	{ 
		return (_v[0]==v._v[0] && _v[1]==v._v[1] && _v[2]==v._v[2]) ? TRUE : FALSE; 
	}

	BOOL Vector3::operator != (const Vector3& v) const 
	{ 
		return (_v[0]!=v._v[0] || _v[1]!=v._v[1] || _v[2]!=v._v[2]) ? TRUE : FALSE; 
	}

	BOOL Vector3::operator <  (const Vector3& v) const
	{
		return ((_v[0] < v._v[0])&&(_v[1] < v._v[1])&&(_v[2] < v._v[2])) ? TRUE : FALSE;
	}

	F32* Vector3::ptr() 
	{
		return _v; 
	}
	
	const F32* Vector3::ptr() const 
	{ 
		return _v; 
	}

	VOID Vector3::set( F32 x, F32 y, F32 z)
	{
		_v[0]=x; _v[1]=y; _v[2]=z;
	}

	VOID Vector3::set( const Vector3& rhs)
	{
		_v[0]=rhs._v[0]; _v[1]=rhs._v[1]; _v[2]=rhs._v[2];
	}

	F32& Vector3::operator [] (U32 i) 
	{ 
		return _v[i]; 
	}

	F32 Vector3::operator [] (U32 i) const 
	{ 
		return _v[i]; 
	}

	F32& Vector3::x() 
	{ 
		return _v[0]; 
	}
	
	F32& Vector3::y() 
	{ 
		return _v[1]; 
	}

	F32& Vector3::z() 
	{ 
		return _v[2]; 
	}

	F32 Vector3::x() const 
	{ 
		return _v[0]; 
	}

	F32 Vector3::y() const 
	{ 
		return _v[1]; 
	}

	F32 Vector3::z() const 
	{ 
		return _v[2]; 
	}

	// dot product.
	F32 Vector3::operator * (const Vector3& rhs) const
	{
		return _v[0]*rhs._v[0]+_v[1]*rhs._v[1]+_v[2]*rhs._v[2];
	}

	// cross product.
	const Vector3 Vector3::operator ^ (const Vector3& rhs) const
	{
		return Vector3(
			_v[1]*rhs._v[2]-_v[2]*rhs._v[1],
			_v[2]*rhs._v[0]-_v[0]*rhs._v[2] ,
			_v[0]*rhs._v[1]-_v[1]*rhs._v[0]);
	}

	// multiply by scalar.
	const Vector3 Vector3::operator * (F32 rhs) const
	{
		return Vector3(_v[0]*rhs, _v[1]*rhs, _v[2]*rhs);
	}

	// unary multiply by scalar.
	Vector3& Vector3::operator *= (F32 rhs)
	{
		_v[0]*=rhs;
		_v[1]*=rhs;
		_v[2]*=rhs;
		return *this;
	}

	// divide by scalar.
	const Vector3 Vector3::operator / (F32 rhs) const
	{
		return Vector3(_v[0]/rhs, _v[1]/rhs, _v[2]/rhs);
	}

	// unary divide by scalar.
	Vector3& Vector3::operator /= (F32 rhs)
	{
		_v[0]/=rhs;
		_v[1]/=rhs;
		_v[2]/=rhs;
		return *this;
	}

	// binary vector add.
	const Vector3 Vector3::operator + (const Vector3& rhs) const
	{
		return Vector3(_v[0]+rhs._v[0], _v[1]+rhs._v[1], _v[2]+rhs._v[2]);
	}

	// unary vector add. slightly more efficient because no temporary intermediate object.
	Vector3& Vector3::operator += (const Vector3& rhs)
	{
		_v[0] += rhs._v[0];
		_v[1] += rhs._v[1];
		_v[2] += rhs._v[2];
		return *this;
	}

	// binary vector subtract.
	const Vector3 Vector3::operator - (const Vector3& rhs) const
	{
		return Vector3(_v[0]-rhs._v[0], _v[1]-rhs._v[1], _v[2]-rhs._v[2]);
	}

	// unary vector subtract.
	Vector3& Vector3::operator -= (const Vector3& rhs)
	{
		_v[0]-=rhs._v[0];
		_v[1]-=rhs._v[1];
		_v[2]-=rhs._v[2];
		return *this;
	}

	// negation operator. returns the negative of the vector.
	const Vector3 Vector3::operator - () const
	{
		return Vector3 (-_v[0], -_v[1], -_v[2]);
	}

	// length of the vector = sqrt( vec . vec )
	F32 Vector3::length() const
	{
		return ::sqrtf( _v[0]*_v[0] + _v[1]*_v[1] + _v[2]*_v[2] );
	}

	// length squared of the vector = vec . vec
	F32 Vector3::length2() const
	{
		return _v[0]*_v[0] + _v[1]*_v[1] + _v[2]*_v[2];
	}

	// normalize the vector so that it has length unity. returns the previous length of the vector.
	F32 Vector3::normalize()
	{
		F32 norm = Vector3::length();
		if (norm>0.0)
		{
			F32 inv = 1.0f/norm;
			_v[0] *= inv;
			_v[1] *= inv;
			_v[2] *= inv;
		}                
		return( norm );
	}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
