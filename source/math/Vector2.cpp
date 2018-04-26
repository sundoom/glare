/*
 *  Vector2.cpp
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

	Vector2::Vector2()
	{
		_v[0]=0.0; _v[1]=0.0;
	}

	Vector2::Vector2(F32 v)
	{
		_v[0] = v;
		_v[1] = v;
	}

	Vector2::Vector2(F32 x,F32 y) 
	{ 
		_v[0]=x; 
		_v[1]=y; 
	}

	Vector2::Vector2(const F32 v[])
	{
		_v[0] = v[0];
		_v[1] = v[1];
	}

	Vector2::Vector2(const Vector2 &rhs)
	{
		_v[0] = rhs._v[0];
		_v[1] = rhs._v[1];
	}

	const Vector2& Vector2::operator=(const Vector2& rhs)
	{
		_v[0] = rhs._v[0];
		_v[1] = rhs._v[1];
		return *this;
	}

	BOOL Vector2::equals(const Vector2 & v, F32 epsilon) const
	{
		return
			(::fabsf(_v[0]-v._v[0]) < epsilon &&
			 ::fabsf(_v[1]-v._v[1]) < epsilon) ? TRUE : FALSE;
	}

	BOOL Vector2::operator == (const Vector2& v) const 
	{ 
		return (_v[0]==v._v[0] && _v[1]==v._v[1]) ? TRUE : FALSE; 
	}

	BOOL Vector2::operator != (const Vector2& v) const 
	{ 
		return (_v[0]!=v._v[0] || _v[1]!=v._v[1]) ? TRUE : FALSE; 
	}

	BOOL Vector2::operator <  (const Vector2& v) const
	{
		return ((_v[0] < v._v[0])&&(_v[1] < v._v[1])) ? TRUE : FALSE;
	}

	F32 * Vector2::ptr() 
	{ 
		return _v; 
	}

	const F32 * Vector2::ptr() const 
	{ 
		return _v; 
	}

	VOID Vector2::set( F32 x, F32 y ) 
	{ 
		_v[0]=x; 
		_v[1]=y; 
	}

	VOID Vector2::set( const Vector2& rhs)
	{
		_v[0]=rhs._v[0]; _v[1]=rhs._v[1];
	}

	F32 & Vector2::operator [] (U32 i) 
	{ 
		return _v[i]; 
	}

	F32 Vector2::operator [] (U32 i) const 
	{ 
		return _v[i]; 
	}

	F32 & Vector2::x() 
	{ 
		return _v[0]; 
	}

	F32 & Vector2::y() 
	{ 
		return _v[1]; 
	}

	F32 Vector2::x() const 
	{ 
		return _v[0]; 
	}

	F32 Vector2::y() const 
	{ 
		return _v[1]; 
	}

	// dot product.
	F32 Vector2::operator * (const Vector2& rhs) const
	{
		return _v[0]*rhs._v[0]+_v[1]*rhs._v[1];
	}

	// multiply by scalar.
	const Vector2 Vector2::operator * (F32 rhs) const
	{
		return Vector2(_v[0]*rhs, _v[1]*rhs);
	}

	// unary multiply by scalar.
	Vector2& Vector2::operator *= (F32 rhs)
	{
		_v[0]*=rhs;
		_v[1]*=rhs;
		return *this;
	}

	// divide by scalar.
	const Vector2 Vector2::operator / (F32 rhs) const
	{
		return Vector2(_v[0]/rhs, _v[1]/rhs);
	}

	// unary divide by scalar.
	Vector2& Vector2::operator /= (F32 rhs)
	{
		_v[0]/=rhs;
		_v[1]/=rhs;
		return *this;
	}

	// binary vector add.
	const Vector2 Vector2::operator + (const Vector2& rhs) const
	{
		return Vector2(_v[0]+rhs._v[0], _v[1]+rhs._v[1]);
	}

	// unary vector add. slightly more efficient because no temporary intermediate object.
	Vector2& Vector2::operator += (const Vector2& rhs)
	{
		_v[0] += rhs._v[0];
		_v[1] += rhs._v[1];
		return *this;
	}

	// binary vector subtract.
	const Vector2 Vector2::operator - (const Vector2& rhs) const
	{
		return Vector2(_v[0]-rhs._v[0], _v[1]-rhs._v[1]);
	}

	// unary vector subtract.
	Vector2& Vector2::operator -= (const Vector2& rhs)
	{
		_v[0]-=rhs._v[0];
		_v[1]-=rhs._v[1];
		return *this;
	}

	// negation operator. returns the negative of the vector.
	const Vector2 Vector2::operator - () const
	{
		return Vector2 (-_v[0], -_v[1]);
	}

	// length of the vector = sqrt( vec . vec )
	F32 Vector2::length() const
	{
		return sqrtf( _v[0]*_v[0] + _v[1]*_v[1] );
	}

	// length squared of the vector = vec . vec
	F32 Vector2::length2() const
	{
		return _v[0]*_v[0] + _v[1]*_v[1];
	}

	// normalize the vector so that it has length unity. * returns the previous length of the vector.
	F32 Vector2::normalize()
	{
		F32 norm = Vector2::length();
		if (norm>0.0)
		{
			F32 inv = 1.0f/norm;
			_v[0] *= inv;
			_v[1] *= inv;
		}
		return( norm );
	}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END

//////////////////////////////////////////////////////////////////////////
